#version 140

in vec3 norm;
in vec3 fragPos;

in vec2 UV;

out vec4 LFragment; 

uniform vec3 lightPos;

uniform sampler2D ourtexture;
void main() { 
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * vec3(1,0,0);
	
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diff * vec3( 1.0,1.0,1.0);

	vec3 result = (ambient + diffuse) * vec3(texture(ourtexture, UV));
	//LFragment = vec4(result, 1.0); 
	LFragment = texture(ourtexture, UV);
}