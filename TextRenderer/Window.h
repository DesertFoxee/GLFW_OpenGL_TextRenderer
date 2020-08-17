#ifndef WINDOW_H
#define WINDOW_H

#define  GLFW_EXPOSE_NATIVE_WGL
#define  GLFW_EXPOSE_NATIVE_WIN32


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include <vector>
#include <Windows.h>




typedef GLFWwindow  WindowBase;


using namespace std;


struct WindowSetting
{
    unsigned int            m_uiScreenW;
    unsigned int            m_uiScreenH;
    int                     m_iAntiaLevel;
    bool                    m_bShowCursor;
    bool                    m_bResizeable;
    bool                    m_bAnisotropic;

    WindowSetting()
    {
        m_uiScreenW = 200;
        m_uiScreenH = 200;
        m_iAntiaLevel = -1;
        m_bShowCursor = true;
        m_bResizeable = false;
        m_bAnisotropic = false;
    }
    WindowSetting(char* title, unsigned int width, unsigned int height, int antiaLevel = -1,
                  bool showCursor = true, bool resizeAble = true, bool uAnisotro = false)
    {
        m_uiScreenW = width;
        m_uiScreenH = height;
        m_iAntiaLevel = antiaLevel;
        m_bShowCursor = showCursor;
        m_bResizeable = resizeAble;
        m_bAnisotropic = uAnisotro;
    }
};


class Window
{
private:
    HWND                    m_hwnd;
    const char* m_strTitle;
    bool                    m_bCreateSuccess;
    bool                    m_bClosed;
    bool                    m_bHide;
    WindowBase* m_pWindow;
    WindowSetting           m_wsSetting;


private:
    void (*DrawFunc)           () = NULL;
    void (*KeyboardFunc)   (int key, int scancode, int action, int mods);
    void (*MouseFunc)      (double xpos, double ypos);
    void (*ScrollFunc)     (double xpos, double ypos);
    void (*ResizeFunc)     (double xpos, double ypos);

private:
    void SetupHintsBefore()
    {
        if (m_wsSetting.m_bResizeable == true) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        if (m_wsSetting.m_iAntiaLevel > 0) glfwWindowHint(GLFW_SAMPLES, m_wsSetting.m_iAntiaLevel);
    }
    void SetupHintsAfter()
    {
        if (m_wsSetting.m_bShowCursor == false) glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (m_wsSetting.m_iAntiaLevel > 0) glEnable(GL_MULTISAMPLE);
    }

public:
    Window(const char* title, unsigned int width, unsigned int height)
    {
        this->m_bClosed = false;
        this->m_bHide = false;
        this->m_strTitle = title;
        this->m_bCreateSuccess = false;
        this->m_wsSetting.m_uiScreenW = width;
        this->m_wsSetting.m_uiScreenH = height;

        SetupHintsBefore();
        this->m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if (this->m_pWindow)
        {
            this->m_bCreateSuccess = true;
            this->m_hwnd = glfwGetWin32Window(m_pWindow);
            SetupHintsAfter();
        }
    }

    Window(const char* title, WindowSetting setting)
    {
        this->m_bClosed = false;
        this->m_bHide = false;
        this->m_strTitle = title;
        this->m_bCreateSuccess = false;
        this->m_wsSetting = setting;

        SetupHintsBefore();
        this->m_pWindow = glfwCreateWindow(setting.m_uiScreenW, setting.m_uiScreenH, m_strTitle, NULL, NULL);;
        if (this->m_pWindow)
        {
            this->m_bCreateSuccess = true;
            this->m_hwnd = glfwGetWin32Window(m_pWindow);
            SetupHintsAfter();
        }
    }

    void SetViewPort(int left, int top, int width, int height)
    {
        glViewport(left, top, width, height);
    }

    void SetPosition(int iLeft, int iRight)
    {
        glfwSetWindowPos(this->m_pWindow, iLeft, iRight);
    }

    void SetDrawFunc(void (*DrawFunc) ())
    {
        this->DrawFunc = DrawFunc;
    }

    void SetKeyboardFunc(void (*KeyboardFunc)(int key, int scancode, int action, int mods))
    {
        this->KeyboardFunc = KeyboardFunc;
    }

    void SetMouseFunc(void (*MouseFunc)      (double xpos, double ypos))
    {
        this->MouseFunc = MouseFunc;
    }

    void SetScrollFunc(void (*ScrollFunc)     (double xpos, double ypos))
    {
        this->ScrollFunc = ScrollFunc;
    }       
  
    void SetScrollFunc(void (*ResizeFunc)     (double xpos, double ypos))
    {
        this->ResizeFunc = ResizeFunc;
        glfwSetWindowSizeCallback(m_pWindow, this->ResizeFunc);
        GLFWwindowsizefun
    }

    void SetTitle(const char* title)
    {
        m_strTitle = title;
        glfwSetWindowTitle(m_pWindow, m_strTitle);
    }

    void SetSetting(WindowSetting setting)
    {
        this->m_wsSetting = setting;
    }

    WindowSetting* GetSetting()
    {
        return &this->m_wsSetting;
    }
    void Hide()
    {
        this->m_bHide = true;
        glfwHideWindow(m_pWindow);
    }
    void Show()
    {
        this->m_bHide = false;
        glfwShowWindow(m_pWindow);
    }
    void Enable(bool isEnable)
    {
        EnableWindow(m_hwnd, isEnable);
    }

    void SetOpacity(float opacity)
    {
        glfwSetWindowOpacity(m_pWindow, opacity);
    }
    int IsClosed()
    {
        return glfwWindowShouldClose(m_pWindow);
    }
    void Close()
    {
        glfwSetWindowShouldClose(m_pWindow, true);
        glfwDestroyWindow(m_pWindow);
    }
    void Focus()
    {
        glfwFocusWindow(m_pWindow);
    }
    void Draw()
    {
        glfwSwapBuffers(m_pWindow);
    }
};

#endif // !WINDOW_H

