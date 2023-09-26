#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <anm/shader.h>
#include <anm/shader.cpp>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};

Vertex vertices[4] =
{
	//x   //y  //z  //u  //v 
  {-0.5, -0.5, 0.0, 0.0, 0.0}, //bottom left
  { 0.5, -0.5, 0.0, 1.0, 0.0}, // bottom right
  { 0.5,  0.5, 0.0, 1.0, 1.0},  //top right
  {-0.5,  0.5, 0.0, 0.0, 1.0} //top left
};

unsigned int indices[6] = {
	0, 1, 2, //tri 1 (left tri)
	2, 3, 0 //tri 2 (right tri)
};


//float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
float colorA[3] = { 1.0f, 0.5f, 0.0f };
float colorB[3] = { 0.5f, 0.5f, 0.0f };
bool showImGUIDemoWindow = true;

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices, int stride);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		//Shaded
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 

		anm::Shader shader = anm::Shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
		shader.use();
		float time = (float)glfwGetTime();
		shader.setFloat("_Time", time);
		shader.setFloat("triangleBrightness", triangleBrightness);
		shader.setVec3("colorA", colorA[0], colorA[1], colorA[2]);
		shader.setVec3("colorB", colorB[0], colorB[1], colorB[2]);
		unsigned int vao = createVAO(vertices, 6, indices, 6, 5);
		glBindVertexArray(vao);


		glDrawArrays(GL_TRIANGLES, 0, 3);


		//glDrawArrays(GL_TRIANGLES, 0, 6);//the draw call
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Color 1", colorA);
			ImGui::ColorEdit3("Color 2", colorB);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices, int stride) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * stride, vertexData, GL_STATIC_DRAW);
	
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);


	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//set triangle to 78, 101, 231