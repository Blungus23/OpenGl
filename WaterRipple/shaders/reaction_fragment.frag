#version 330

out vec4 frag_color;
in vec3 fragCoord;
in vec2 texlCoordinate;

uniform vec2 mousePos;
uniform bool mouseDown;
uniform sampler2D _MainTexture; 
uniform sampler2D pastGeneration; 
uniform int screenWidth;
uniform int screenHeight;
uniform float brushSize;
uniform int pixleSize;

vec2 u_resolution = vec2(screenWidth,screenHeight);
vec2 texel = 1.0 / textureSize(_MainTexture, 0) * pixleSize;
vec2 unitPixel = vec2(1.0,1.0)/vec2(screenWidth,screenHeight);
vec2 pos = fragCoord.xy/vec2(screenWidth,screenHeight);


vec4 getPixle(vec2 uv,sampler2D tex,vec2 texelSize,float x,float y){
	vec4 neig = texture(tex, uv + vec2(x, y)*texelSize).rgba;
	return neig;
};

vec4[5] getAllNeighbors(vec2 uv,sampler2D tex,vec2 texelSize,int offset){
	vec4 neighs[5];
	neighs[0] = getPixle(uv,tex,texelSize,0,-offset); // down
	neighs[1] = getPixle(uv,tex,texelSize,-offset,0); // left
	neighs[2] = getPixle(uv,tex,texelSize,offset,0);  // right
	neighs[3] = getPixle(uv,tex,texelSize,0,offset);  // up
	neighs[4] = getPixle(uv,tex,texelSize,0,0);  // current
	return neighs;
}

float eval(vec2 uv,sampler2D tex,vec2 texelSize,int offset,float current_r){
	vec4 neighs[] = getAllNeighbors(uv,_MainTexture,texel,1);
	float total = 0.0f;
	for(int i =0;i<4;i++){
		total += neighs[i].r;
	}
	total = (total/4);
	float newB = total;
	return newB;
}


vec3 get_Square(vec2 uv,vec2 start, vec2 end, vec3 color){
	vec2 left_bottom = step(start,uv);
	vec2 right_top = step(end,1.0-uv);
	return vec4(color,1.0f) * (left_bottom.x * left_bottom.y * right_top.x * right_top.y);
}



void main(){
	vec2 pos = texlCoordinate;
	vec4 col = texture2D(_MainTexture, pos);
	col.r = eval(pos,_MainTexture,texel,1,col.r);
	float dist = length(fragCoord.xy - (mousePos/pixleSize));
	if(mouseDown && dist < brushSize && mousePos.x > -10){
		col.r = 1.0f;
	}
	frag_color = col;
}




//Code for ripple effect
/*	
	vec2 pos = texlCoordinate;
	vec4 col = texture2D(_MainTexture, pos);
	col.r = eval(pos,pastGeneration,texel,1,col.r);
	float dist = length(fragCoord.xy - (mousePos/pixleSize));
	if(mouseDown && dist < brushSize && mousePos.x > -10){
		col.r = 1.0f;
}*/