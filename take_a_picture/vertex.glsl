#version 330 core
layout(location=0)in vec3 vPos;
layout(location=1)in vec3 vNormal;
uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;
out vec3 FragPos;
out vec3 Normal;
void main()
{ 
	gl_Position =  projectionTransform*viewTransform *modelTransform*vec4(vPos,1.0);
	FragPos=vec3(modelTransform*vec4(vPos,1.0));
	Normal = vec3(modelTransform * vec4(vNormal, 1.0));
}
