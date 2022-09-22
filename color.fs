#version 330 core
out vec4 FragColor;

in vec3 LightingResult;
  
uniform vec3 objectColor;

void main()
{
    vec3 result = LightingResult * objectColor;
    FragColor = vec4(result, 1.0);
}