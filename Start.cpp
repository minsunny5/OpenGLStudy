#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//global
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -0.3f);

int main()
{
	//Initialize glfw
	glfwInit();

	//configure glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//window context를 현재스레드의 main context로 만들라고 한다.
	glfwMakeContextCurrent(window);
	//유저가 창크기를 바꿀 때마다 이 콜백함수가 불리게 하기 위해 등록
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize glad" << endl;
		return -1;
	}


	// ------------------------------------
	//depth testing on
	// ------------------------------------
	glEnable(GL_DEPTH_TEST);


	// ------------------------------------
	// build and compile our shader program
	// ------------------------------------
	Shader ourShader("transform.vs", "texture.fs");


	//ViewPort setting (1st,2nd) arguments means lower left corner of the window. (-1 to 1) 
	glViewport(0, 0, 800, 600);

	// ------------------------------------------------------------------
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
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

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//0레벨 텍스쳐로 만들기
		glGenerateMipmap(GL_TEXTURE_2D);//0레벨 텍스쳐 기반으로 밉맵들 자동 생성하기
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

	//Cube 들의 위치
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

	//Render Loop (루프 한번 = 하나의 프레임)
	while (!glfwWindowShouldClose(window))
	{
		//keyboard input check
		processInput(window);

		//Rendering commands..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//버퍼 리셋할 때는 이 컬러로 바꾼다.(state-setting function)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//depth buffer를 쓸거니까 depth버퍼도 리셋해준다.

		// fragment shader의 샘플러2d변수로 texture를 보내준다.
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//render
		ourShader.use();

		//Transformation
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, cameraPos);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);//먼저 첫번째 VAO를 그리겠다고 세팅해줌.
		//Cube 10개를 그려보자.
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);//먼저 초기화
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glfwSwapBuffers(window);//백버퍼를 모니터에 출력
		glfwPollEvents();//트리거된 이벤트(키보드 인풋이나 마우스 인풋 등)가 있는지 확인
	}

	//메모리 해제
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glfw 리소스 메모리 해제
	glfwTerminate();


	return 0;
}

//유저가 창크기를 바꿀 때마다 불리는 콜백함수
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//esc키가 눌렸다면
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)//up키가 눌렸다면
	{
		cameraPos += glm::vec3(0.0f, 0.0f, 0.3f);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)//down키가 눌렸다면
	{
		cameraPos += glm::vec3(0.0f, 0.0f, -0.3f);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)//오른쪽 방향키가 눌렸다면
	{
		cameraPos += glm::vec3(-0.3f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)//왼쪽 방향키가 눌렸다면
	{
		cameraPos += glm::vec3(0.3f, 0.0f, 0.0f);
	}
	
}