#ifndef WINDOW_H
#define WINDOW_H

#define  GLFW_EXPOSE_NATIVE_WGL
#define  GLFW_EXPOSE_NATIVE_WIN32


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include <vector>
#include "Config.h"
#include "Keyboard.h"
#include "Mouse.h"




typedef GLFWwindow  WindowBase;


using namespace std;

enum class WindowButton
{
    NONE,
    CANCLE = IDCANCEL,
    TRYAGAIN = IDTRYAGAIN,
    CONTINUE = IDCONTINUE,
    RETRY = IDRETRY,
    OK = IDOK,
    YES = IDYES,
    NO = IDNO
};

enum class WindowDialogState
{
    YESNO,
    RETRYCANCEL,
    INFOR,
    YESNOCANCEL,
    ERR = -22,
};


struct WindowSetting
{
    int                     m_iAntiaLevel;
    bool                    m_bShowCursor;
    bool                    m_bResizeable;
    bool                    m_bAnisotropic;

    WindowSetting()
    {
        m_iAntiaLevel = -1;
        m_bShowCursor = true;
        m_bResizeable = false;
        m_bAnisotropic = false;
    }
    WindowSetting(char* title, unsigned int width, unsigned int height, int antiaLevel = -1,
        bool showCursor = true, bool resizeAble = true, bool uAnisotro = false)
    {
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
    unsigned int            m_uiWidth;
    unsigned int            m_uiHeight;
    float                   m_iOpacity;

    bool                    m_bCreateSuccess;
    bool                    m_bEnable;
    bool                    m_bClose;
    bool                    m_bHide;
    WindowSetting           m_wsSetting;

    float                   m_fDeltaTime;


    GLFWwindow* m_pWindow;

private:
    void (*DrawFunc)       (Window*, float deltime)         = NULL;
    void (*UpdateFunc)     (Window*)                        = NULL;
    void (*ProcessFunc)    (Window* , float deltime)        = NULL;
    void (*KeyboardFunc)   (Window*, int key, int scancode, int action, int mods);
    void (*MouseFunc)      (Window*, int button, int action, int mods);
    void (*ScrollFunc)     (Window*, double xoffset, double yoffset);
    void (*ResizeFunc)     (Window*, int widht, int height);
    void (*CurrsorPosFunc) (Window*, double xpos, double ypos);


private:
    MKeyboard* m_pKeyboard = NULL;
    MMouse* m_pMouse = NULL;

private:
    void SetupFunctionCallback()
    {
        glfwSetKeyCallback(m_pWindow, KeyboardFuncDef);
        glfwSetMouseButtonCallback(m_pWindow, MouseFuncDef);

    }
    void SetupHintsBefore()
    {
        if (m_wsSetting.m_bResizeable == true)
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        }
        if (m_wsSetting.m_iAntiaLevel > 0) glfwWindowHint(GLFW_SAMPLES, m_wsSetting.m_iAntiaLevel);
    }
    void SetupHintsAfter()
    {
        if (m_wsSetting.m_bShowCursor == false) glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (m_wsSetting.m_iAntiaLevel > 0) glEnable(GL_MULTISAMPLE);
    }

private:
    static void DrawDef(GLFWwindow* window)
    {
        Window* winProcess = (Window*)glfwGetWindowUserPointer(window);
        winProcess->DrawFunc(winProcess, 0.0f);
    }
    static void KeyboardFuncDef(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* winProcess = (Window*)glfwGetWindowUserPointer(window);
        winProcess->getKeyboard()->EnableKeyInput();
        if (action == GLFW_PRESS)
        {
            winProcess->m_pKeyboard->SetKey(key, GLFW_PRESS);
        }
        else if (action == GLFW_RELEASE)
        {
            winProcess->m_pKeyboard->SetKey(key, GLFW_RELEASE);
        }
    }

