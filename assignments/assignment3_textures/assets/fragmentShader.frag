#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _BrickTexture;
uniform sampler2D _NoiseTexture;
uniform float _Time;

void main(){
	FragColor = texture(_NoiseTexture, UV);
}