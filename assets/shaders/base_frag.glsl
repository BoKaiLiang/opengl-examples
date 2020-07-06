#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D diffuseMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{               
    vec3 normal = normalize(Normal);

    // get diffuse color
    vec3 color = texture(diffuseMap, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = 0.5 * dot(lightDir, normal) + 0.5;
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}