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
	float specular; //Specular coefficient (0-1)
	float shininess;//shininess
}_Material;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	//add lighting calculations
	vec3 lightVector = normalize(vec3(
		_Light.position.x-fs_in.WorldPosition.x, 
		_Light.position.y-fs_in.WorldPosition.y,
		_Light.position.z-fs_in.WorldPosition.z));
	_Material.diffuseK = max(dot(normal, lightVector), 0);
	
	FragColor = _Material.diffuseK * texture(_Texture,fs_in.UV);
}