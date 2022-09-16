#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;
uniform int size;
out vec2 TexCoord;
out vec3 pos;
void main(){
  pos = vec3(aPos.xy * size,0.0f);
  gl_Position = vec4(pos,1.0f);
  TexCoord = aTex;
}