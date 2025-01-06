#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

std::string Shader::loadShaderCode(std::string shaderName, std::string file_extension)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(shadersPath + shaderName + file_extension);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();

        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode   = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << shaderName << file_extension << std::endl;
    }

    return shaderCode.c_str();
}

Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName)
{
    // 1. retrieve the vertex/fragment source code from filePath

    ID = glCreateProgram();

    // vertex shader
    const char* vShaderCode = loadShaderCode(vertexShaderName, ".vert").c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");
    glAttachShader(ID, vertexShader);

    // fragment shader
    const char* fShaderCode = loadShaderCode(fragmentShaderName, ".frag").c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");
    glAttachShader(ID, fragmentShader);

    unsigned int geometryShader = 0;
    if(!std::string(geometryShaderName).empty())
    {
        const char* gShaderCode = loadShaderCode(geometryShaderName, ".geom").c_str();
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &gShaderCode, NULL);
        glCompileShader(geometryShader);
        checkCompileErrors(fragmentShader, "GEOMETRY");
        glAttachShader(ID, geometryShader);
    }

    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete unused shaders, once we already linked and compiled them to shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryShader != 0)
    {
        glDeleteShader(geometryShader);
    }
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3& vector)
{
    setVec3(name, vector.x, vector.y, vector.z);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int  success;
    char infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


