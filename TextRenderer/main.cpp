#include "Utils.h"
#include "Camera.h"
#include "config.h"
#include "Shader.h"
#include "Window.h"
#include "Keyboard.h"


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

MKeyboard keyboard;
MMouse mouse;


// transport matrix 
glm::mat4 view_mat, model_mat;
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
         -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base ・left front
         1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base ・right back
    };

    glGenBuffers(nvbos, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramindPosition), &pyramindPosition, GL_STATIC_DRAW);


}



/* ======================= Loop Function ======================== */

void display(Window* window, float deltime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Use();

    // get value glsl 
    model_mat = glm::translate(glm::mat4(1.0f), cube_pos);
    view_mat = Camera::GetView();
    shader.LoadMatrix(model_mat, view_mat, *Camera::GetProjMatrix());

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

void update(Window * window)
{


}
void process(Window* window , float deltime)
{
   
    if (keyboard.PressedShorcutKey("COMMENT"))
    {
        cout << "Pressed shortcut key Control + K + C" << endl;
    }

    if (keyboard.PressedKey(GLFW_KEY_ESCAPE))
    {
        window->SetClose();
    }

}

/* ======================= Event Function ======================= */

void funcResize(Window* window, int widht, int height)
{
    glViewport(0, 0, widht, height);
    Camera::SetProjMatrix(window->getWidth() , window->getHeight(), angle_view, _near, _far);
}


void funcCurrsorPos(Window* window, double xpos, double ypos)
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

void Setup(Window* window)
{
    cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    shader.LoadShader("Vertex.glsl", "Frag.glsl");
    Camera::setUp(glm::vec3(0.0, 0.0, 8.0));
    Camera::SetProjMatrix(window->getWidth(), window->getHeight(), angle_view, _near, _far);
    keyboard.LoadShortkeyDefine("confkeyboard.ini");
    setupCube();
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
    window.SetUpdateFunc(update);
    window.SetProcessFunc(process);
    window.SetScrollFunc(funcCurrsorPos);
    window.SetResizeFunc(funcResize);

    window.SetKeyboard(&keyboard);
    window.SetMouse(&mouse);


    Setup(&window);
    
    while (!window.IsClosed())
    {
        float current_time = (float)glfwGetTime();
        delta_time = current_time - last_frame;
        last_frame = current_time;
        window.Process(delta_time);
        window.Update();
        window.Draw(delta_time);
    }
    Config::UnLoadLibraries();

    return EXIT_SUCCESS;
}