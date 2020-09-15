#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "Define.h"
#include "Config.h"
#include "Utils.h"

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

    /***********************************************************************
    * Load cấu hình phím tắt từ  file
    *         TRUE  : Nếu load thành công
    *         FALSE : Không thành công
    * Author: DesertFox    -Date ::15/09/2020
    **********************************************************************/
    bool LoadShortkeyDefine(string path)
    {
        ASSERT(path != "");
        if (path == "") return false;
        ifstream file(path);
        if (file.is_open())
        {
            string line;
            while (!file.eof())
            {
                getline(file, line);
                Utils::RemoveExtraSpace(line);
                vector<string> keynames = Utils::Split(line, " ");
                const char* key = keynames.size() >= 2 ? keynames[0].c_str() : "";

                if (key != "")
                {
                    keynames.erase(keynames.begin());
                    Define::PushToKeyShortcut(key, keynames);
                }
            }
            file.close();
        }
        else
        {
            std::cout << "[Error] << Load file shortkey failed !" << endl;
            return false;
        }
        return true;
    }

    bool NameInKeyShortcut(const char* shortKeyName)
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

    /***********************************************************************
     * Kiểm tra tổ hợp phím ấn có thuộc trong 1 phím tắt khác không
     * Return :
     *         TRUE  : Thuộc
     *         FALSE : Không thuộc
     * Author: DesertFox    -Date ::06/09/2020
     **********************************************************************/
    bool IsChildrenShortcut(vector<int>* srcShortkey, vector<int>* shortkey)
    {
        ASSERT(srcShortkey && shortkey);
        if (srcShortkey == NULL || shortkey == NULL) return false;

        if (shortkey->empty() || srcShortkey->empty())
        {
            return false;
        }
        size_t minSize = srcShortkey->size() <= shortkey->size() ? srcShortkey->size() : shortkey->size();

        for (auto i = 0; i < minSize; i++)
        {
            if (srcShortkey[i] != shortkey[i])
                return false;
        }

        return true;
    }

    bool CheckKeyInShortCut(vector<int>* shortkey)
    {
        for (auto itshortkey : Define::KeyShortcutNames)
        {
            if (IsChildrenShortcut(itshortkey.second, shortkey) == true)
            {
                return true;
            }
        }
        return false;
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
    /***********************************************************************
     * Xóa trạng thái phím khi một phím được nhả (released)
     * Author: DesertFox    -Date ::06/09/2020
     **********************************************************************/
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
     * Thiết lập trạng thái phím khi được ấn (pressed)
     * Author: DesertFox    -Date ::06/09/2020
     **********************************************************************/
    void AddKeyInStack(int keycode)
    {
        if (IsKeyFunc(keycode) && CheckInStackFunc(keycode) == -1)
        {
            m_viStackKeyFunc.push_back(keycode);
        }

        else if (IsKeyPrin(keycode))
        {
            m_viStackKeyShor.push_back(keycode);

            vector<int> shortkey;
            shortkey.insert(shortkey.end(), m_viStackKeyFunc.begin(), m_viStackKeyFunc.end());
            shortkey.insert(shortkey.end(), m_viStackKeyShor.begin(), m_viStackKeyShor.end());

            if (m_viStackKeyFunc.size() <= 0 || !CheckKeyInShortCut(&shortkey))
            {
                m_viStackKeyShor.clear();
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
            AddKeyInStack(keycode);
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
    bool GetKeyInputState()
    {
        return m_bIsPollEventKey;
    }
    //========================

    void SetKey(int keycode, bool state)
    {
        m_arbKey[keycode] = state;
        PushStackKey(keycode, state);
        if (state == GLFW_PRESS)
        {
            m_iLastKey = keycode;
        }
    }

    bool GetKey(int keycode)
    {
        return  m_arbKey[keycode];
    }

    bool PressedKey(int keycode)
    {
        return m_arbKey[keycode] && m_bIsPollEventKey && (m_iLastKey == keycode);
    }

    bool PressedShorcutKey(const char* nameShortkey)
    {
        if (!NameInKeyShortcut(nameShortkey)) return false;

        vector<int>* shortKey = GetShortcutKey(nameShortkey);

        if (m_iLastKey == (*shortKey).back() && m_bIsPollEventKey)
        {
            int oldIndex = -1;
            for (auto keyCheck : *shortKey)
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

