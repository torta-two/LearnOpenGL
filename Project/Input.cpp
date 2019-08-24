#include "Input.h"

GLFWwindow* Input::window;

bool Input::SetWindow(GLFWwindow* win)
{
	if (win != NULL)
	{
		window = win;
		return true;
	}
	else
	{
		cout << "Input failed to load window£¡" << endl;
		return false;
	}
}

bool Input::GetKeyDown(int key)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (window != NULL)
		return glfwGetKey(window, key) == GLFW_PRESS;
	else 
		return false;           
}
 