    static void MouseFuncDef(GLFWwindow* window, int button, int action, int mods)
    {
        Window* winProcess = (Window*)glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS)
        {
            winProcess->m_pMouse->SetButton(button, GLFW_PRESS);
        }
        else if (action == GLFW_RELEASE)
        {
            winProcess->m_pMouse->SetButton(button, GLFW_RELEASE);
        }
    }
    static void ResizeFuncDef(GLFWwindow* window, int xpos, int ypos)
    {
        Window* winProcess = (Window*)glfwGetWindowUserPointer(window);
        winProcess->ResizeFunc(winProcess, xpos, ypos);
    }
    static void ScrollFuncDef(GLFWwindow* window, double xpos, double ypos)
    {
        Window* winProcess = (Window*)glfwGetWindowUserPointer(window);
        winProcess->ScrollFunc(winProcess, xpos, ypos);
    }
    static void CursorPosFuncDef(GLFWwindow* window, double xpos, double ypos)
    {
        Window* winProcess = (Window*)glfwGetWindowUserPointer(window);
        winProcess->CurrsorPosFunc(winProcess, xpos, ypos);
    }

public:
    Window(const char* title, unsigned int width, unsigned int height)
    {
        this->m_bClose = false;
        this->m_bHide = false;
        this->m_strTitle = title;
        this->m_uiWidth = width;
        this->m_uiHeight = height;
        this->m_iOpacity = 1.f;

        this->m_bCreateSuccess = false;

        SetupHintsBefore();
        this->m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if (this->m_pWindow)
        {
            this->m_bCreateSuccess = true;
            this->m_hwnd = glfwGetWin32Window(m_pWindow);
            glfwSetWindowUserPointer(m_pWindow, this);
            SetupHintsAfter();
        }
        SetupFunctionCallback();
    }

    Window(const char* title, unsigned int width, unsigned int height, WindowSetting setting)
    {
        this->m_bClose = false;
        this->m_bHide = false;
        this->m_strTitle = title;
        this->m_uiWidth = width;
        this->m_uiHeight = height;
        this->m_iOpacity = 1.f;

        this->m_bCreateSuccess = false;
        this->m_wsSetting = setting;

        SetupHintsBefore();
        this->m_pWindow = glfwCreateWindow(this->m_uiWidth, this->m_uiHeight, m_strTitle, NULL, NULL);;
        if (this->m_pWindow)
        {
            this->m_bCreateSuccess = true;
            this->m_hwnd = glfwGetWin32Window(m_pWindow);
            glfwSetWindowUserPointer(m_pWindow, this);
            SetupHintsAfter();
        }
        SetupFunctionCallback();
    }

    ~Window()
    {
        glfwDestroyWindow(m_pWindow);
    }

    void SetActiveContext()
    {
        glfwMakeContextCurrent(m_pWindow);
        this->Focus();
        if (Config::GetGLEWStatus() == false)
        {
            Config::LoadGLEWLibrary();
            if (Config::GetGLEWStatus() == false)
            {
                exit(EXIT_FAILURE);
            }
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

    void SetDrawFunc(void (*DrawFunc) (Window*, float deltime))
    {
        this->DrawFunc = DrawFunc;
    }
    void SetUpdateFunc(void (*UpdateFunc) (Window*))
    {
        this->UpdateFunc = UpdateFunc;
    }
    void SetProcessFunc(void (*ProcessFunc) (Window*, float deltime))
    {
        this->ProcessFunc = ProcessFunc;
    }
    void SetResizeFunc(void (*ResizeFunc) (Window* window, int width, int height))
    {
        this->ResizeFunc = ResizeFunc;
        glfwSetWindowSizeCallback(m_pWindow, ResizeFuncDef);
    }
    void SetKeyboardFunc(void (*KeyboardFunc)(Window* window, int key, int scancode, int action, int mods))
    {
        this->KeyboardFunc = KeyboardFunc;
        glfwSetKeyCallback(m_pWindow, KeyboardFuncDef);
    }
    void SetMouseFunc(void (*MouseFunc)      (Window* window, int button, int action, int mods))
    {
        this->MouseFunc = MouseFunc;
        glfwSetMouseButtonCallback(m_pWindow, MouseFuncDef);
    }
    void SetScrollFunc(void (*ScrollFunc)     (Window* window, double xoffset, double yoffset))
    {
        this->ScrollFunc = ScrollFunc;
        glfwSetScrollCallback(m_pWindow, ScrollFuncDef);
    }
    void SetCursorPosFunc(void (*CurrsorPosFunc)     (Window* window, double xpos, double ypos))
    {
        this->CurrsorPosFunc = CurrsorPosFunc;
        glfwSetCursorPosCallback(m_pWindow, CursorPosFuncDef);
    }

    void SetTitle(const char* title)
    {
        m_strTitle = title;
        glfwSetWindowTitle(m_pWindow, m_strTitle);
    }
    string GetTitle()
    {
        return m_strTitle;
    }

    void SetSetting(WindowSetting setting)
    {
        this->m_wsSetting = setting;
    }

    GLFWwindow* getGLFW()
    {
        return m_pWindow;
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
        this->m_bEnable = isEnable;
        EnableWindow(m_hwnd, isEnable);
    }

    void SetOpacity(float opacity)
    {
        this->m_iOpacity = opacity;
        glfwSetWindowOpacity(m_pWindow, opacity);
    }

    float GetOpacity()
    {
        return this->m_iOpacity;
    }
    void SetWidth(unsigned int width)
    {
        this->m_uiWidth = width;
        glfwSetWindowSize(m_pWindow, this->m_uiWidth, this->m_uiHeight);
    }
    void SetHeight(unsigned int height)
    {
        this->m_uiHeight = height;
        glfwSetWindowSize(m_pWindow, this->m_uiWidth, this->m_uiHeight);
    }
    unsigned int getWidth()
    {
        return this->m_uiWidth;
    }
    unsigned int getHeight()
    {
        return this->m_uiHeight;
    }

    WindowButton ShowMessage(const wchar_t* title, const wchar_t* msg, WindowDialogState state)
    {

        int result = -1;
        switch (state)
        {
            case WindowDialogState::YESNO:
                result = MessageBox(m_hwnd, msg, title, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);
                break;
            case WindowDialogState::RETRYCANCEL:
                result = MessageBox(m_hwnd, msg, title, MB_ICONASTERISK | MB_RETRYCANCEL | MB_DEFBUTTON1);
                break;
            case WindowDialogState::INFOR:
                result = MessageBox(m_hwnd, msg, title, MB_ICONWARNING | MB_OK | MB_DEFBUTTON1);
                break;
            case WindowDialogState::YESNOCANCEL:
                result = MessageBox(m_hwnd, msg, title, MB_ICONWARNING | MB_YESNOCANCEL | MB_DEFBUTTON1);
                break;
            case WindowDialogState::ERR:
                result = MessageBox(m_hwnd, msg, title, MB_ICONHAND | MB_OK | MB_DEFBUTTON1);
                break;
            default:
                break;
        }
        WindowButton btnClicked;
        switch (result)
        {
            case IDTRYAGAIN:
                btnClicked = WindowButton::TRYAGAIN;
                break;
            case IDRETRY:
                btnClicked = WindowButton::RETRY;
                break;
            case IDCONTINUE:
                btnClicked = WindowButton::CONTINUE;
                break;
            case IDOK:
                btnClicked = WindowButton::OK;
                break;
            case IDYES:
                btnClicked = WindowButton::YES;
                break;
            case IDNO:
                btnClicked = WindowButton::NO;
                break;
            default:
                btnClicked = WindowButton::CANCLE;
        }
        return btnClicked;
    }

    void Process(float deltime)
    {
        this->ProcessFunc(this, deltime);
    }

    void Update()
    {
        this->UpdateFunc(this);
    }

    void Draw(float deltime)
    {
        m_pKeyboard->DisableKeyInput();
        this->DrawFunc(this, deltime);
        this->SwapBuffers();
        glfwPollEvents();
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(m_pWindow);
    }

    int IsClosed()
    {
        return glfwWindowShouldClose(m_pWindow);
    }
    void Close()
    {
        this->m_bClose = true;
        glfwSetWindowShouldClose(m_pWindow, true);
        glfwDestroyWindow(m_pWindow);
    }
    void SetClose(bool isClosed = true)
    {
        this->m_bClose = isClosed;
        glfwSetWindowShouldClose(m_pWindow, true);
    }
    void Focus()
    {
        glfwFocusWindow(m_pWindow);
    }

    void SetKeyboard(MKeyboard* keyboard)
    {
        this->m_pKeyboard = keyboard;
    }

    void SetMouse(MMouse* mouse)
    {
        this->m_pMouse = mouse;
    }


    MKeyboard* getKeyboard()
    {
        return this->m_pKeyboard;
    }
};

#endif // !WINDOW_H
