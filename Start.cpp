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
	//���̴� �ڵ� �ٿ��ֱ�
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//������ �ߵƳ� Ȯ���ϱ�
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

	//������ �ߵƳ� Ȯ���ϱ�
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//������ �����ߴ��� Ȯ��
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
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
	

	//���� ���̴� ������Ʈ���� �ʿ������ ����
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
	/* ���ؽ� ���� ������Ʈ ����� */
	//openGL�� �ٸ� ������Ʈ�� ó�� VBO�� ������Ʈ�ϱ� ���� ID�� �޾ƾ��Ѵ�. -> glGenBuffer�� ��
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//3.vertex attribute�� ��������.
	//vertex data�� �̷��� �ؼ��ض� �ϰ� �˷��ִ� �κ�
	//1�� �μ�:�츮�� vertex shader���� layout (location = 0) �� aPos��� ������ �����߾��µ� �� ������ ���� �������ִ� �����̴�.
	//2�� �μ�:���ۿ� ����ִ� ���� 3���� 0�� ������ �־��ֶ�� ��
	//3�� �μ�:����ȭ �ϱ� vs ���ϱ�
	//4�� �μ�:stride ���� vertex �����ʹ� �� 3ĭ(float * 3) �ִٰ� ���´ٰ� �˷��ִ� ��, tightly pack�Ǿ��ִٸ� 0������ openGL�� �˾Ƽ� ���� �ִ´�.
	//5�� �μ�: ���ۿ��� �󸶸�ŭ ������ ���°� �Ǿ�� vertex �����Ͱ� ���۵Ǵ��� offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//���� �Լ��� ���� �ڵ����� GL_ARRAY_BUFFER�� �ٿ�� �Ǿ��ִ� VBO�� �Ҵ�Ǿ��ִ� �޸𸮿� �����ؼ� vertex attribute 0���� vertex data�� �����Ѵ�.
	glEnableVertexAttribArray(0);
	//���� VBO�� Vertex Attribute�� �����Ǿ��ٰ� �˷��ش�.

	//glVertexAttribPointer()�Լ��� ���ؼ� �̹� VBO�� ������ �Ǿ����� GL_ARRAY_BUFFER�� ���ε��� Ǯ����� �ȴ�.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// �ٸ� VAO call�� �� VAO�� �ǵ帱��� unbind �� �� ������ ���� ���Ͼ�� ���̱� �Ѵ�.
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

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

		//�簢�� �׸���
		glUseProgram(shaderProgram);//���� ���α׷� Ȱ��ȭ ��Ű��
		glBindVertexArray(VAO);//������ ���� �׸��� ������Ʈ�� �ϳ��ۿ� ���µ� �̰� ��� ���ε����� �ʿ�� ������ ���ϼ��� ���� �׳� ���ش�.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//2��° ����: indices�� 6����
		//glBindVertexArray(0); //�Ź� unbind ���� �ʿ�� ����.

		glfwSwapBuffers(window);//����۸� ����Ϳ� ���
		glfwPollEvents();//Ʈ���ŵ� �̺�Ʈ(Ű���� ��ǲ�̳� ���콺 ��ǲ ��)�� �ִ��� Ȯ��
	}

	//�޸� ����
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
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