//UV colors Version
#version 450
out vec4 FragColor;
in vec2 UV;
uniform float _Time = 1.0;
in vec2 fragCoord;
out vec2 uv;
uniform float triangleBrightness;

uniform vec3 colorA;
uniform vec3 colorB;
vec3 result; 

float circleSDF(vec2 p, float r){
    return length(p)-r;
}

void main(){

	
	result = mix(colorA, colorB, UV.y);
	vec2 circlePos = vec2(0.3,sin(_Time));
	float circleRadius = 0.2;
	float d = circleSDF(UV - circlePos,circleRadius);
	d= step(0.0,d);
	result = result *abs(sin(_Time));

	
	float fore = 1.0 - step(abs(sin(UV.x*24.0) * 0.7 ),UV.y);
	result = mix(result,vec3(0.2,0.4,0.0),fore);
	FragColor = vec4(result, triangleBrightness);
}




/*
vec3 color = mix(vec3(1.0,1.0,0.0),vec3(0.9,0.0,0.5),uv.y);
    
//Get 0-1 T value for hill shape
float hills = 1.0 - step(sin(uv.x*6.0) * 0.2 + 0.3,uv.y);
    
//Blend dark grey hills
color = mix(color,vec3(0.2),hills);
*/














/* RGBA Colors version
#version 450
out vec4 FragColor;
in vec4 Color;
uniform float _Time = 1.0;
void main(){
	FragColor = Color * abs(sin(_Time));
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
*/