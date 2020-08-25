#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>

#define DEFINE_DEFAULT


using namespace std;


/***********************************************************************
 * Định nghĩa các shortCut keys 
 * Bây giờ load từ source 
 * Có thời gian sẽ quay lại sẽ load ShorCut keys từ file ShortcutKey.ini
 * Author: DesertFox    -Date ::22/08/2020
 **********************************************************************/

class Define
{
#ifndef KEYBOARD_H

public:

    static vector<vector<int>> KeyShortcuts;

    static map<const char*, vector<int>*> KeyShortcutNames;

#endif // !KEYBOARD_H



};

