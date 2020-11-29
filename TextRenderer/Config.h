#pragma once
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 480        

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define DEBUG

//=========================
// Define
//=========================

#ifdef DEBUG
#       define ASSERT(f) _ASSERT(f)
#else 
#       define ASSERT 
#endif // DEBUG

//=========================
//=========================
//=========================
// Define
//=========================

using namespace std;

class Config
{

private:
    static bool m_bGLFWStatus;
    static bool m_bGLEWStatus;

public:
    static bool GetGLFWStatus()
    {
        return m_bGLFWStatus;
    }
    static bool GetGLEWStatus()
    {
        return m_bGLEWStatus;
    }
    static void SetGLFWStatus(bool status)
    {
        m_bGLFWStatus = status;
    }
    static bool SetGLEWStatus(bool status)
    {
        m_bGLEWStatus = status;
    }



    static void LoadLibraries()
    {
        LoadGLFWLibrary();
    }
    static void LoadGLFWLibrary()
    {
        int iInitGLFW = glfwInit();
        ASSERT(iInitGLFW != GLFW_FALSE);
        if (iInitGLFW == GLFW_FALSE)
        {
            m_bGLFWStatus = false;
            cout << "Error [conf_load.cpp]: Load glfwInit() failed !" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        m_bGLFWStatus = true;
    }
    /***********************************************************************
     * Load GLEW thư viện sau khi tạo setActive MakeCurrent OpenGL
     * Author: DesertFox    -Date ::18/08/2020
     **********************************************************************/
    static void LoadGLEWLibrary()
    {
        int iInitGLEW = glewInit();
        ASSERT(iInitGLEW == GLEW_OK);
        if (iInitGLEW != GLEW_OK)
        {
            m_bGLEWStatus = false;
            cout << "Error [conf_load.cpp]: Load glewInit() failed !" << endl;
        }
        m_bGLEWStatus = true;
    }

    static void UnLoadLibraries()
    {
        glfwTerminate();
    }


};

