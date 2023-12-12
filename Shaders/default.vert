#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;  
out vec3 Normal;
  
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // TODO: Don't inverse matrices on the GPU! Better to do this on CPU then send it through a uniform like the other 3.
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}