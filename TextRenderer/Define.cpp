#include "Define.h"

vector<vector<int>> Define::KeyShortcuts;
//{
//    {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K          , GLFW_KEY_C},
//    {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K          , GLFW_KEY_U},
//    {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K          , GLFW_KEY_K}
//};

map<const char*, vector<int>*> Define::KeyShortcutNames;
//{
//    {"COMMENT"           ,&KeyShortcuts[0]},
//    {"UN_COMMENT"        ,&KeyShortcuts[1]},
//    {"BOOK_MARKS"        ,&KeyShortcuts[2]},
//
//};

bool Define::PushToKeyShortcut(const char* key, vector<string> keynames)
{
    vector<int> tempKey;
    for (auto key : keynames)
    {
        int keycode = GetKeyCodeFromName(key);
        if (keycode == -1) return false;
        tempKey.push_back(keycode);
    }
    KeyShortcuts.push_back(tempKey);
    KeyShortcutNames.insert(std::make_pair(key, &KeyShortcuts[KeyShortcuts.size() - 1]));
    return true;
}
