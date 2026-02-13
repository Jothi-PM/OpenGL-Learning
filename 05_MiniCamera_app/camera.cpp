#include<iostream>
#include"Texture.h"
#include"shaders.h"
#include<GL\glew.h>
#include<glfw\glfw3.h>
#include<opencv2/opencv.hpp>
using namespace std;
// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLint PREVIEW_WIDTH, PREVIEW_HEIGHT;
uint NUM_FILTERS = 3;
GLuint VBO, VAO, EBO, shader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Adjust the viewport when the framebuffer size changes
}

static void addShaders(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	GLint success;
	glGetProgramiv(theShader, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(theShader, 512, NULL, infoLog);
		std::cout << "Program link error: " << infoLog << std::endl;
	}

	glAttachShader(theProgram, theShader);
	glDeleteShader(theShader);
}


static GLuint compileShaders(const char* vertex, const char* frag) {

	shader = glCreateProgram();

	if (!shader)
	{
		std::cout << "glCreateprogram failed \n";
		return 0;
	}

	addShaders(shader, vertex, GL_VERTEX_SHADER);
	addShaders(shader, frag, GL_FRAGMENT_SHADER);


	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return 0;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return 0;
	}
	return shader;
}

static void uploadBuffers()
{
GLfloat vertices[] = {
    // position        // tex coords
    -1.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // bottom-left
     1.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // bottom-right
    -1.0f,  1.0f, 0.0f,  1.0f, 0.0f,  // top-left
     1.0f,  1.0f, 0.0f,  0.0f, 0.0f   // top-right
};

GLuint indices[] = {
    0, 1, 2,
    1, 3, 2
};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void drawFrames(GLuint shader, Texture& camTex)
{
	glUseProgram(shader);
    camTex.useTexture(GL_TEXTURE0);
	GLuint texLoc = glGetUniformLocation(shader, "uTexture");
	if (texLoc == -1) {
		std::cout << "Could not find uniform: uTexture\n";
	}
    glUniform1i(texLoc,0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
    glUseProgram(0);
}


int main() {

    // Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	uploadBuffers();
	GLuint frameShader = compileShaders(vShader, fPassThroughShader);
	GLuint grayShader = compileShaders(vShader, fGrayShader);
	GLuint sepiaShader = compileShaders(vShader, fSepiaShader);
	GLuint edgeShader = compileShaders(vShader, fEdgeShader);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Camera not opened\n";
        return -1;
    }

    cv::Mat frame;
    cap>>frame; // first frame

    std::cout<<frame.cols<<" "<<frame.rows<<" "<<frame.channels()<<"\n";

    // prepare background texture
	Texture camTex;
    camTex.createTexture(frame.cols, frame.rows, frame.channels());
    bool grayScaled = false;

    while (!glfwWindowShouldClose(mainWindow)) {
        if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mainWindow, true);
        
        cap >> frame;
        if (frame.empty()) {
            std::cout << "Empty frame\n";
            break;
        }

        camTex.updateTexture((const void*)frame.data);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
		PREVIEW_WIDTH = bufferWidth / 3;
		PREVIEW_HEIGHT = bufferHeight / 3;
		
		// Draw Frame as is
		glViewport(0, PREVIEW_HEIGHT, bufferWidth, bufferHeight-PREVIEW_HEIGHT);
		drawFrames(frameShader, camTex);

		//grayscaled
		glViewport(0, 0, PREVIEW_WIDTH, PREVIEW_HEIGHT);
		drawFrames(grayShader, camTex);

		// sepia
		glViewport(PREVIEW_WIDTH, 0, PREVIEW_WIDTH, PREVIEW_HEIGHT);
		drawFrames(sepiaShader, camTex);

		//edge detected
		glViewport(2*PREVIEW_WIDTH, 0, PREVIEW_WIDTH, PREVIEW_HEIGHT);
		drawFrames(edgeShader, camTex);
  
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
    return 0;
}
