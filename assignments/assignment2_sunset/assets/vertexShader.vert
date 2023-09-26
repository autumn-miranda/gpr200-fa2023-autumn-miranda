#version 450 // uv color version
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;
uniform float _Time;
out vec2 fragCoord;
uniform vec2 resolution;

void main(){
	gl_Position = vec4(vPos, 1.0);
	fragCoord = vPos.xy;
	//UV = vUV/resolution.xy;
	//UV = UV*2.0 - 1.0;
	fragCoord.x = fragCoord.x * (resolution.x/resolution.y);
	UV = vUV;
	//UV.x = vUV.x*(1080/720);
}

/* Version with RGBA Color
#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vColor;
out vec4 Color;
uniform float _Time;
void main(){
	Color = vColor;
	vec3 offset = vec3(0,sin(vPos.x + _Time), 0) * 0.5;
	gl_Position = vec4(vPos+offset, 1.0);
}

*/