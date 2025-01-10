#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/fwd.hpp"

enum class ShaderType
{
    Vertex,
    Fragment,
    Geometry
};

class Shader
{
public:
    // the program ID
    unsigned int ID;
    const std::string shadersPath = "../src/shaders/";
    // constructor reads and builds the shader
    Shader(const char* vertexShaderName, const char* fragmentShaderName, const char* geometryShaderName = "");
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &matrix) const;
    void setVec3(const std::string &name, float x, float y, float z);
    void setVec3(const std::string &name, glm::vec3 &vector);
    void setVec2(const std::string &name, glm::vec2 &vector);

private:
    unsigned int createShader(const std::string& shaderName, ShaderType shaderType);
    void checkCompileErrors(unsigned int shader, std::string type);
};
