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
	//셰이더 코드 붙여주기
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//컴파일 잘됐나 확인하기
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

	//컴파일 잘됐나 확인하기
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//컴파일 성공했는지 확인
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
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
	

	//이제 셰이더 오브젝트들은 필요없으니 해제
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//ViewPort setting (1st,2nd) arguments means lower left corner of the window. (-1 to 1) 
	glViewport(0, 0, 800, 600);

	// ------------------------------------------------------------------
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	/* 버텍스 버퍼 오브젝트 만들기 */
	//openGL의 다른 오브젝트들 처럼 VBO도 오브젝트니까 고유 ID를 받아야한다. -> glGenBuffer로 겟
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//3.vertex attribute을 설정하자.
	//vertex data를 이렇게 해석해라 하고 알려주는 부분
	//1번 인수:우리가 vertex shader에서 layout (location = 0) 로 aPos라는 변수를 선언했었는데 그 변수에 값을 연결해주는 과정이다.
	//2번 인수:버퍼에 들어있는 값중 3개를 0번 변수에 넣어주라는 뜻
	//3번 인수:정규화 하기 vs 안하기
	//4번 인수:stride 다음 vertex 데이터는 딱 3칸(float * 3) 있다가 나온다고 알려주는 것, tightly pack되어있다면 0넣으면 openGL이 알아서 값을 넣는다.
	//5번 인수: 버퍼에서 얼마만큼 떨어진 상태가 되어야 vertex 데이터가 시작되는지 offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//ㄴ이 함수를 쓰면 자동으로 GL_ARRAY_BUFFER로 바운드 되어있는 VBO에 할당되어있는 메모리에 접근해서 vertex attribute 0번에 vertex data를 연결한다.
	glEnableVertexAttribArray(0);
	//이제 VBO와 Vertex Attribute가 설정되었다고 알려준다.

	//glVertexAttribPointer()함수를 통해서 이미 VBO랑 연결이 되었으니 GL_ARRAY_BUFFER와 바인딩을 풀어놔도 된다.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 다른 VAO call이 이 VAO를 건드릴까봐 unbind 할 수 있지만 거의 안일어나는 일이긴 한다.
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

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

		//사각형 그리기
		glUseProgram(shaderProgram);//만든 프로그램 활성화 시키기
		glBindVertexArray(VAO);//솔직히 지금 그리는 오브젝트가 하나밖에 없는데 이걸 계속 바인딩해줄 필요는 없지만 통일성을 위해 그냥 해준다.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//2번째 인자: indices가 6개라서
		//glBindVertexArray(0); //매번 unbind 해줄 필요는 없다.

		glfwSwapBuffers(window);//백버퍼를 모니터에 출력
		glfwPollEvents();//트리거된 이벤트(키보드 인풋이나 마우스 인풋 등)가 있는지 확인
	}

	//메모리 해제
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
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