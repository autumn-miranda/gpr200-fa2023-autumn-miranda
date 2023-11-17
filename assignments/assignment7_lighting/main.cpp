#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <string>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

bool bp = true;

const int MAX_LIGHTS = 3;
int numLights = 1;

struct Light
{
	ew::Vec3 position;//World Space position
	ew::Vec3 color;//RGB
};

struct Material
{
	float ambientK; // ambient coefficient (0-1)
	float diffuseK; //Diffuse Coefficient (0-1)
	float specularK; //Specular coefficient (0-1)
	float shininess;//shininess
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader lightShader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg", GL_REPEAT, GL_LINEAR);

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));
	ew::Mesh lightMesh(ew::createSphere(0.5, 16));

	//Light light1;


	

	Material material;
	material.diffuseK = 0.0f;
	material.ambientK = 0.0f;
	material.specularK = 0.0f;
	material.shininess = 2.0f;

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);

	Light light[MAX_LIGHTS];

	light[0].position = ew::Vec3(0.0f, 2.0f, 0.0f);
	light[0].color = ew::Vec3(1.0f, 0.0f, 0.0f);

	light[1].position = ew::Vec3(1.0f, 2.0f, 0.0f);
	light[1].color = ew::Vec3(0.0f, 1.0f, 0.0f);

	light[2].position = ew::Vec3(-1.0f, 2.0f, 0.0f);
	light[2].color = ew::Vec3(0.0f, 0.0f, 1.0f);

	ew::Transform lightTransform[MAX_LIGHTS];

	for (int i = 0; i < MAX_LIGHTS; i++) {
		lightTransform[i].scale = 0.1;
		lightTransform[i].position = light[i].position;
	}

	resetCamera(camera, cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);
		shader.setVec3("cameraPos", camera.position);
		shader.setBool("BP", bp);
		
		shader.setVec3("_Light[0].color", light[0].color);
		shader.setVec3("_Light[0].position", light[0].position);
		lightTransform[0].position = light[0].position;
		
		shader.setVec3("_Light[1].color", light[1].color);
		shader.setVec3("_Light[1].position", light[1].position);
		lightTransform[1].position = light[1].position;

		shader.setVec3("_Light[1].color", light[1].color);
		shader.setVec3("_Light[1].position", light[1].position);
		lightTransform[1].position = light[1].position;


		shader.setVec3("_Material.diffuseK", material.diffuseK);
		shader.setVec3("_Material.specularK", material.specularK);
		shader.setFloat("_Material.shininess", material.shininess);
		ew::Vec3 allColor;
		for (int i = 0; i < numLights; i++) {
			allColor += light[i].color;
		}
		shader.setVec3("ambientColor", allColor);

		//RENDER
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO: Render point lights
		lightShader.use();
		for (int i = 0; i < numLights; i++) {
			lightShader.setMat4("_Model", lightTransform[i].getModelMatrix());
			lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
			lightShader.setVec3("_Color", light[i].color);
			lightMesh.draw();
		}


		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//Draw shapes
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			ImGui::ColorEdit3("BG color", &bgColor.x);
			ImGui::ColorEdit3("Light Color", &light[0].color.x);
				if (ImGui::DragFloat3("Light 0", &light[0].position.x, 0.1f)) {
					shader.setVec3("_Light[0].position", light[0].position);
					lightTransform[0].position = light[0].position;
					lightShader.setMat4("_Model", lightTransform[0].getModelMatrix());
				}
				if (ImGui::DragFloat3("Light 1", &light[1].position.x, 0.1f)) {
					shader.setVec3("_Light[1].position", light[1].position);
					lightTransform[1].position = light[1].position;
					lightShader.setMat4("_Model", lightTransform[1].getModelMatrix());
				}
				if (ImGui::DragFloat3("Light 2", &light[2].position.x, 0.1f)) {
					shader.setVec3("_Light[2].position", light[2].position);
					lightTransform[2].position = light[2].position;
					lightShader.setMat4("_Model", lightTransform[2].getModelMatrix());
				}
			ImGui::SliderInt("Number of Lights", &numLights, 1, MAX_LIGHTS);
			if (ImGui::Checkbox("Blinn-Phong", &bp)) {
				shader.setBool("BP", bp);
			}
			if (ImGui::DragFloat("Ambient", &material.ambientK, 0.01f, 0.0f, 1.0f)) {
				shader.setFloat("_Material.ambientK", material.ambientK);
			}
			if (ImGui::DragFloat("Diffuse", &material.diffuseK, 0.01f, 0.0f, 1.0f)) {
				shader.setFloat("_Material.diffuseK", material.diffuseK);
			}
			if (ImGui::DragFloat("Specular", &material.specularK, 0.01f, 0.0f, 1.0f)) {
				shader.setFloat("_Material.specularK", material.specularK);
			}
			if (ImGui::DragFloat("Shininess", &material.shininess, 1.0f, 2.0f, 256.0f)) {
				shader.setFloat("_Material.shininess", material.shininess);
			}

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}


