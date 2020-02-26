#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texture;

uniform int numOfInstances;

uniform mat4 transforms[200];

void main()
{
    gl_Position = transforms[gl_InstanceID] * vec4(aPos, 1.0);
}