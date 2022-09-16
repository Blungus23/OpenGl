#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;
in vec3 pos;

uniform sampler2D _MainTexture;
uniform sampler2D smileTexture;
uniform bool swap;

vec2 res = vec2(800,600);


void main()
{
	vec3 col = texture(smileTexture,TexCoord).rgb;
	FragColor = vec4(col,1.0);
}
