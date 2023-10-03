#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _BrickTexture;
uniform sampler2D _NoiseTexture;
uniform float _Tile;
uniform float _Time;

void main(){
	vec4 bricks = texture(_BrickTexture, UV/_Tile);
	vec4 other = texture(_NoiseTexture, UV/sin(_Time)*2);
	FragColor = mix(bricks, other, sin(_Time));
	//FragColor = vec4(bricks);
}