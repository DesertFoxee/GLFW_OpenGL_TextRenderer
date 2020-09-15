#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <string>

#define DEFINE_DEFAULT

#define GETBUI_KEYCODE_FROMNAME(_name)    \
{                                         \
    if (name == #_name)                   \
    {                                     \
        return _name;                     \
    };                                    \
}                                         \


using namespace std;


/***********************************************************************
 * Định nghĩa các shortCut keys 
 * Bây giờ load từ source 
 * Có thời gian sẽ quay lại sẽ load ShorCut keys từ file ShortcutKey.ini
 * Author: DesertFox    -Date ::22/08/2020
 **********************************************************************/

class Define
{
private:
#ifndef KEYBOARD_H

    static int GetKeyCodeFromName(const string name)
    {
        /* Printable keys */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_SPACE              )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_APOSTROPHE         )  /* ' */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_COMMA              )  /* , */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_MINUS              )  /* - */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_PERIOD             )  /* . */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_SLASH              )  /* / */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_0                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_1                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_2                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_3                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_4                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_5                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_6                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_7                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_8                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_9                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_SEMICOLON          ) /* ; */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_EQUAL              ) /* = */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_A                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_B                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_C                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_D                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_E                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_G                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_H                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_I                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_J                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_K                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_L                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_M                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_N                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_O                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_P                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_Q                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_R                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_S                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_T                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_U                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_V                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_W                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_X                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_Y                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_Z                  )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_LEFT_BRACKET       ) /* [ */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_BACKSLASH          ) /* \ */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_RIGHT_BRACKET      ) /* ] */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_GRAVE_ACCENT       ) /* ` */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_WORLD_1            ) /* non-US #1 */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_WORLD_2            ) /* non-US #2 */
        /* Function keys  */
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_ESCAPE             )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_ENTER              )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_TAB                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_BACKSPACE          )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_INSERT             )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_DELETE             )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_RIGHT              )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_LEFT               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_DOWN               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_UP                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_PAGE_UP            )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_PAGE_DOWN          )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_HOME               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_END                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_CAPS_LOCK          )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_SCROLL_LOCK        )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_NUM_LOCK           )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_PRINT_SCREEN       )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_PAUSE              )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F1                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F2                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F3                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F4                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F5                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F6                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F7                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F8                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F9                 )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F10                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F11                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F12                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F13                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F14                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F15                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F16                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F17                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F18                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F19                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F20                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F21                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F22                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F23                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F24                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_F25                )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_0               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_1               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_2               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_3               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_4               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_5               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_6               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_7               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_8               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_9               )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_DECIMAL         )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_DIVIDE          )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_MULTIPLY        )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_SUBTRACT        )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_ADD             )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_ENTER           )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_KP_EQUAL           )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_LEFT_SHIFT         )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_LEFT_CONTROL       )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_LEFT_ALT           )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_LEFT_SUPER         )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_RIGHT_SHIFT        )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_RIGHT_CONTROL      )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_RIGHT_ALT          )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_RIGHT_SUPER        )
        GETBUI_KEYCODE_FROMNAME(GLFW_KEY_MENU               )
        return -1;
    }
public:

    static vector<vector<int>> KeyShortcuts;

    static map<const char*, vector<int>*> KeyShortcutNames;

    static bool PushToKeyShortcut(const char* key, vector <string> keynames);

#endif // !KEYBOARD_H



};

