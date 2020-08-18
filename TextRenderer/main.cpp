#include "Utils.h"
#include "Camera.h"
#include "config.h"
#include "Shader.h"
#include "Window.h"


#define nvbos 2


/* ======================= Data Function ======================= */
int width;
int height;


bool first_mouse = false;

float delta_time = 0.0f;
float last_frame = 0.0f;

GLuint program;

GLuint vbo[nvbos];


// object 
glm::vec3 cube_pos;
glm::vec3 pyramid_pos;


// transport matrix 
glm::mat4 proj_mat, view_mat, model_mat;
glm::mat4 r_mat;

// Shader;
Shader shader;

// view
float angle_view = 60, _near = 0.1f, _far = 1000.f;

// mouse 
int m_last_x = SCREEN_WIDTH / 2, m_last_y = SCREEN_HEIGHT / 2;


/* ======================= Setup Function ======================= */


void setupCube()
{
	float vertexPositions[108] = {
	-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};


	float pyramindPosition[] = {
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
		 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
		 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
		 -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base Eleft front
		 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base Eright back
	};

	glGenBuffers(nvbos, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramindPosition), &pyramindPosition, GL_STATIC_DRAW);


}

void Setup(Window * window)
{
	cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	proj_mat = Utils::getMatrixProj(window, angle_view, _near, _far);
	program = Utils::createShaderProgram("Vertex.glsl", "Frag.glsl");
	shader.LoadShader("Vertex.glsl", "Frag.glsl");
	Camera::setUp(glm::vec3(0.0, 0.0, 8.0));
	setupCube();
}


/* ======================= Loop Function ======================== */

void display(Window * window )
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader.Use();

	// get value glsl 
	model_mat = glm::translate(glm::mat4(1.0f), cube_pos);
	view_mat = Camera::getView();
	shader.LoadMatrix(model_mat, view_mat, proj_mat);

	// Draw Cube 
	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//shader.DontUse();
}
void update()
{


}
void process(Window * window)
{
	if (glfwGetKey(window->getGLFW(), GLFW_KEY_A) == GLFW_PRESS)
	{
		Camera::moveLeft(5.f * delta_time);
	}
	if (glfwGetKey(window->getGLFW(), GLFW_KEY_S) == GLFW_PRESS)
	{
		Camera::moveDown(5.f * delta_time);
	}
	if (glfwGetKey(window->getGLFW(), GLFW_KEY_D) == GLFW_PRESS)
	{
		Camera::moveRight(5.f * delta_time);
	}
	if (glfwGetKey(window->getGLFW(), GLFW_KEY_W) == GLFW_PRESS)
	{
		Camera::moveUp(5.f * delta_time);
	}
}

/* ======================= Event Function ======================= */

void funcResize(Window* window, int widht, int height)
{
	glViewport(0, 0, widht, height);
	proj_mat = Utils::getMatrixProj(window, angle_view, _near, _far);
}

void funcKeyboard(Window* window , int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		window->setClose();
	}
}
void funcMouse(Window* window, int button, int action, int mods)
{

}

void funcCurrsorPos(Window* window,double xpos, double ypos)
{

	/*float xoffset = xpos - SCREEN_WIDTH / 2;
	float yoffset = SCREEN_HEIGHT / 2 - ypos;

	const float sensitivity = 10.f * delta_time;
	xoffset *= sensitivity;
	yoffset *= sensitivity;



	m_last_x = xpos;
	m_last_y = ypos;
	Camera::rotate(xoffset, yoffset);
	glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);*/
}
void funcCu(GLFWwindow* window, double xpos, double ypos)
{
}



int main()
{
	Config::LoadGLFWLibrary();
	WindowSetting setting;
	setting.m_iAntiaLevel = 8;
	setting.m_bResizeable = false;

	Window window("OpenGL", SCREEN_WIDTH, SCREEN_HEIGHT, setting);
	window.SetActiveContext();
	window.SetDrawFunc(display);
	window.SetKeyboardFunc(funcKeyboard);
	window.SetMouseFunc(funcMouse);
	window.SetScrollFunc(funcCurrsorPos);
	window.SetResizeFunc(funcResize);
	

	Setup(&window);
	while (!window.IsClosed())
	{
		float current_time = glfwGetTime();
		delta_time = current_time - last_frame;
		last_frame = current_time;
		process(&window);
		update();
		window.Draw();
	}
	Config::UnLoadLibraries();
	return EXIT_SUCCESS;
}