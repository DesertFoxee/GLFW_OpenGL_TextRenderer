#ifndef UTILS_H
#define UTILS_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Config.h"

#define PI 3.1415


using namespace std;


class Utils
{
public:
    static float cvDegreeToRadius(float degree)
    {
        return (degree * float(PI / 180));
    }
    static float cvRadiusToDegree(float radius)
    {
        return (radius * float(180 / PI));
    }
    static float map(float value, float smin, float smax, float dmin, float dmax)
    {
        if (value <= smin) return dmin;
        if (value >= smax) return dmax;
        float a1 = value - smin;
        float a2 = dmax - dmin;
        float b1 = smax - smin;
        return dmin + (a1 * a2) / b1;
    }
    static vector<string> Split(string strOrg, const string delimiter)
    {
        vector<string> strSplit;
        size_t pos = 0;
        std::string token;
        while ((pos = strOrg.find(delimiter)) != std::string::npos)
        {
            token = strOrg.substr(0, pos);
            strSplit.push_back(token);
            strOrg.erase(0, pos + delimiter.length());
        }
        if(!strOrg.empty()) strSplit.push_back(strOrg);
        return strSplit;
    }
    static void RemoveSpace(string& str)
    {
        for (size_t i = 0; i < str.length();)
        {
            if (str[i] == ' ')
            {
                str.erase(i, 1);
            }
            else { i++; }
        }
    }
    static void RemoveExtraSpace(string& str)
    {

        while (str.length() > 0 && str[0] == ' ')
        {
            str = str.erase(0, 1);
        }
        if (str.length() == 0) return;
        for (size_t i = 0; i < str.length();)
        {
            if (str[i] == ' ' && str[i + 1] == ' ')
            {
                str.erase(i + 1, 1);
            }
            else { i++; }
        }
        if (str.length() - 1 > 0 && str[str.length() - 1] == ' ')  str.pop_back();
    }
    static void RemoveExtraTab(string& str)
    {

        while (str.length() > 0 && str[0] == '\t')
        {
            str = str.erase(0, 1);
        }
        if (str.length() == 0) return;
        for (size_t i = 0; i < str.length();)
        {
            if (str[i] == '\t' && str[i + 1] == '\t')
            {
                str.erase(i + 1, 1);
            }
            else { i++; }
        }
        if (str.length() - 1 > 0 && str[str.length() - 1] == '\t')  str.pop_back();
    }
    /* ====================== <Test > =================== */
    static void showVec3(glm::vec3 vec)
    {
        cout << vec.x << "," << vec.y << "," << vec.z << endl;
    }
};
#endif  // !UTILS_H