#include "Tools.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* InitOpenGL(int ScreenWidth, int ScreenHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "创建GLFW窗口失败 Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败 Failed to initialize GLAD" << std::endl;
	}

	framebuffer_size_callback(window, ScreenWidth, ScreenHeight);
	Input::SetWindow(window);

	return window;
}

void Render(GLFWwindow* window, float *clearColor, GLbitfield clearType, void renderFun())
{

}
