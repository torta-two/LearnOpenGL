#pragma once

#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

class Input
{
public:
	static bool SetWindow(GLFWwindow* win);
	static bool GetKeyDown(int key);

private:
	static GLFWwindow* window;
};

