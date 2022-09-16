#version 330
in vec3 pixColor;
in vec2 pixTexture;

out vec4 FragCol;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragCol = mix(texture(texture1,pixTexture),texture(texture2,pixTexture),0.2) * vec4(pixColor,1.0);
}