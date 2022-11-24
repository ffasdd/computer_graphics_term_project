#version 330 core

layout(location = 0) in vec3 aPos;   // 위치 변수는 attribute position 0을 가집니다.

uniform mat4 Smatrix;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec3 inColor;

void main()
{
	gl_Position = projectionTransform * viewTransform * Smatrix * vec4(aPos, 1.0);

}