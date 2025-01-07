#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 25) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 projection;
uniform float time;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    vec3 normal = gs_in[index].normal;
    vec3 nextPoint = normal * MAGNITUDE;
    gl_Position = projection * (gl_in[index].gl_Position + vec4(nextPoint, 1.0));
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
