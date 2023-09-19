#version 450
out vec4 FragColor;
in vec4 Color;
uniform float _Time = 1.0;
void main(){
	FragColor = Color * abs(sin(_Time));
}