#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 scaleMatrix;

out vec3 fragCoord;
out vec2 texlCoordinate;

void main(){
	gl_Position = scaleMatrix * vec4(aPos,1.0f);
	fragCoord = aPos;
	texlCoordinate = aTex;
}	