#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;//view space로 변환된 광원의 위치벡터
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //abmient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);//원래 viewPos - FragPos인데 View space라 view pos는 항상 (0,0,0)이다.
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir),0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}