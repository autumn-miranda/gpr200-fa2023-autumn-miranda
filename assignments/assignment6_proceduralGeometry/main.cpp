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

//#include "../anm/procGen.h"
#include <anm/procGen.h>
//#include <anm/procGen.cpp>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;

struct AppSettings {
	const char* shadingModeNames[6] = { "Solid Color","Normals","UVs","Texture","Lit","Texture Lit"};
	int shadingModeIndex;

	ew::Vec3 bgColor = ew::Vec3(0.1f);
	ew::Vec3 shapeColor = ew::Vec3(1.0f);
	bool wireframe = true;
	bool drawAsPoints = false;
	bool backFaceCulling = true;

	//Euler angles (degrees)
	ew::Vec3 lightRotation = ew::Vec3(0, 0, 0);
}appSettings;

ew::Camera camera;
ew::CameraController cameraController;

ew::Mesh reshapePlane(float width, float height, int subdivisions) {
	ew::MeshData planeMeshData = anm::createPlane(width, height, subdivisions);
	return ew::Mesh(planeMeshData);
};

ew::Mesh reshapeCyl(float height, float radius, int segments) {
	ew::MeshData cylMeshData = anm::createCylinder(height, radius, segments);
	return ew::Mesh(cylMeshData);
};

ew::Mesh reshapeSphere(float radius, int segments) {
	ew::MeshData sphereMeshData = anm::createSphere(radius, segments);
	return ew::Mesh(sphereMeshData);
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

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, appSettings.wireframe ? GL_LINE : GL_FILL);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	//Create cube
	ew::MeshData cubeMeshData = ew::createCube(0.5f);
	ew::Mesh cubeMesh(cubeMeshData);

	//plane
	float planeWidth = 1.0f;
	float planeHeight = 1.0f;
	int planeSubdivisions = 5;
	//previous plane values
	float prevPlaneWidth = 1.0f;
	float prevPlaneHeight = 1.0f;
	int prevPlaneSubdivisions = 5;

	//cylinder
	float cylRad = 1.0f;
	float cylHeight = 3.0f;
	int cylSegments = 9;
	//previous cylinder values
	float prevCylRad = 1.0f;
	float prevCylHeight = 3.0f;
	int prevCylSegments = 9;

	//sphere
	float sphereRad = 1.0f;
	int sphereSegments = 16;
	//previous sphere values
	float prevSphereRad = 1.0f;
	int prevSphereSegments = 16;

	//Create Shapes
	ew::Mesh planeMesh(anm::createPlane(planeWidth, planeHeight, planeSubdivisions));
	
	ew::MeshData cylMeshData = anm::createCylinder(cylHeight, cylRad, cylSegments);
	ew::Mesh cylMesh(cylMeshData);

	ew::MeshData sphereMeshData = anm::createSphere(sphereRad, sphereSegments);
	ew::Mesh sphereMesh(sphereMeshData);

	//Initialize transforms
	ew::Transform cubeTransform;

	ew::Transform planeTransform;
	planeTransform.position = ew::Vec3(1.5f, 0.0f, 0.5f);

	ew::Transform cylTransform;
	cylTransform.position = ew::Vec3(5.0f, 0.0f, 0.5f);

	ew::Transform sphereTransform;
	sphereTransform.position = ew::Vec3(-3.0f, 0.0f, 0.5f);

	resetCamera(camera,cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		cameraController.Move(window, &camera, deltaTime);

		//Render
		glClearColor(appSettings.bgColor.x, appSettings.bgColor.y, appSettings.bgColor.z, 1.0f);

		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//redraw if values change between loops
		if (planeWidth != prevPlaneWidth || planeHeight != prevPlaneHeight || planeSubdivisions != prevPlaneSubdivisions) {
			prevPlaneWidth = planeWidth;
			prevPlaneHeight = planeHeight;
			prevPlaneSubdivisions = planeSubdivisions;
			planeMesh = reshapePlane(planeWidth, planeHeight, planeSubdivisions);
		}
		
		if (cylHeight != prevCylHeight || cylRad != prevCylRad || cylSegments != prevCylSegments) {
			prevCylHeight = cylHeight;
			prevCylRad = cylRad;
			prevCylSegments = cylSegments;
			cylMesh = reshapeCyl(cylHeight, cylRad, cylSegments);
		}

		if (sphereRad != prevSphereRad || sphereSegments != prevSphereSegments) {
			prevSphereRad = sphereRad;
			prevSphereSegments = sphereSegments;
			sphereMesh = reshapeSphere(sphereRad, sphereSegments);
		}


		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setInt("_Mode", appSettings.shadingModeIndex);
		shader.setVec3("_Color", appSettings.shapeColor);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//Euler angels to forward vector
		ew::Vec3 lightRot = appSettings.lightRotation * ew::DEG2RAD;
		ew::Vec3 lightF = ew::Vec3(sinf(lightRot.y) * cosf(lightRot.x), sinf(lightRot.x), -cosf(lightRot.y) * cosf(lightRot.x));
		shader.setVec3("_LightDir", lightF);

		//Draw cube
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);

		shader.setMat4("_Model", cylTransform.getModelMatrix());
		cylMesh.draw((ew::DrawMode)appSettings.drawAsPoints);

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw((ew::DrawMode)appSettings.drawAsPoints);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::Button("Reset Camera")) { resetCamera(camera, cameraController); }
			if (ImGui::Button("Reset Shapes")) { 
				//plane
				planeWidth = 1.0f;
				planeHeight = 1.0f;
				planeSubdivisions = 5;

				planeTransform.position = ew::Vec3(1.5f, 0.0f, 0.5f);
				planeTransform.rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
				planeTransform.scale = ew::Vec3(1.0f, 1.0f, 1.0f);

				//cylinder
				cylRad = 1.0f;
				cylHeight = 3.0f;
				cylSegments = 9;

				cylTransform.position = ew::Vec3(5.0f, 0.0f, 0.5f);
				cylTransform.rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
				cylTransform.scale = ew::Vec3(1.0f, 1.0f, 1.0f);

				//sphere
				sphereRad = 1.0f;
				sphereSegments = 16;

				sphereTransform.position = ew::Vec3(-3.0f, 0.0f, 0.5f);
				sphereTransform.rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
				sphereTransform.scale = ew::Vec3(1.0f, 1.0f, 1.0f);
			}
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

			ImGui::ColorEdit3("BG color", &appSettings.bgColor.x);
			ImGui::ColorEdit3("Shape color", &appSettings.shapeColor.x);
			ImGui::Combo("Shading mode", &appSettings.shadingModeIndex, appSettings.shadingModeNames, IM_ARRAYSIZE(appSettings.shadingModeNames));
			if (appSettings.shadingModeIndex > 3) {
				ImGui::DragFloat3("Light Rotation", &appSettings.lightRotation.x, 1.0f);
			}
			if (ImGui::CollapsingHeader("Plane Values")) {
				ImGui::DragFloat("Width (P)", &planeWidth, 0.1f);
				ImGui::DragFloat("Height (P)", &planeHeight, 0.1f);
				ImGui::DragInt("Subdivisions (P)", &planeSubdivisions, 1);
			}
			if (ImGui::CollapsingHeader("Plane Transform")) {
				ImGui::DragFloat3("Position (P)", &planeTransform.position.x, 0.05f);
				ImGui::DragFloat3("Rotation (P)", &planeTransform.rotation.x, 1.0f);
				ImGui::DragFloat3("Scale (P)", &planeTransform.scale.x, 0.05f);
			}
			if (ImGui::CollapsingHeader("Cylinder Values")) {
				ImGui::DragFloat("Height (C)", &cylHeight, 0.1f);
				ImGui::DragFloat("Radius (C)", &cylRad, 0.1f);
				ImGui::DragInt("Segments (C)", &cylSegments, 1);
			}
			if (ImGui::CollapsingHeader("Cylinder Transform")) {
				ImGui::DragFloat3("Position (C)", &cylTransform.position.x, 0.05f);
				ImGui::DragFloat3("Rotation (C)", &cylTransform.rotation.x, 1.0f);
				ImGui::DragFloat3("Scale (C)", &cylTransform.scale.x, 0.05f);
			}
			if (ImGui::CollapsingHeader("Sphere Values")) {
				ImGui::DragFloat("Radius (S)", &sphereRad, 0.1f);
				ImGui::DragInt("Segments (S)", &sphereSegments, 1);
			}
			if (ImGui::CollapsingHeader("Sphere Transform")) {
				ImGui::DragFloat3("Position (S)", &sphereTransform.position.x, 0.05f);
				ImGui::DragFloat3("Rotation (S)", &sphereTransform.rotation.x, 1.0f);
				ImGui::DragFloat3("Scale (S)", &sphereTransform.scale.x, 0.05f);
			}
			ImGui::Checkbox("Draw as points", &appSettings.drawAsPoints);
			if (ImGui::Checkbox("Wireframe", &appSettings.wireframe)) {
				glPolygonMode(GL_FRONT_AND_BACK, appSettings.wireframe ? GL_LINE : GL_FILL);
			}
			if (ImGui::Checkbox("Back-face culling", &appSettings.backFaceCulling)) {
				if (appSettings.backFaceCulling)
					glEnable(GL_CULL_FACE);
				else
					glDisable(GL_CULL_FACE);
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
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 3);
	camera.target = ew::Vec3(0);
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}
