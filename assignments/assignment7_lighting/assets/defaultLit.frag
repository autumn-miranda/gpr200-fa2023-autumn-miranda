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

uniform bool BP;

uniform int numLights;

struct Material
{
	float ambientK; // ambient coefficient (0-1)
	float diffuseK; //Diffuse Coefficient (0-1)
	float specularK; //Specular coefficient (0-1)
	float shininess;//shininess
};
uniform Material _Material;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

uniform sampler2D _Texture;

void main(){
	vec3 lightVector = normalize(vec3(_Light.position - fs_in.WorldPosition));
	vec3 cameraVector = normalize(vec3(cameraPos - fs_in.WorldPosition));
	vec3 normal = normalize(fs_in.WorldNormal);
	//add lighting calculations
	
	float diff = max(dot(normal, lightVector), 0);
	vec3 r;
	float spec;
	if(BP){ 
		r = normalize(lightVector + cameraVector);
		spec = pow(max(dot(fs_in.WorldNormal, r), 0), _Material.shininess);
	}
	else{ 
		r = reflect(-1*lightVector, fs_in.WorldNormal);
		spec = pow(max(dot(cameraVector, r), 0), _Material.shininess);	
	}
	vec3 ambient = ambientColor * _Material.ambientK;

	vec3 diffuse = _Light.color * diff * _Material.diffuseK;
	vec3 specular = _Light.color * spec * _Material.specularK;
	
	vec3 resultColor = diffuse + specular + ambient;
	FragColor = vec4(resultColor,1.0) * texture(_Texture,fs_in.UV);

	//vec4(ambient,1.0f)+(vec4(_Light.color,0.0)*
	//(_Light.color * (_Material.diffuseK + _Material.specular), 1.0f)
}