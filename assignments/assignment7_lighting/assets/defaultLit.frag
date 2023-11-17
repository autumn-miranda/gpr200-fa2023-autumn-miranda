#version 450
out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 3
uniform Light _Light[MAX_LIGHTS];


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
	vec3 results[MAX_LIGHTS];
	vec3 resultColor;
	vec3 ambient = ambientColor * _Material.ambientK;
	for(int i = 0; i<numLights; i++){
	vec3 lightVector = normalize(vec3(_Light[i].position - fs_in.WorldPosition));
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
	

	vec3 diffuse = _Light[i].color * diff * _Material.diffuseK;
	vec3 specular = _Light[i].color * spec * _Material.specularK;
	
	results[i] = diffuse + specular;
	resultColor += results[i];
	}
	resultColor += ambient;
	FragColor = vec4(resultColor,1.0) * texture(_Texture,fs_in.UV);

}