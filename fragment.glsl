#version 330 core
in vec4 vertexPosition;
out vec4 FragColor;
uniform vec4 ourColor;

void main()
{
    FragColor = vertexPosition;
}