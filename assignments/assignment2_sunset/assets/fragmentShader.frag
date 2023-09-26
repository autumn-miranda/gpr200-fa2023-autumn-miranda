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
	vec2 circlePos = vec2(-1*cos(_Time)*0.3+0.5,sin(_Time));
	float circleRadius = 0.2;
	float d = circleSDF(UV - circlePos,circleRadius);
	d = d;
	d= smoothstep(-0.05,0.05,d);
	result = mix(vec3(1.0f, 0.9f, 0.0f) , result, d);
	result = result *(sin(_Time));
	

	
	float fore = 1.0 - step(abs(sin((UV.x*720)*24.0) * 0.2),UV.y);
	result = mix(result,vec3(0.2,0.4,0.0),fore);
	FragColor = vec4(result, triangleBrightness);
}




/*


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