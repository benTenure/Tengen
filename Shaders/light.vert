#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float xoffset;
uniform float yoffset;

void main()
{
    //vec3 adjPos = vec3(aPos.x * xoffset, aPos.y * yoffset, aPos.z);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 