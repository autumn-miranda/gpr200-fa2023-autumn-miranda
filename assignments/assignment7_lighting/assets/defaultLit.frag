#version 450
out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 color;
};
uniform Light _Light;


struct Material
{
	float ambientK; // ambient coefficient (0-1)
	float diffuseK; //Diffuse Coefficient (0-1)
	float speculat; //Specular coefficient (0-1)
	float shininess;//shininess
};

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	FragColor = texture(_Texture,fs_in.UV);
}