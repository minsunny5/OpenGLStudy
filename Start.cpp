#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"//openGL 3.3������ ����
"layout (location = 0) in vec3 aPos;\n"//���ؽ� ���̴� ���� ��� input vertex attributes�� ����
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//���ؽ� ���̴��� Output = gl_Position
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//��Ȳ��
"}\n\0";
const char* fragmentShaderSource_yellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"//�����
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
	//window context�� ���罺������ main context�� ������ �Ѵ�.
	glfwMakeContextCurrent(window);
	//������ âũ�⸦ �ٲ� ������ �� �ݹ��Լ��� �Ҹ��� �ϱ� ���� ���
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
	/* ���ؽ� ���̴� ������Ʈ ����� */
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//vertexShader -> �� ID�� VertexShader�� ���̰ڴ�!
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	//���̴� �ڵ� �ٿ��ֱ�
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//������ �����ߴ��� Ȯ��
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* �����׸�Ʈ ���̴� ������Ʈ ����� */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//���̴� ������Ʈ ����
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//������Ʈ�� ���̴� �ڵ� �ٿ��ֱ�
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//������ �����ߴ��� Ȯ��
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* ����� �����׸�Ʈ ���̴� ������Ʈ ����� */
	unsigned int fragmentShader_yellow;
	fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);//���̴� ������Ʈ ����
	glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);//������Ʈ�� ���̴� �ڵ� �ٿ��ֱ�
	glCompileShader(fragmentShader_yellow);
	glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);//������ �����ߴ��� Ȯ��
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	/* ���̴� ��ŷ �ܰ� */
	//���� vertex shader�� output�̶� fragment shader�� input�� �ȸ����� ��ŷ ������ ��.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();//���α׷� ������Ʈ ����
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//��ũ �ߵƳ� Ȯ���ϱ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram_yellow;
	shaderProgram_yellow = glCreateProgram();//���α׷� ������Ʈ ����
	glAttachShader(shaderProgram_yellow, vertexShader);
	glAttachShader(shaderProgram_yellow, fragmentShader_yellow);
	glLinkProgram(shaderProgram_yellow);
	//��ũ �ߵƳ� Ȯ���ϱ�
	glGetProgramiv(shaderProgram_yellow, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	

	//��ũ �������� ���� ���̴� ������Ʈ���� �ʿ������ ����
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
	
	/* ���ؽ� ���� ������Ʈ ����� */
	//openGL�� �ٸ� ������Ʈ�� ó�� VBO�� ������Ʈ�ϱ� ���� ID�� �޾ƾ��Ѵ�. -> glGenBuffer�� ��
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	//ù��° �ﰢ�� ����
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//�ι�° �ﰢ�� ����
	glBindVertexArray(VAOs[1]);//glBindVertexArray�� �ٸ��� ���ε��ϸ� �ǵ帱 VAO����� �ٲ�.
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//Render Loop (���� �ѹ� = �ϳ��� ������)
	while (!glfwWindowShouldClose(window))
	{
		//keyboard input check
		processInput(window);

		//Rendering commands..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���� ������ ���� �� �÷��� �ٲ۴�.(state-setting function)
		glClear(GL_COLOR_BUFFER_BIT);//���۸� �����Ѵ�.(state-using function) GL_DEPTH_BUFFER_BIT,GL_STENCIL_BUFFER_BIT�� CLEAR����

		//�ﰢ�� 2�� �׸���
		glUseProgram(shaderProgram);//ó���� ���� ���α׷� Ȱ��ȭ ��Ű��
		glBindVertexArray(VAOs[0]);//���� ù��° VAO�� �׸��ڴٰ� ��������.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glUseProgram(shaderProgram_yellow);//yellow ���� ���α׷� Ȱ��ȭ ��Ű��
		glBindVertexArray(VAOs[1]);//���� �ι�° VAO�� �׸��ڴٰ� ��������.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//����۸� ����Ϳ� ���
		glfwPollEvents();//Ʈ���ŵ� �̺�Ʈ(Ű���� ��ǲ�̳� ���콺 ��ǲ ��)�� �ִ��� Ȯ��
	}

	//�޸� ����
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram_yellow);
	//glfw ���ҽ� �޸� ����
	glfwTerminate();


	return 0;
}

//������ âũ�⸦ �ٲ� ������ �Ҹ��� �ݹ��Լ�
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//escŰ�� ���ȴٸ�
	{
		glfwSetWindowShouldClose(window, true);
	}
}