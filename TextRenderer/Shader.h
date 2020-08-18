#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>



#define DEBUG

#ifdef DEBUG
#define SHOW_ERROR true
#else
#define SHOW_ERROR false
#endif // DEBUG


using namespace std;

class ShaderHelper
{
public:
    /***********************************************************************
     * Get shader log  and show outstream standard
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    static void ShowLogShader(GLuint shaderID)
    {
        string log = GetLogShader(shaderID);
        stringstream ss(log);
        string item;
        while (getline(ss, item, '\n'))
        {
            std::cout << "\t" << item << endl;
        }
    }
    /***********************************************************************
     * Get program log  and show outstream standard
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    static void ShowLogProgram(GLuint shaderProID)
    {
        string log = GetLogProgram(shaderProID);
        stringstream ss(log);
        string item;
        if (SHOW_ERROR)
        {
            while (getline(ss, item, '\n'))
            {
                std::cout << "\t" << item << endl;
            }
        }
    }
    /***********************************************************************
     * Get all error OpenGL codes
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    static bool checkError()
    {
        GLint error = glGetError();
        if (error == GL_NO_ERROR)
        {
            return false;
        }
        if (SHOW_ERROR)
        {
            std::cout << "[Error] : OpenGL" << endl;
            while (error != GL_NO_ERROR)
            {
                std::cout << "\t" << "- " << error << endl;
                error = glGetError();
            }
        }
        return true;
    }
    /***********************************************************************
     *
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    static GLint IsCompiled(GLuint shaderID)
    {
        GLint isCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
        return isCompiled;
    }

    /***********************************************************************
     * Check for compile error program OpenGL
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    static GLint IsLinked(GLuint shaderProID)
    {
        GLint isLinked = GL_FALSE;
        glGetProgramiv(shaderProID, GL_LINK_STATUS, &isLinked);
        return isLinked;
    }

    /***********************************************************************
     * Get shader error log information
     * Author: DesertFox    -Date ::17/08/2020
     * Return :
     **********************************************************************/
    static string GetLogShader(GLuint shaderID)
    {
        string strLog = "";
        GLint logsLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logsLength);

        if (logsLength > 0)
        {
            GLchar* logs = new GLchar[logsLength];
            glGetShaderInfoLog(shaderID, logsLength, &logsLength, logs);
            strLog = std::string(logs);
            delete[] logs;
        }
        return strLog;
    }

    /***********************************************************************
     *
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    static string GetLogProgram(GLuint shaderProID)
    {
        string strLog = "";
        GLint logsLength = 0;
        glGetProgramiv(shaderProID, GL_INFO_LOG_LENGTH, &logsLength);

        if (logsLength > 0)
        {
            GLchar* logs = new GLchar[logsLength];
            glGetProgramInfoLog(shaderProID, logsLength, &logsLength, logs);
            strLog = std::string(logs);
            delete[] logs;
        }
        return strLog;
    }
    static string LoadSourceFile(const char* srcFile)
    {
        string content = "";
        if (!srcFile) return "";
        ifstream file(srcFile);
        if (file.is_open())
        {
            while (!file.eof())
            {
                string line;
                getline(file, line);
                content.append(line + "\n");
            }
            file.close();
        }
        return content;
    }

};

class Shader
{
private:
    GLuint          m_vertexShader = 0;
    GLuint          m_geomShader = 0;
    GLuint          m_fragmentShader = 0;
    GLuint          m_program = 0;


public:
    /***********************************************************************
     *
     * Author: DesertFox    -Date ::17/08/2020
     **********************************************************************/
    void LoadShader(const char* vertSrc, const char* fragSrc, const char* geomSrc = NULL)
    {
        string strSrcVert = "", strSrcFrag = "", strSrcGeom = "";
        string errLoadLogs = "";
        try
        {
            strSrcVert = ShaderHelper::LoadSourceFile(vertSrc);
            strSrcFrag = ShaderHelper::LoadSourceFile(fragSrc);
            strSrcGeom = ShaderHelper::LoadSourceFile(geomSrc);

            if (strSrcVert.empty())             errLoadLogs += "\tLoad source failed : " + string(vertSrc) + "\n";
            if (strSrcFrag.empty())             errLoadLogs += "\tLoad source failed : " + string(fragSrc) + "\n";
            if (geomSrc && strSrcGeom.empty())  errLoadLogs += "\tLoad source failed : " + string(geomSrc) + "\n";

            if (!errLoadLogs.empty()) throw errLoadLogs;
        }
        catch (std::string ex)
        {
            std::cout << "[Error]::Load file source failed !" << endl;
            std::cout << ex << endl;
            return;
        }
        const GLchar* cSrcVert = strSrcVert.c_str();
        const GLchar* cSrcFrag = strSrcFrag.c_str();

        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vertexShader, 1, &cSrcVert, 0);
        glCompileShader(m_vertexShader);
        ShaderHelper::checkError();
        if (!ShaderHelper::IsCompiled(m_vertexShader))
        {
            std::cout << "[ Log ]:: VertexShader logs :" << endl;
            ShaderHelper::ShowLogShader(m_vertexShader);
            return;
        }

        if (geomSrc)
        {
            m_geomShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(m_geomShader, 1, &cSrcFrag, 0);
            glCompileShader(m_geomShader);
            ShaderHelper::checkError();
            if (!ShaderHelper::IsCompiled(m_geomShader))
            {
                std::cout << "[ Log ]:: GeomatryShader logs :" << endl;
                ShaderHelper::ShowLogShader(m_geomShader);
                return;
            }
        }

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragmentShader, 1, &cSrcFrag, 0);
        glCompileShader(m_fragmentShader);
        ShaderHelper::checkError();
        if (!ShaderHelper::IsCompiled(m_fragmentShader))
        {
            std::cout << "[ Log ]:: FragmentShader logs :" << endl;
            ShaderHelper::ShowLogShader(m_fragmentShader);
            return;
        }

        m_program = glCreateProgram();
        glAttachShader(m_program, m_vertexShader);
        if (geomSrc)  (m_program, m_geomShader);
        glAttachShader(m_program, m_fragmentShader);

        glLinkProgram(m_program);
        ShaderHelper::checkError();
        if (!ShaderHelper::IsLinked(m_program))
        {
            std::cout << "[ Log ] Linker logs :" << endl;
            ShaderHelper::ShowLogProgram(m_program);
            this->Clear();
        }
    }

    void LoadMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
    {
        glUseProgram(m_program);
        GLuint unifViewModel  = glGetUniformLocation(m_program, "vmMatrix");
        GLuint unifProj  = glGetUniformLocation(m_program, "projMatrix");

        glUniformMatrix4fv(unifViewModel, 1, GL_FALSE, glm::value_ptr(view * model));
        glUniformMatrix4fv(unifProj , 1, GL_FALSE, glm::value_ptr(proj));
    }

    void Clear()
    {
        glDetachShader(m_program, m_vertexShader);
        glDetachShader(m_program, m_fragmentShader);
    }

    void Use()
    {
        glUseProgram(m_program);
    }

    void DontUse()
    {
        glUseProgram(0);
    }

    void Delete()
    {
        this->Clear();
        glDeleteProgram(m_program);
    }
};

