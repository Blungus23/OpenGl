#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 pixTexture;
out vec3 pixColor;
uniform int mult;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
void main(){
	// remember the order of matrix multiplication
	gl_Position = projection * view * model *vec4(aPos,1.0);
	pixTexture = aTexCoord*mult;
	pixColor = aPos;
}