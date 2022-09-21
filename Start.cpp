#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ---------------------------------
// Global
// -----------------------------------
//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;

//Timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	//유저가 커서를 움직일 때마다 이 콜백함수가 불리도록 등록
	glfwSetCursorPosCallback(window, mouse_callback);
	//유저가 스크롤을 움직일 때마다 이 콜백함수가 불리도록 등록
	glfwSetScrollCallback(window, scroll_callback);

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
	Shader objShader("color.vs", "color.fs");
	Shader lightSrcShader("lighting.vs", "lighting.fs");


	//ViewPort setting (1st,2nd) arguments means lower left corner of the window. (-1 to 1) 
	glViewport(0, 0, 800, 600);

	// ------------------------------------------------------------------
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//light VAO, 위의 VAO를 똑같이 써도 되지만 큐브를 많이 수정할 것이라서 따로 만든다.
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Vertex Buffer는 그대로 쓴다.

	//attribute setting
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Render Loop (루프 한번 = 하나의 프레임)
	while (!glfwWindowShouldClose(window))
	{
		//delta time calculation
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//keyboard input check
		processInput(window);

		//Rendering commands..
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//버퍼 리셋할 때는 이 컬러로 바꾼다.(state-setting function)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//depth buffer를 쓸거니까 depth버퍼도 리셋해준다.

		//render
		//cube object Shader setting
		objShader.use(); // don't forget to activate/use the shader before setting uniforms!
		objShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		objShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		objShader.setVec3("lightPos", lightPos);
		objShader.setVec3("viewPos", camera.Position);

		//Transformation
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		objShader.setMat4("view", view);
		objShader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		objShader.setMat4("model", model);

		//Render Cube Obj
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Lighting Src Shader Setting
		lightSrcShader.use();
		lightSrcShader.setMat4("view", view);
		lightSrcShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSrcShader.setMat4("model", model);

		//Render Lighting Src
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		glfwSwapBuffers(window);//백버퍼를 모니터에 출력
		glfwPollEvents();//트리거된 이벤트(키보드 인풋이나 마우스 인풋 등)가 있는지 확인
	}

	//메모리 해제
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
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

//유저가 마우스 위치를 받아오기 위한 콜백함수
//마우스 커서가 움직이면 이 함수가 불린다.
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 마우스가 가리키는 y 좌표가 아래에서 위로 올라갈 때 yoffset이 증가되어야 하는데 원래 스크린 y좌표는 아래에서 위로 갈때 감소하기 때문에 reverse해줌.

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//esc키가 눌렸다면
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)//up키가 눌렸다면
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)//down키가 눌렸다면
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)//오른쪽 방향키가 눌렸다면
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)//왼쪽 방향키가 눌렸다면
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	
}