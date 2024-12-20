#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vertexPosition; // specify a color output to the fragment shader

uniform float offset;

void main()
{
    gl_Position = vec4(aPos.x + offset, aPos.yz, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexPosition = gl_Position; // set the output variable to a dark-red color
}