#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 Color;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);// + vec4(Color.x, Color.y, Color.z, 1.0f);
    //FragColor = vec4(Color, 1.0f);
}