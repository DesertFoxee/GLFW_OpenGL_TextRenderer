#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#define MAX_KEYBOARD GLFW_KEY_LAST


using namespace std;

class MKeyboard
{
    bool                    m_arbKey[MAX_KEYBOARD] = { false };
    bool                    m_bIsPollEventKey;

    int                     m_iLastKey;

    vector<int>             m_viStackKeyPrin;
    vector<int>             m_viStackKeyFunc;

    vector<int>             m_viKeyComplex;
private:
    void getArguments(int key)
    {
        m_viKeyComplex.push_back(key);
        return;
    }

    template<typename... Args>
    void getArguments(int key, Args... args)
    {
        m_viKeyComplex.push_back(key);
        getArguments(args...);
    }

public:
    MKeyboard()
    {
        m_bIsPollEventKey = false;
        for (int i = 0; i < MAX_KEYBOARD; i++)
        {
            m_arbKey[i] = false;
        }
    }

    bool IsKeyFunc(int keycode)
    {
        bool isKeyFunc = false;
        switch (keycode)
        {
            case GLFW_KEY_LEFT_CONTROL:
            case GLFW_KEY_RIGHT_CONTROL:
            case GLFW_KEY_RIGHT_SHIFT:
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_ALT:
            case GLFW_KEY_LEFT_ALT:
                isKeyFunc = true;
            default:
                break;
        }
        return isKeyFunc;
    }
    /***********************************************************************
     * Check key is printable key
     * Author: DesertFox    -Date ::19/08/2020
     **********************************************************************/
    bool IsKeyPrin(int keycode)
    {
        return (keycode >= 32) && (keycode <= 162);
    }

    void RemoveKeyInStack(int keycode)
    {
        // Remove keycode in stack key normal 
        for (auto itKey = m_viStackKeyFunc.begin(); itKey != m_viStackKeyFunc.end(); )
        {
            if (*itKey == keycode)
            {
                itKey = m_viStackKeyFunc.erase(itKey);

            }
            else ++itKey;
        }
        // Remove keycode in stack key function 
        for (auto itKey = m_viStackKeyPrin.begin(); itKey != m_viStackKeyPrin.end(); )
        {
            if (*itKey == keycode)
            {
                itKey = m_viStackKeyPrin.erase(itKey);
            }
            else ++itKey;
        }
    }

    bool CheckInStackFunc(int keycode)
    {
        for (auto key : m_viStackKeyFunc)
        {
            if (key == keycode)
                return true;
        }
        return false;
    }

    bool CheckInStack(int keycode)
    {
        for (auto key : m_viStackKeyPrin)
        {
            if (key == keycode)
                return true;
        }
        return false;
    }

    void PushStackKey(int keycode, bool state)
    {
        if (state == GLFW_RELEASE)
        {
            RemoveKeyInStack(keycode);
        }
        else if (state == GLFW_PRESS)
        {
            if (IsKeyFunc(keycode) && !CheckInStackFunc(keycode))
            {
                m_viStackKeyFunc.push_back(keycode);
            }
            else if (IsKeyPrin(keycode) && !CheckInStack(keycode))
            {
                m_viStackKeyPrin.push_back(keycode);
            }
        }
    }

    //========================
    void EnableKeyInput()
    {
        m_bIsPollEventKey = true;
    }
    void DisableKeyInput()
    {
        m_bIsPollEventKey = false;
    }
    bool getInputState()
    {
        return m_bIsPollEventKey;
    }
    //========================

    void setKeyRealtime(int keycode, bool state)
    {
        m_arbKey[keycode] = state;
        PushStackKey(keycode, state);
        m_iLastKey = keycode;
    }

    bool getKeyRealtime(int keycode)
    {
        return  m_arbKey[keycode];
    }
    bool PressedKey(int keycode)
    {
        return m_arbKey[keycode] && m_bIsPollEventKey && (m_iLastKey == keycode);
    }

    template<typename... Args>
    bool PressedComplexKey(int key, Args...args)
    {
        m_viKeyComplex.clear();
        this->getArguments(key, args...);

        for (auto keyCheck : m_viKeyComplex)
        {
            if (IsKeyFunc(keyCheck) && !CheckInStackFunc(keyCheck))
            {
                return false;
            }
            else if (IsKeyPrin(keyCheck) && !CheckInStack(keyCheck))
            {
                return false;
            }
            else 
                return false;
        }
        return true;
    }
    void Show()
    {
        cout << std::endl;
        for (auto keyget : m_viKeyComplex)
        {
            std::cout << keyget << "  ";
        }
        std::cout << std::endl;
    }

  
};

