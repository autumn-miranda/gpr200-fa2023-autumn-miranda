#version 450
out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 color;
};
uniform Light _Light;

uniform vec3 cameraPos;
uniform vec3 ambientColor;

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
	vec3 lightVector = normalize(vec3(
		_Light.position.x-fs_in.WorldPosition.x, 
		_Light.position.y-fs_in.WorldPosition.y,
		_Light.position.z-fs_in.WorldPosition.z));
		vec3 cameraVector = normalize(vec3(
		cameraPos.x-fs_in.WorldPosition.x, 
		cameraPos.y-fs_in.WorldPosition.y,
		cameraPos.z-fs_in.WorldPosition.z));
	vec3 normal = normalize(fs_in.WorldNormal);
	//add lighting calculations
	
	_Material.diffuseK = max(dot(normal, lightVector), 0);
	vec3 r = reflect(-1*lightVector, fs_in.WorldNormal);
	_Material.specular = pow(max(dot(r, cameraVector), 0),_Material.shininess);
	vec3 ambient = ambientColor * _Material.ambientK;
	
	vec3 resultColor = (_Light.color * (_Material.diffuseK /*+ _Material.specular*/)) + ambient;
	FragColor = vec4(resultColor,1.0) * texture(_Texture,fs_in.UV);

	//vec4(ambient,1.0f)+(vec4(_Light.color,0.0)*
	//(_Light.color * (_Material.diffuseK + _Material.specular), 1.0f)
}