#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord),texture(texture2, TexCoord),0.2); //텍스쳐 coord와 텍스쳐를 받아와서 색을 샘플링한다
}