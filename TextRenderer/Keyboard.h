#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Define.h"
#include <vector>
#include <list>
#include <iostream>

#define MAX_KEYBOARD GLFW_KEY_LAST
#define MAX_KEY_FRIN_SHORTCUT 2
#define MAX_KEY_FUNC_PRESSED  2 

#define MAX_KEY_SHORTCUT      3


using namespace std;

class MKeyboard
{
    bool                    m_arbKey[MAX_KEYBOARD] = { false };
    bool                    m_bIsPollEventKey;          // Có sự kiện trong hàng đợi : [TRUE] có , [FLASE] không
    bool                    m_bIsPressedShortKey;

    int                     m_iLastKey;                 // Phím được nhấn cuối cùng

    vector<int>             m_viStackKeyPrin;           // Lưu trữ phím có thể in được ấn
    vector<int>             m_viStackKeyFunc;           // Lưu trữ phím chức năng được ấn 
    vector<int>             m_viStackKeyShor;           // Lưu trữ phím có thể in được ấn phục vụ cho ShortCut
    vector<vector<int>*>    m_viShortcutsContain;       // Chứa danh sách phím tắt phù hợp với tổ hợp phím được ấn

    vector<int>             m_viKeyComplex;             // Giá trị tạm thời lấy tham số chuyền vào của hàm 
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
        m_bIsPressedShortKey = false;
        for (int i = 0; i < MAX_KEYBOARD; i++)
        {
            m_arbKey[i] = false;
        }
    }

    bool LoadShortkeyDefine()
    {
        //?????????????????????????
    }

    bool CheckInShortcutKey(const char* shortKeyName)
    {
    #ifdef DEFINE_DEFAULT
        return Define::KeyShortcutNames[shortKeyName] ? true : false;
    #else
        return false;
    #endif
    }

    vector<int>* GetShortcutKey(const char* shortKeyName)
    {
        return Define::KeyShortcutNames[shortKeyName];
    }


    bool IsChildrenShortcut(vector<int>& srcShortkey , vector<int>& shortkey)
    {
        if (shortkey.empty() || srcShortkey.empty())
        {
            return false;
        }
        size_t minSize = srcShortkey.size() <= shortkey.size() ? srcShortkey.size() : shortkey.size();

        for (auto i = 0; i < minSize; i++)
        {
            if (srcShortkey[i] != shortkey[i])
                return false;
        }

        return true;
    }

    bool IsKeyInShortcut(vector<int> *shortcut , int key, int index)
    {
        if (index >= shortcut->size())
            return false;

        return  (*shortcut)[index] == key;
    }


    
    void SetShortcutKeys(int key , int index)
    {
        if (m_viShortcutsContain.empty())
        {
            for (auto itShortcut : Define::KeyShortcutNames)
            {
                auto ptrShortcut = itShortcut.second;
                if (!ptrShortcut->empty() &&  && (*ptrShortcut)[index] == key)
                {
                    m_viShortcutsContain.push_back(ptrShortcut);
                }
            }
        }
        else
        {
            for (auto itShortcut = m_viShortcutsContain.begin(); itShortcut != m_viShortcutsContain.end();)
            {
                if (IsKeyInShortcut(*itShortcut, key, index) == false)
                {
                    itShortcut = m_viShortcutsContain.erase(itShortcut);
                }
                else
                {
                    itShortcut++;
                }
            }

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
        // Remove keycode in stack key shortcut 
        if (m_viStackKeyFunc.empty() || m_bIsPressedShortKey ||
            m_viStackKeyShor.size() + m_viStackKeyFunc.size() >= MAX_KEY_SHORTCUT)
        {
            m_viStackKeyShor.clear();
            m_bIsPressedShortKey = false;
        }
    }

    /***********************************************************************
    * Kiểm tra phím có trong stack key chức năng có thể in
    * Return:
    *           - [>1 ]  : Chỉ số của key trong stack
    *           - [-1 ]  : Không có trong stack
    * Author: DesertFox    -Date ::21/08/2020
    **********************************************************************/
    int CheckInStackFunc(int keycode)
    {
        int index = 0;
        for (auto key : m_viStackKeyFunc)
        {
            if (key == keycode)
            {
                return index;
            }
            index++;
        }
        return -1;
    }
    /***********************************************************************
    * Kiểm tra phím có trong stack key chức năng có thể in
    * Return:
    * - [>1 ]  : Chỉ số của key trong stack
    * - [-1 ]  : Không có trong stack
    * Author: DesertFox    -Date ::21/08/2020
    **********************************************************************/
    int CheckInStackPrin(int keycode)
    {
        int index = 0;
        for (auto key : m_viStackKeyPrin)
        {
            if (key == keycode)
            {
                return index;
            }
            index++;
        }
        return -1;
    }

    /***********************************************************************
   * Kiểm tra phím có trong stack key chức năng có thể in
   * Return:
   * - [>1 ]  : Chỉ số của key trong stack
   * - [-1 ]  : Không có trong stack
   * Author: DesertFox    -Date ::21/08/2020
   **********************************************************************/
    int CheckInStackShor(int keycode)
    {
        int index = 0;
        for (auto key : m_viStackKeyShor)
        {
            if (key == keycode)
            {
                return index;
            }
            index++;
        }
        return -1;
    }

    void PushStackKey(int keycode, bool state)
    {
        if (state == GLFW_RELEASE)
        {
            RemoveKeyInStack(keycode);
        }
        else if (state == GLFW_PRESS)
        {
            if (IsKeyFunc(keycode) && CheckInStackFunc(keycode) == -1)
            {
                SetShortcutKeys(keycode , m_viStackKeyFunc.size());
                m_viStackKeyFunc.push_back(keycode);
            }

            else if (IsKeyPrin(keycode))
            {
                SetShortcutKeys(keycode, m_viStackKeyFunc.size() + m_viStackKeyShor.size());
                if (m_viStackKeyFunc.size() > 0 && m_viShortcutsContain.empty() &&
                    m_viStackKeyFunc.size() + m_viStackKeyShor.size() < MAX_KEY_SHORTCUT )
                {
                    m_viStackKeyShor.push_back(keycode);
                }
                if (CheckInStackPrin(keycode) == -1)
                {
                    m_viStackKeyPrin.push_back(keycode);
                }
            }
            else
            {
                // Các phím không được xử lý 
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
        if (state == GLFW_PRESS)
        {
            m_iLastKey = keycode;
        }
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
    bool PressedShorcutKey(int key, Args...args)
    {
        m_viKeyComplex.clear();
        this->getArguments(key, args...);

        if (m_iLastKey == m_viKeyComplex.back() && m_bIsPollEventKey)
        {
            int oldIndex = -1;
            for (auto keyCheck : m_viKeyComplex)
            {
                int indexKeyFunc = CheckInStackFunc(keyCheck);
                int indexKeyShor = CheckInStackShor(keyCheck);
                if (indexKeyFunc == -1 && indexKeyShor == -1)
                {
                    return false;
                }
                else
                {
                    int newIndex = (indexKeyFunc != -1) ? indexKeyFunc : (indexKeyShor + m_viStackKeyFunc.size());
                    if (newIndex == oldIndex + 1)
                    {
                        oldIndex = newIndex;
                    }
                    else return false;
                }
            }
            m_bIsPressedShortKey = true;
            return true;
        }
        return false;
    }

    bool PressedShorcutKey(const char* shortKeyName)
    {
        if (!CheckInShortcutKey(shortKeyName)) return false;

        vector<int>* shortKey = GetShortcutKey(shortKeyName);

        if (m_iLastKey == (*shortKey).back() && m_bIsPollEventKey)
        {
            int oldIndex = -1;
            for (auto keyCheck : m_viKeyComplex)
            {
                int indexKeyFunc = CheckInStackFunc(keyCheck);
                int indexKeyShor = CheckInStackShor(keyCheck);
                if (indexKeyFunc == -1 && indexKeyShor == -1)
                {
                    return false;
                }
                else
                {
                    int newIndex = (indexKeyFunc != -1) ? indexKeyFunc : (indexKeyShor + m_viStackKeyFunc.size());
                    if (newIndex == oldIndex + 1)
                    {
                        oldIndex = newIndex;
                    }
                    else return false;
                }
            }
            m_bIsPressedShortKey = true;
            return true;
        }
        return false;
    }


    template<typename... Args>
    bool PressedComplexKey(int key, Args...args)
    {
        m_viKeyComplex.clear();
        this->getArguments(key, args...);

        if (m_iLastKey == m_viKeyComplex.back() && m_bIsPollEventKey)
        {
            int oldIndex = -1;
            for (auto keyCheck : m_viKeyComplex)
            {
                int indexKeyFunc = CheckInStackFunc(keyCheck);
                int indexKeyFrin = CheckInStackPrin(keyCheck);
                if (indexKeyFunc == -1 && indexKeyFrin == -1)
                {
                    return false;
                }
                else
                {
                    int newIndex = (indexKeyFunc != -1) ? indexKeyFunc : (indexKeyFrin + m_viStackKeyFunc.size());
                    if (newIndex == oldIndex + 1)
                    {
                        oldIndex = newIndex;
                    }
                    else return false;
                }
            }

            return true;
        }
        return false;
    }
    void Show()
    {
        cout << std::endl;
        for (auto keyget : m_viStackKeyShor)
        {
            std::cout << keyget << "  ";
        }
        std::cout << std::endl;
    }


};

