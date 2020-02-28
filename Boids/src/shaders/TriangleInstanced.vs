#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texture;
layout (location = 2) in mat4 instanceMatrix;

void main()
{
    gl_Position = instanceMatrix * vec4(aPos, 1.0);
}