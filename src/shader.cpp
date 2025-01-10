#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

unsigned int Shader::createShader(const std::string& shaderName, ShaderType shaderType)
{
    unsigned int shaderID;
    std::string shaderCode;
    std::ifstream shaderFile;
    std::string fileExtension;

    switch (shaderType)
    {
        case ShaderType::Fragment:
            fileExtension = ".frag";
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::Vertex:
            fileExtension = ".vert";
            shaderID = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderType::Geometry:
            fileExtension = ".geom";
            shaderID = glCreateShader(GL_GEOMETRY_SHADER);
            break;
    }

    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(shadersPath + shaderName + fileExtension);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();

        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << shaderName << fileExtension << std::endl;
        return 0;
    }

    const char* shaderCodeCstr = shaderCode.c_str();

    glShaderSource(shaderID, 1, &shaderCodeCstr, NULL);
    glCompileShader(shaderID);

    switch (shaderType)
    {
        case ShaderType::Fragment:
            checkCompileErrors(shaderID, "FRAGMENT");
            break;
        case ShaderType::Vertex:
            checkCompileErrors(shaderID, "VERTEX");
            break;
        case ShaderType::Geometry:
            checkCompileErrors(shaderID, "GEOMETRY");
            break;
    }

    glAttachShader(ID, shaderID);
    return shaderID;
}

Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName)
{
    // 1. retrieve the vertex/fragment source code from filePath

    ID = glCreateProgram();

    unsigned int vertexShaderID = createShader(vertexShaderName, ShaderType::Vertex);
    unsigned int fragmentShaderID = createShader(fragmentShaderName, ShaderType::Fragment);

    unsigned int geometryShaderID = 0;
    if(!std::string(geometryShaderName).empty())
    {
        geometryShaderID = createShader(geometryShaderName, ShaderType::Geometry);
    }

    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete unused shaders, once we already linked and compiled them to shader program
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if(geometryShaderID != 0)
    {
        glDeleteShader(geometryShaderID);
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

void Shader::setVec2(const std::string& name, glm::vec2& vector)
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y);
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


