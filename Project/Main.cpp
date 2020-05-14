#include "Tools.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"

using namespace glm;

const char* vertexPath = "Shader/Vertex.shader";
const char* fragmentPath = "Shader/Fragment.shader";
const string modelPath = "Resources/Objects/nanosuit/nanosuit.obj";

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

vec3 cameraPos = vec3(0, 0, 3);
vec3 cameraFront = vec3(0, 0, -1);
vec3 cameraUp = vec3(0, 1, 0);

float deltaTime = 0.0f;	//当前帧与上一帧的时间差
float lastFrame = 0.0f;	//上一帧deltaTime

float lastX = SCREEN_WIDTH/2;
float lastY = 300;
const float sensitivity = 0.08f;

float myPitch = 0.0f;
float myYaw = -90.0f;

float fov = 45.0f;

vec3 ambientLightColor = vec3(1, 1, 1);
float ambientStrength = 0.3f;

mat4 CameraMove(vec3 &cameraPos, vec3 &cameraFront, vec3 &cameraUp);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float groundVertices[] = 
{
	-1,-1,-1,	
	 1,-1,-1,
	-1,-1, 1,
	 1,-1, 1
};

unsigned int indices[] =
{
	   0, 1, 2, 
	   1, 2, 3 
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main()
{
	GLFWwindow* window = InitOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader shader(vertexPath, fragmentPath);
	Model model(modelPath);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.UseShader();

		/*Gram-Schmidt Process:
		===========================================================
		vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
		vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
		vec3 cameraDirection = normalize(cameraPos - cameraTarget);
		vec3 up = vec3(0.0f, 1.0f, 0.0f);
		//先up后dir计算right为正，先up后dir计算right为负
		vec3 right = normalize(cross(up, cameraDirection));
		up = normalize(cross(cameraDirection, right));		
		===========================================================*/

		glm::mat4 projMatrix = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		shader.SetMatrix("projMatrix", projMatrix);

		mat4 viewMatrix = CameraMove(cameraPos, cameraFront, cameraUp);
		shader.SetMatrix("viewMatrix", viewMatrix);

		glm::mat4 modelMatrix = mat4(1.0f);
		modelMatrix = translate(modelMatrix, vec3(0.0f, -1.75f, 0.0f));
		modelMatrix = scale(modelMatrix, vec3(0.15f, 0.15f, 0.15f));
		shader.SetMatrix("modelMatrix", modelMatrix);
		
		shader.SetFloat("AmbientStrength", ambientStrength);
		shader.SetVec3("AmbientLightColor", ambientLightColor);

		model.Draw(shader);

		//------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}


mat4 CameraMove(vec3 &cameraPos, vec3 &cameraFront, vec3 &cameraUp)
{
	mat4 matrix = mat4(1.0f);

	float speed = 2.5f * deltaTime;

	if (Input::GetKeyDown(GLFW_KEY_LEFT))
		cameraPos -= speed * normalize(cross(cameraFront, cameraUp));
	if (Input::GetKeyDown(GLFW_KEY_RIGHT))
		cameraPos += speed * normalize(cross(cameraFront, cameraUp));
	if (Input::GetKeyDown(GLFW_KEY_UP))
		cameraPos += speed * cameraFront;
	if (Input::GetKeyDown(GLFW_KEY_DOWN))
		cameraPos -= speed * cameraFront;

	matrix = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	return matrix;
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float yOffset = ((float)xpos - lastX) * sensitivity;
	float xOffset = (lastY - (float)ypos) * sensitivity;
	lastX = (float)xpos;
	lastY = (float)ypos;

	myPitch = myPitch + xOffset;
	myYaw = myYaw + yOffset;

	if (myPitch > 89.0f)
		myPitch = 89.0f;
	if (myPitch < -89.0f)
		myPitch = -89.0f;

	vec3 front;
	front.x = cos(radians(myPitch)) * cos(radians(myYaw));
	front.y = sin(radians(myPitch));
	front.z = cos(radians(myPitch)) * sin(radians(myYaw)); 

	cameraFront = normalize(front);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	if (fov >= 1.0f && fov <= 65.0f)
		fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}



