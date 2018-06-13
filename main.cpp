#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();

// Window dimensions
const GLuint WIDTH = 640, HEIGHT = 640;

// Camera control variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), cameraFront = glm::vec3(0.0f, 0.0f, -1.0f), cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f, pitch = 0.0f, lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0;	
bool keys[1024];

//Time between current frame and last frame & Time of last frame
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

int main(void)
{
	bool yes = false, repeat = false;
	int i = 0, x = 0;

	//create the glfw window
	GLFWwindow * window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(HEIGHT, HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);//Make the window's context current
								   
	glfwSetKeyCallback(window, key_callback);// Set the required callback functions

	glfwSetCursorPosCallback(window, mouse_callback);

	//Setting up mouse controls
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialize GLEW to setup OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	//Viewport dimensions
	glViewport(0, 0, 640, 640);

	//Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	//Setting position and colour data
	GLfloat firstLargeTriangle[] = {
		//position data,      colour data
	   -0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.0f,  1.0f, 0.0f, 0.0f,// Right
		0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,// Top 
			
	   -0.4f, -0.4f, 0.2f, 1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.2f,  1.0f, 0.0f, 0.0f,// Right
		0.0f, 0.0f, 0.2f,  1.0f, 0.0f, 0.0f,// Top 

	    0.0f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.2f,  1.0f, 0.0f, 0.0f,// Right
	    0.0f, 0.0f, 0.2f,  1.0f, 0.0f, 0.0f,// Top 
	   -0.4f, 0.4f, 0.2f,  1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.0f,  1.0f, 0.0f, 0.0f,// Right
	    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,// Top 
	    
	   -0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.2f,  1.0f, 0.0f, 0.0f,// Right
	   -0.4f, -0.4f, 0.2f, 1.0f, 0.0f, 0.0f,// Top 
	   -0.4f, 0.4f, 0.2f,  1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.0f,  1.0f, 0.0f, 0.0f,// Right
	   -0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,// Top

	    0.0f, 0.0f, 0.2f,  1.0f, 0.0f, 0.0f,// Left 
	    0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,// Right
	   -0.4f, -0.4f, 0.2f, 1.0f, 0.0f, 0.0f,// Top
       -0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,// Left 
	   -0.4f, -0.4f, 0.2f, 1.0f, 0.0f, 0.0f,// Right
	   -0.0f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// Top  
	};
	GLfloat secondLargeTriangle[] = {
	   -0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Left
		0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Right
		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,// Top 

	   -0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Left
		0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Right
		0.0f, 0.0f, 0.2f,  0.0f, 1.0f, 0.0f,// Top 

		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,// Left
		0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Right
		0.0f, 0.0f, 0.2f,  0.0f, 1.0f, 0.0f,// Top 
		0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Left
		0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Right
		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,// Top 

	   -0.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// Left 
	   -0.0f, 0.0f, 0.2f,  0.0f, 1.0f, 0.0f,// Right
	   -0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Top 
	   -0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Right
	   -0.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// Top

		0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Left
		0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Right
	   -0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Top
       -0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Left 
	   -0.4f, 0.4f, 0.2f,  0.0f, 1.0f, 0.0f,// Right
	    0.4f, 0.4f, 0.0f,  0.0f, 1.0f, 0.0f,// Top 	
	};
	GLfloat mediumTriangle[] = {
		//position data,      colour data
		0.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,// Left 
		0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Right
		0.4f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Top 

		0.4f,  0.0f, 0.2f, 0.0f, 0.0f, 1.0f,// Left 
		0.0f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Right
		0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Top

		0.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,// Left 
		0.4f,  0.0f, 0.2f, 0.0f, 0.0f, 1.0f,// Right
		0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Top 
		0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Left 
		0.0f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Right
		0.4f,  0.0f, 0.2f, 0.0f, 0.0f, 1.0f,// Top 

		0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Left 
		0.0f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Right
		0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Top 
		0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Left 
		0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Right
		0.4f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Top 
		
		0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Left 
		0.4f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,// Right
		0.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,// Top 
		0.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,// Left 
		0.4f,  0.0f, 0.2f, 0.2f, 0.0f, 1.0f,// Right
		0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f,// Top 
	};
	GLfloat firstSmallTriangle[] = {
	   -0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Left
		0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Right
		0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,// Top 

	   -0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Left
		0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Right
		0.0f, 0.0f, 0.2f,   0.0f, 1.0f, 1.0f,// Top

	   -0.0f, -0.0f, 0.0f,  0.0f, 1.0f, 1.0f,// Left
		0.0f,  0.0f, 0.2f,  0.0f, 1.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Top
	   -0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Left
	   -0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Right
		0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f,// Top

	    0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f,// Left
		0.0f,  0.0f, 0.2f,  0.0f, 1.0f, 1.0f,// Right
		0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Top
		0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Left
	    0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Right
		0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f,// Top

		0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Left
		0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Top
	   -0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Left
	   -0.2f, -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,// Right
		0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 1.0f,// Top
	};
	GLfloat secondSmallTriangle[] = {
		0.4f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,// Right
		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 1.0f,// Top 

		0.4f, 0.4f, 0.2f,  1.0f, 1.0f, 1.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 1.0f,// Top 

		0.4f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.4f, 0.4f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 1.0f,// Top 
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.4f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,// Top 

		0.4f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.4f, 0.4f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 1.0f,// Top 
		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.4f, 0.4f, 0.0f,  1.0f, 1.0f, 1.0f,// Top 

		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 1.0f,// Top 
		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 1.0f,// Left
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 1.0f,// Right
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,// Top 
	};
	GLfloat square[] = {
		0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Right
		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Top 
		0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Right
		0.2f,-0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Top 

		0.0f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Top 
		0.0f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.2f, -0.2f, 0.2f, 1.0f, 1.0f, 0.0f,// Top 
		
		0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Right
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Top 
		0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.0f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Top 

		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.2f,-0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Top 
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.2f,-0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.2f,-0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Top 

		0.0f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Left
		0.2f,-0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.2f,-0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Top 
		0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.2f,-0.2f, 0.0,   1.0f, 1.0f, 0.0f,// Right
		0.0f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Top 

		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.2f, 0.2f, 0.2f,  1.0f, 1.0f, 0.0f,// Top 
		0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 0.0f,// Left
		0.4f, 0.0f, 0.2f,  1.0f, 1.0f, 0.0f,// Right
		0.4f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,// Top
	};
	GLfloat parallelogram[] = {
		0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
	   -0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Right
		0.0f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Top 
	   -0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
		0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Top 

	   	0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Left
	   -0.4f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
		0.0f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 
	   -0.4f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Left
		0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 

        0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
	   -0.0f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Right
		0.0f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 
	    0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
		0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
	   -0.0f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Top
	   
	   -0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
       -0.4f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 
	   -0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
	   -0.4f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Top 

	   -0.0f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
	   -0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Right
	   -0.0f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 
       -0.4f, -0.4f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
		0.0f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
	   -0.4f, -0.4f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 

	   -0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Left
	    0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Right
		0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Top 
	   -0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Left
		0.2f, -0.2f, 0.0f,  1.0f, 0.0f, 1.0f,// Right
	   -0.2f, -0.2f, 0.2f,  1.0f, 0.0f, 1.0f,// Top 
	};

	GLuint VBOs[7], VAOs[7];
	glGenVertexArrays(7, VAOs);
	glGenBuffers(7, VBOs);

	//First Large Triangle Setup
	glBindVertexArray(VAOs[0]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstLargeTriangle), firstLargeTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); //Set attribute index of the position attribute to 0 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); //Set colour index of the position attribute to 1 in the vertex shader

	glBindVertexArray(0);

	//Second Large Triangle Setup
	glBindVertexArray(VAOs[1]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondLargeTriangle), secondLargeTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);		
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); 
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Medium Triangle Setup
	glBindVertexArray(VAOs[2]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mediumTriangle), mediumTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//First Small Triangle Setup
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstSmallTriangle), firstSmallTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Second Small Triangle setup
	glBindVertexArray(VAOs[4]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondSmallTriangle), secondSmallTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Square setup
	glBindVertexArray(VAOs[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Parallelogram setup
	glBindVertexArray(VAOs[6]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(parallelogram), parallelogram, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Build & compile shader program
	GLuint shaderProgram = initShader("vert.glsl","frag.glsl");
	glfwSetTime(0.0f);
	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		//Calculate deltatime of current frame
		GLfloat currentFrame = (GLfloat) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		do_movement();
		
		//if statements for delay at the start of animation and before animation repeats & only starts when movment keys are unpressed

		if ((GLfloat)glfwGetTime() * 1.0 >= 2.0 && i == 0){
			i = 1;
			yes = true;
			repeat = false;
			glfwSetTime(0.0f);
		}

		if (x == 0 && (GLfloat)glfwGetTime() * 1.0 >= 5.0){
			yes = true;
			repeat = false;
			x = 0;
			if(!keys[GLFW_KEY_W] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_D]){
				glfwSetTime(0.0f);
			}
		}

		if (x == 1 && (GLfloat)glfwGetTime() * 1.0 >= 5.0){
			yes = false;
			repeat = true;
			x = 0;
			if(!keys[GLFW_KEY_W] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_D]){
				glfwSetTime(0.0f);
			}
		}
		//setting up the cameras position
		glm::mat4 view;
		glm::mat4 projection;
				
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Pass them to the shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//clears colour and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		GLint transformLoc;

		//Drawing First Large Triangle//
		glm::mat4 transform;

		//Draws And Animates The Shape After The Two Second Delay
	    if(yes == true && repeat == false){

			if ((GLfloat)glfwGetTime() * 0.5 <= 1.0){
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f)); 
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.5f, 0.0f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f)); 
				x = 1;
	  }

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if ((GLfloat)glfwGetTime() * 0.5 <= 1.0){
				transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.5f, 0.0f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
				x = 0;
	  }
	
		//Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		
		//Drawing Second Large Triangle//
		transform = glm::mat4();

		//Draws And Animates The Shape After The Two Second Delay
		if(yes == true && repeat == false){
		
			if (glfwGetTime() * 0.6f <= 0.6){
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.6f, (GLfloat)glfwGetTime() * 0.4f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.6f, 0.4f, 0.0f));

			if((GLfloat)glfwGetTime() * 1.5f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if (glfwGetTime() * 0.6f <= 0.6){
				transform = glm::translate(transform, glm::vec3(0.6f, 0.4f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.6f, (GLfloat)glfwGetTime() * -0.4f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

			if((GLfloat)glfwGetTime() * 1.5f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		

		//Drawing Medium Triangle//
		transform = glm::mat4();

		//Draws And Animates The Shape After The Two Second Delay
		if(yes == true && repeat == false){
		
			if (glfwGetTime() * 0.435f <= 0.435){
				transform = glm::translate(transform, glm::vec3(0.0f, -0.0f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.435f, (GLfloat)glfwGetTime() * 0.4f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.435f, 0.4f, 0.0f));

			if((GLfloat)glfwGetTime() * 0.8f <= 1.5708){
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, -1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, -1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(1.5708), glm::vec3(0.0f, 0.0f, -1.0f));
			}

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if (glfwGetTime() * 0.435f <= 0.435){
				transform = glm::translate(transform, glm::vec3(0.435f, 0.4f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.435f, (GLfloat)glfwGetTime() * -0.4f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

			if((GLfloat)glfwGetTime() * 0.8f <= 1.5708){
				transform = glm::rotate(transform, (GLfloat)(1.5708), glm::vec3(0.0f, 0.0f, -1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -0.8f, glm::vec3(0.0f, 0.0f, -1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

		//Drawing First Small Triangle//
		transform = glm::mat4();

		//Draws And Animates The Shape After The Two Second Delay
		if(yes == true && repeat == false){
		
			if (glfwGetTime() * 0.73f <= 0.73){
				transform = glm::translate(transform, glm::vec3(0.0f, -0.0f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.73f, (GLfloat)glfwGetTime() * -0.55f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.73f, -0.55f, 0.0f));

			if((GLfloat)glfwGetTime() * 1.0f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, -1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, -1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, -1.0f));
		}

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if (glfwGetTime() * 0.73f <= 0.73){
				transform = glm::translate(transform, glm::vec3(0.73f, -0.55f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.73f, (GLfloat)glfwGetTime() * 0.55f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

			if((GLfloat)glfwGetTime() * 1.0f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[3]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	

		//Drawing Second Small Triangle//
		transform = glm::mat4();

		//Draws And Animates The Shape After The Two Second Delay
		if(yes == true && repeat == false){
		
			if (glfwGetTime() * 0.117f <= 0.117){
				transform = glm::translate(transform, glm::vec3(0.0f, -0.0f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.032f, (GLfloat)glfwGetTime() * 0.117f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.032f, 0.117f, 0.0f));

			if((GLfloat)glfwGetTime() * 0.8f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, -1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if (glfwGetTime() * 0.117f <= 0.117){
				transform = glm::translate(transform, glm::vec3(0.032f, 0.117f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.032f, (GLfloat)glfwGetTime() * -0.117f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

			if((GLfloat)glfwGetTime() * 0.8f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[4]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	
		//Drawing Square//
		transform = glm::mat4();

		//Draws And Animates The Shape After The Two Second Delay
		if(yes == true && repeat == false){
		
			if (glfwGetTime() * 0.25f <= 0.25){
				transform = glm::translate(transform, glm::vec3(0.0f, -0.0f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.25f, (GLfloat)glfwGetTime() * 0.117f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(-0.25f, 0.117f, 0.0f));

			if((GLfloat)glfwGetTime() * 0.8f <= 0.785398){
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0.785398), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if (glfwGetTime() * 0.117f <= 0.117){
				transform = glm::translate(transform, glm::vec3(-0.25f, 0.117f, 0.0f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.25f, (GLfloat)glfwGetTime() * -0.117f, 0.0f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

			if((GLfloat)glfwGetTime() * 0.8f <= 0.785398){
				transform = glm::rotate(transform, (GLfloat)(0.785398), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		 // Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[5]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
			
		//Drawing Parallelogram//
		transform = glm::mat4();

		if(yes == true && repeat == false){
		
			//Draws And Animates The Shape After The Two Second Delay
			if (glfwGetTime() * 0.25f <= 0.25){

				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * -0.25f, 0.0f, (GLfloat)glfwGetTime() * 0.2f));
			}
			else
				transform = glm::translate(transform, glm::vec3(-0.25f, 0.0f, 0.2f));

			if((GLfloat)glfwGetTime() * 1.0f <= 3.14159){
				//transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(-1.0f, 0.0f, 0.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -1.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(3.14159), glm::vec3(-1.0f, 0.0f, 0.0f));
			
			if((GLfloat)glfwGetTime() * 1.0f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//Return to Original Square when done with previous movements
		if(yes == false && repeat == true){

			if (glfwGetTime() * 0.25f <= 0.25){
				transform = glm::translate(transform, glm::vec3(-0.25f, 0.0f, 0.2f));
				transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.25f, 0.0f,  (GLfloat)glfwGetTime() * -0.2f));
			}
			else
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	
			if((GLfloat)glfwGetTime() * 1.0f <= 3.14159){
				transform = glm::rotate(transform, (GLfloat)(3.14159), glm::vec3(-1.0f, 0.0f, 0.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -1.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(1.0f, 0.0f, 0.0f));
				
			if((GLfloat)glfwGetTime() * 1.0f <= 2.35619){
				transform = glm::rotate(transform, (GLfloat)(2.35619), glm::vec3(.0f, 0.0f, 1.0f));
				transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
				transform = glm::rotate(transform, (GLfloat)(0), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		
		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[6]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12); 

		//Swap front and back buffers 
		glfwSwapBuffers(window);

		//Poll for and process events 
		glfwPollEvents();
	}
	//De-allocate all resources
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}

 //Camera comtrpol methods
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse)
	{
		lastX = (GLfloat) xpos;
		lastY = (GLfloat) ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat) xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat) ypos; // Reversed since y-coordinates go from bottom to left
	lastX = (GLfloat) xpos;
	lastY = (GLfloat) ypos;

	GLfloat sensitivity = (GLfloat) 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//Stops screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}