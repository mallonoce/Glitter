#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec4 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // diffuse vector
    vec3 norm = normalize(Normal);

    vec3 lightDir;
    if(light.position.w == 1.0f)
        lightDir = normalize(light.position.xyz - FragPos); // point light
    else // if(light.position.w == 0.0f)
        lightDir = normalize(-light.position.xyz); // directional light, use position vector as direction



    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular vector
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
    FragColor = vec4(ambient + diffuse + specular, 1.0);
} 