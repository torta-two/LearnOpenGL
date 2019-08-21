#include "MyTools.h"
#include "MyShader.h"
#include "MyInput.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* vertexPath = "shader.vertex";
const char* fragmentPath = "shader.fragment";

const unsigned int SCREEN_WIDTH = 512;
const unsigned int SCREEN_HEIGHT = 512;

unsigned int GetTexture(const char *imgPath, int index, GLenum warpMode, GLenum filterMode);

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

unsigned int indices[] =
{
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
};


int main()
{
	GLFWwindow* window = InitOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);
	Shader shader(vertexPath, fragmentPath);

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));		//color
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));		//texcoord	
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	unsigned int texture1 = GetTexture("wall.jpg", 0, GL_REPEAT, GL_LINEAR);
	unsigned int texture2 = GetTexture("face.jpg", 1, GL_REPEAT, GL_LINEAR);

	float x = 0, y = 0, z = -3.0;
	float speed = 0.08f;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Input::GetKeyDown(GLFW_KEY_LEFT))
		{
			x += speed;
		}

		if (Input::GetKeyDown(GLFW_KEY_RIGHT))
		{
			x -= speed;
		}

		if (Input::GetKeyDown(GLFW_KEY_UP))
		{
			y -= speed;
		}

		if (Input::GetKeyDown(GLFW_KEY_DOWN))
		{
			y += speed;
		}

		if (Input::GetKeyDown(GLFW_KEY_W))
		{
			z += speed;
		}

		if (Input::GetKeyDown(GLFW_KEY_S))
		{
			z -= speed;
		}
		//------------------------------------------------------------------

		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		//矩阵的计算顺序与阅读顺序相反，vertex一定要在transform的右侧！
		//先平移再旋转，旋转轴为自身中心
		//先旋转再平移，旋转轴为窗口中心
		//因为平移不会移动旋转的中心轴
		shader.UseShader();
		
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projMatrix = glm::mat4(1.0f);
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::translate(viewMatrix, glm::vec3(x, y, z));
		//fov,aspeat,near,far
		projMatrix = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,100.0f);
		
		//shader.SetMatrix("modelMatrix", modelMatrix);
		shader.SetMatrix("viewMatrix", viewMatrix);
		shader.SetMatrix("projMatrix", projMatrix);
		shader.SetInt("texture1", 0);
		shader.SetInt("texture2", 1);
		
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));

			if (i % 3 == 0)
			{
				modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f * (float)glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
			}

			shader.SetMatrix("modelMatrix", modelMatrix);
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


