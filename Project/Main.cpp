#include "Tools.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glm;

const char* vertexPath = "shader.vertex";
const char* fragmentPath = "shader.fragment";

const unsigned int SCREEN_WIDTH = 512;
const unsigned int SCREEN_HEIGHT = 512;

unsigned int GetTexture(const char *imgPath, int index, GLenum warpMode, GLenum filterMode);
mat4 CameraMove(vec3 &cameraPos, vec3 &cameraFront, vec3 &cameraUp);

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
	Shader shader(vertexPath, fragmentPath);

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	//解析顶点数据
	//(一个属性就要解析一次，不然电脑傻乎乎的不知道你写的顶点数据啥意思！)
	//param 1：在vertex shader里的location
	//param 2：顶点该属性分量个数
	//param 3：顶点该属性分量数据类型
	//param 4：是否normalize数据（unsigned映射到(0,1),signed映射到(-1,1)）
	//param 5：步长(stride),即一个顶点与下一个顶点之间的数据间隔
	//param 6：该属性与开头的偏移量，void*类型，需要强制转换
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);						//vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));		//texcoord	
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	unsigned int texture1 = GetTexture("wall.jpg", 0, GL_REPEAT, GL_LINEAR);
	unsigned int texture2 = GetTexture("face.jpg", 1, GL_REPEAT, GL_LINEAR);

	vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//------------------------------------------------------------------

		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		shader.UseShader();
		
		//glm::mat4 viewMatrix = glm::mat4(1.0f);
		//viewMatrix = glm::translate(viewMatrix, glm::vec3(x, y, z));

		glm::mat4 projMatrix = glm::mat4(1.0f);
		//fov,aspeat,near,far
		projMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,100.0f);
		
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


		mat4 view = CameraMove(cameraPos, cameraFront, cameraUp);

		shader.SetMatrix("viewMatrix", view);
		shader.SetMatrix("projMatrix", projMatrix);
		shader.SetInt("texture1", 0);
		shader.SetInt("texture2", 1);
		
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = translate(modelMatrix, cubePositions[i]);
			shader.SetMatrix("modelMatrix", modelMatrix);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}


mat4 CameraMove(vec3 &cameraPos, vec3 &cameraFront, vec3 &cameraUp)
{
	float speed = 0.08f;

	if (Input::GetKeyDown(GLFW_KEY_LEFT))
		cameraPos -= speed * normalize(cross(cameraFront, cameraUp));
	if (Input::GetKeyDown(GLFW_KEY_RIGHT))
		cameraPos += speed * normalize(cross(cameraFront, cameraUp));
	if (Input::GetKeyDown(GLFW_KEY_UP))
		cameraPos += speed * cameraFront;
	if (Input::GetKeyDown(GLFW_KEY_DOWN))
		cameraPos -= speed * cameraFront;

	return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}


unsigned int GetTexture(const char *imgPath, int index, GLenum warpMode, GLenum filterMode)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, warpMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "图片加载失败！ Failed to load image！" << endl;
	}
	stbi_image_free(data);

	return texture;
}


