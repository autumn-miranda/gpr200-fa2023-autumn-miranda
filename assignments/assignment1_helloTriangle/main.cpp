#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h> //Open GL
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[9] = { //all positions in the unit cube
	//x	//y	//z
	-0.5,  -0.5,  0.0,	//bottom left
	 0.5,  -0.5,  0.0,	//bottom right
	 0.0,   0.5,  0.0 	//top center
};

//Shader Sources
const char* vertexShaderSource = R"(
		#version 450
		layout(location = 0) in vec3 vPos;
		void main(){
			gl_Position = vec4(vPos, 1.0);
		}
	)";

const char* fragmentShaderSource = R"(
		#version 450
		out vec4 FragColor;
		void main(){
			FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)";

//FINISH Function Later
//create a new vertex array with vertex data
/*unsigned int createVAO(float* vertexData, int numVerticies)
{
	
};*/

//unsigned int createShader(GLenum shadeType, const char* sourceCode);

//Creates a new shader program with vertex + fragment stages
//Returns id of new shader program if successful, 0 if failed
//unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

//gl_Position - from model straight to clip space
int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);//a seperate library that creates the window
	//gets window events and inputs
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//vao responsible for defining attributes
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	

	//define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allocate space for vertex data and send to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//define position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);


	//create shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check for success
	int successv;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successv);
	if (!successv)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("failed to compile vshader: %s", infoLog);
	}

	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	//check for success
	int successf;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successf);
	if (!successf)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		printf("failed to compile fshader: %s", infoLog);
	}

	//create a program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw Call
		glUseProgram(shaderProgram);//pull from program
		glBindVertexArray(vao);//pull from shader
		glDrawArrays(GL_TRIANGLES, 0, 3);//the draw call itself
		//start at 0, read in 3 vertices
		glfwSwapBuffers(window);
	}//gl - calls to OpenGL API
	//gl calls run on the GPU

	printf("Shutting down...");
}
