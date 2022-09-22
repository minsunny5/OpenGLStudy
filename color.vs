#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightingResult;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 FragPos = vec3(view * model * vec4(aPos,1.0));
    vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
    vec3 LightPos = vec3(view * vec4(lightPos, 1.0));

    //abmient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular lighting
    float specularStrength = 0.8;
    vec3 viewDir = normalize(-FragPos);//원래 viewPos - FragPos인데 View space라 view pos는 항상 (0,0,0)이다.
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir),0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    LightingResult = ambient + diffuse + specular;

}