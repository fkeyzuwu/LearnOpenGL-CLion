#version 330 core
out vec4 FragColor;

#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}