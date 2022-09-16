#version 330

out vec4 frag_color;
in vec3 fragCoord;
in vec2 texlCoordinate;

uniform vec2 mousePos;
uniform bool mouseDown;
uniform sampler2D _MainTexture; 
uniform int screenWidth;
uniform int screenHeight;
uniform float brushSize;
uniform int pixleSize;
uniform bool swap;


const float damping = 0.9975;
vec2 res = vec2(screenWidth,screenHeight);

float buffer1(vec2 uv){
	vec2 tex = texture(_MainTexture,uv).xy;
	if(swap){
		return tex.x;
	}
	return tex.y;
}

float buffer2(vec2 uv){
    vec2 tex = texture(_MainTexture, uv).xy;
    if(swap){
        return tex.y;
    }
    return tex.x;
}


float cubicPulse(float c, float w, float x){
	x = abs(x-c);
	if(x>w) return  0.0;
	x /= w;
	return 1.0 - x*x*(3.0-2.0*x);
}

float hash11(float p){
	p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

vec2 hash22(vec2 p){
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}

float drawDrop(vec2 fc){
	vec2 uv = fc.xy/res.xy;
	vec2 m = (mousePos.xy - 0.5 * res.xy) / res.y;
	if(!mouseDown){
		m = vec2(-2);
	}
	vec2 cv = (fc.xy - 0.5  * res.xy) / res.y;
	float dist = length(fragCoord.xy - (mousePos/pixleSize));
	float mouseShape = cubicPulse(0.0, 0.05, distance(m,cv));
	return mouseShape;
}

void main(){
	vec2 uv  = fragCoord.xy / res.xy;
	vec2 texel = vec2(1.0/res.x,1.0/res.y);
}


/*float newBuffer2 = (buffer1(uv+vec2(+texel.x, 0.)) + 
                        buffer1(uv+vec2(-texel.x, 0.)) + 
                        buffer1(uv+vec2(0., +texel.y)) + 
                        buffer1(uv+vec2(0., -texel.y))) / 2. - buffer2(uv);
    newBuffer2 *= damping;
	newBuffer2 += drawDrop(fragCoord.xy);
	newBuffer2 = clamp(newBuffer2,0.,1.);
	vec2 buffers;
	if(swap){
		buffers = vec2(buffer1(uv),newBuffer2);
	}else{
		buffers = vec2(newBuffer2, buffer1(uv));    
	}
	float offsetX = buffer1(uv+vec2(-texel.x,0.)) - buffer1(uv+vec2(+texel.x,0.));
	float offsetY = buffer1(uv+vec2(0.,-texel.y)) - buffer1(uv+vec2(0.,-texel.y));
	vec2 waveDisplacement = vec2(offsetX,offsetY);
	
	frag_color = vec4(buffers,waveDisplacement);*/