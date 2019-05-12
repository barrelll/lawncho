#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 vertexUVcoords;
//layout(location = 2) in vec3 vertexNormalcoords;

out vec2 UV;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(pos, 1);
    UV = vertexUVcoords;
}
