#version 450
out vec4 FragColor;
in vec2 UV;
uniform float _Time;
uniform sampler2D _charTexture;

void main(){
	FragColor = texture(_charTexture, UV);
}