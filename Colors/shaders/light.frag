#version 330

out vec4 frag_color;

uniform vec3 lightColor;
void main(){
	frag_color = vec4(1.0f - lightColor,1.0f);
}