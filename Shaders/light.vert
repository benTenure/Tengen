#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float xoffset;
uniform float yoffset;

void main()
{
    vec3 adjPos = vec3(aPos.x * (xoffset * 10), aPos.y * (yoffset * 10), aPos.z);
    gl_Position = projection * view * model * vec4(adjPos, 1.0);
} 