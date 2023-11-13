#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	vs_out.UV = vUV;
	//calcuate vs_out.WorldPosition
	vs_out.WorldPosition = (_Model * vec4(vPos, 1.0f)).xyz;//.xyz is called swizzling
	//calculate vs_out WorldNormal
	vs_out.WorldNormal = normalize(_Model * vec4(vNormal, 0.0f)).xyz;
	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}