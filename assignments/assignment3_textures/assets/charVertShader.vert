#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _Time;
uniform float _Scale;
out vec2 UV;
void main(){
	UV = vUV/_Scale;
	vec2 position;
	position = vec2(-1*cos(_Time),sin(vPos.x*sin(_Time)));
	if(_Scale>0.5){
	position = position * sin(1/_Scale);
	}
	else{//this extra movement is just so that the character will mostly stay on screen
	position = abs(position * sin(1/_Scale));
	}
	gl_Position = vec4(vPos.xy+position, vPos.z, 1.0);
}