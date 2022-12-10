#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D outTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

bool lighton=true;
float light_p=0.1;

void main(){
	float ambientLight =light_p;
	vec3 ambient =ambientLight*lightColor;
	vec3 normalVector=normalize(Normal);
	vec3 lightDir=normalize(lightPos - FragPos);
	float diffuseLight=max(dot(normalVector,lightDir),0.0);
	vec3 diffuse = diffuseLight*lightColor;

	//vec3 vTexCol = texture(outTexture, TexCoord).rgb;

	vec3 result=(ambient+diffuse);
	FragColor = vec4(result,1.0);
	FragColor=texture(outTexture,TexCoord)*FragColor;
	
}