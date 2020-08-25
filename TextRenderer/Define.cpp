#include "Define.h"
vector<vector<int>> Define::KeyShortcuts =
{
    {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K          , GLFW_KEY_C},
    {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K          , GLFW_KEY_U},
    {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_K          , GLFW_KEY_K}
};

map<const char*, vector<int>*> Define::KeyShortcutNames
{
    {"COMMENT"           ,&KeyShortcuts[0]},
    {"UN_COMMENT"        ,&KeyShortcuts[1]},
    {"BOOK_MARKS"        ,&KeyShortcuts[2]},

};