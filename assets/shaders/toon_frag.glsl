#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 specColor;
uniform vec3 viewPos;
uniform float shininess;

void main()
{
	// ambient
	vec3 ambient = vec3(0.4,0.4,0.4) * lightColor * objectColor;

	// diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffIntensity = max(dot(norm, lightDir), 0.0);
	diffIntensity = step(0.1, diffIntensity);
	vec3 diffuse = diffIntensity * lightColor * objectColor;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = clamp(dot(norm, halfwayDir), 0.0, 1.0);
	float specIntensity = pow(spec * diffIntensity, shininess * shininess);
	float specIntensitySmooth = smoothstep(0.05, 0.1, specIntensity);
	vec3 specular = specIntensitySmooth * objectColor * specColor * lightColor;


	FragColor = vec4(ambient + diffuse + specular, 1.0);
}