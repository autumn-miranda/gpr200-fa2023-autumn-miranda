#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h> //Open GL
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[21] = { //all positions in the unit cube
	 //x    //y   //z   //r   //g   //b   //a
	-0.5,  -0.5,  0.0,	1.0,  0.0,  0.0,  1.0, //bottom left
	 0.5,  -0.5,  0.0,	0.0,  1.0,  0.0,  1.0, //bottom right
	 0.0,   0.5,  0.0, 	0.0,  0.0,  1.0,  1.0  //top center
};

//Shader Sources
const char* vertexShaderSource = R"(
		#version 450
		layout(location = 0) in vec3 vPos;
		layout(location = 1) in vec4 vColor;
		out vec4 Color;
		uniform float _Time;
		void main(){
			Color = vColor;
			vec3 offset = vec3(0,sin(vPos.x + _Time), 0) * 0.5;
			gl_Position = vec4(vPos+offset, 1.0);
		}
	)";

const char* fragmentShaderSource = R"(
		#version 450
		out vec4 FragColor;
		in vec4 Color;
		uniform float _Time = 1.0;
		void main(){
			FragColor = Color * abs(sin(_Time));
		}
	)";

//create a new vertex array with vertex data
unsigned int createVAO(float* vertexData, int numVertices)
{
	//vao responsible for defining attributes
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allocate space for vertex data and send to GPU
	int stride = 7;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stride * numVertices, vertexData, GL_STATIC_DRAW);//I edited this line of code during class on 9/12
	//in glBufferData we can't just do sizeof(vertexData) because it will return the size of the pointer, not the array

	//define position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (const void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (const void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);
	
	return vao;
};

unsigned int createShader(GLenum shadeType, const char* sourceCode) 
{
	unsigned int shader = glCreateShader(shadeType);
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	return shader;
};

//Creates a new shader program with vertex + fragment stages
//Returns id of new shader program if successful, 0 if failed
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) 
{
	//create shader object
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	
	//create a program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	//check for success
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("failed to compile vshader: %s", infoLog);
		return 0;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("failed to compile fshader: %s", infoLog);
		return 0;
	}

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
		return 0;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;

};

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

	
	unsigned int vao = createVAO(vertices, 3);
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//Draw Call
		glUseProgram(shaderProgram);//pull from program
		glBindVertexArray(vao);//pull from shader

		//Time
		//Current time in seconds this frame
		float time = (float)glfwGetTime();
		//get location of uniform by name
		int timeLocation = glGetUniformLocation(shaderProgram, "_Time");
		//set the value of _Time uniform
		glUniform1f(timeLocation, time);

		glDrawArrays(GL_TRIANGLES, 0, 3);//the draw call itself
		//start at 0, read in 3 vertices
		glfwSwapBuffers(window);
	}//gl - calls to OpenGL API
	//gl calls run on the GPU

	printf("Shutting down...");
}
