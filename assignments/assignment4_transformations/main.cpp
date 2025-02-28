#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/ewMath/vec3.h>
#include <ew/ewMath/mat4.h>
#include <ew/procGen.h>
#include <anm/shader.h>
#include <anm/transformations.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Square aspect ratio for now. We will account for this with projection later.
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
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

	anm::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	anm::Transform transform[4];

	transform[0].position = ew::Vec3(-0.5f, 0.5f, 0.0f);
	transform[1].position = ew::Vec3(-0.5f, -0.5f, 0.0f);
	transform[2].position = ew::Vec3(0.5f, 0.5f, 0.0f);
	transform[3].position = ew::Vec3(0.5f, -0.5f, 0.0f);

	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//float time = (float)glfwGetTime();
			//ew::Mat4 Translate(ew::Vec3(time, 0.0, 0.0));

		//Set uniforms
		shader.use();
		shader.setMat4("_Model", transform[0].getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", transform[1].getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", transform[2].getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", transform[3].getModelMatrix());
		cubeMesh.draw();

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Transform");
			for (size_t i = 0; i < 4; i++) {
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &transform[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &transform[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &transform[i].scale.x, 0.05f);
				}
				ImGui::PopID();
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
}

