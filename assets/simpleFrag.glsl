#version 420

uniform vec3 objectColor;
uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform int gamma;

in vec2 texCoords0;
vec4 gammaColor;
vec4 combinedColor;

in vec3 normal0;
in vec3 FragPos;
float specularStrength = 6.0f;


void main()
{
	
   
	vec3 gammaTex = pow(texture2D(diffuse, texCoords0).rgb, vec3(2.2));
	vec4 gammaColor = vec4(gammaTex, 1.0f);
	
	vec3 ambient = 0.7 * lightColor;

	vec3 norm = normalize(normal0);
	vec3 lightDir = normalize(lightPos - FragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);


	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 tempColor = (ambient + diffuse + specular) * gammaColor.rgb;
	combinedColor = vec4(tempColor, 1.0);

	vec4 fragColor = combinedColor + vec4(objectColor, 1.0);

		
	if (gamma == 1)
	{
		float gamma = 2.2;
		fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
	} 

	gl_FragColor = clamp(fragColor, 0.0f, 1.0f); 
}