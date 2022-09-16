#version 330
out vec4 FragCol;
in vec3 pixColor;
in vec2 pixTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform int val;
uniform float alpha;

void main(){
	if(val % 2 == 0){
		FragCol = mix(texture(texture1, pixTexture), texture(texture2, pixTexture), alpha);	
		return;
	}
	FragCol = mix(texture(texture1, pixTexture), texture(texture3, pixTexture), alpha);		
}