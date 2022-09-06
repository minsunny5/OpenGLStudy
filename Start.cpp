#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"//openGL 3.3버전에 대응
"layout (location = 0) in vec3 aPos;\n"//버텍스 쉐이더 내의 모든 input vertex attributes을 선언
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//버텍스 쉐이더의 Output = gl_Position
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//주황색
"}\n\0";
const char* fragmentShaderSource_yellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"//노란색
"}\n\0";


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
	// build and compile our shader program
	// ------------------------------------
	/* 버텍스 셰이더 오브젝트 만들기 */
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//vertexShader -> 이 ID에 VertexShader를 붙이겠다!
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	//셰이더 코드 붙여주기
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//컴파일 성공했는지 확인
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* 프래그먼트 셰이더 오브젝트 만들기 */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//셰이더 오브젝트 생성
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//오브젝트에 셰이더 코드 붙여주기
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//컴파일 성공했는지 확인
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* 노란색 프래그먼트 셰이더 오브젝트 만들기 */
	unsigned int fragmentShader_yellow;
	fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);//셰이더 오브젝트 생성
	glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);//오브젝트에 셰이더 코드 붙여주기
	glCompileShader(fragmentShader_yellow);
	glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);//컴파일 성공했는지 확인
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	/* 셰이더 링킹 단계 */
	//만약 vertex shader의 output이랑 fragment shader의 input이 안맞으면 링킹 에러가 남.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();//프로그램 오브젝트 생성
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//링크 잘됐나 확인하기
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram_yellow;
	shaderProgram_yellow = glCreateProgram();//프로그램 오브젝트 생성
	glAttachShader(shaderProgram_yellow, vertexShader);
	glAttachShader(shaderProgram_yellow, fragmentShader_yellow);
	glLinkProgram(shaderProgram_yellow);
	//링크 잘됐나 확인하기
	glGetProgramiv(shaderProgram_yellow, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	

	//링크 끝났으면 이제 셰이더 오브젝트들은 필요없으니 해제
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader_yellow);


	//ViewPort setting (1st,2nd) arguments means lower left corner of the window. (-1 to 1) 
	glViewport(0, 0, 800, 600);

	// ------------------------------------------------------------------
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices1[] = {
		// first triangle
		-1.0f, -0.5f, 0.0f,  // left 
		 0.0f, -0.5f, 0.0f,  // right
		-0.5f, 0.5f, 0.0f,  // top 
	};
	float vertices2[] = {
		// second triangle
		 0.0f, -0.5f, 0.0f,  // left
		 1.0f, -0.5f, 0.0f,  // right
		 0.5f, 0.5f, 0.0f   // top 
	};
	
	/* 버텍스 버퍼 오브젝트 만들기 */
	//openGL의 다른 오브젝트들 처럼 VBO도 오브젝트니까 고유 ID를 받아야한다. -> glGenBuffer로 겟
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//첫번째 삼각형 세팅
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//두번째 삼각형 세팅
	glBindVertexArray(VAOs[1]);//glBindVertexArray에 다른걸 바인딩하면 건드릴 VAO대상이 바뀜.
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//Render Loop (루프 한번 = 하나의 프레임)
	while (!glfwWindowShouldClose(window))
	{
		//keyboard input check
		processInput(window);

		//Rendering commands..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//버퍼 리셋할 때는 이 컬러로 바꾼다.(state-setting function)
		glClear(GL_COLOR_BUFFER_BIT);//버퍼를 리셋한다.(state-using function) GL_DEPTH_BUFFER_BIT,GL_STENCIL_BUFFER_BIT도 CLEAR가능

		//삼각형 2개 그리기
		glUseProgram(shaderProgram);//처음에 만든 프로그램 활성화 시키기
		glBindVertexArray(VAOs[0]);//먼저 첫번째 VAO를 그리겠다고 세팅해줌.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glUseProgram(shaderProgram_yellow);//yellow 버전 프로그램 활성화 시키기
		glBindVertexArray(VAOs[1]);//이제 두번째 VAO를 그리겠다고 세팅해줌.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//백버퍼를 모니터에 출력
		glfwPollEvents();//트리거된 이벤트(키보드 인풋이나 마우스 인풋 등)가 있는지 확인
	}

	//메모리 해제
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram_yellow);
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
}