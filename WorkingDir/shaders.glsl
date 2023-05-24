///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef TEXTURED_GEOMETRY

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location=0) in vec3 aPosition;
layout(location=1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
	vTexCoord = aTexCoord;
	gl_Position = vec4(aPosition,1.0);
}

#elif defined(FRAGMENT) ///////////////////////////////////////////////

in vec2 vTexCoord;
uniform sampler2D uTexture;
layout(location=0) out vec4 oColor;

void main()
{
	oColor = texture(uTexture,vTexCoord);
}

#endif
#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef MODEL_SHADER

#if defined(VERTEX) ///////////////////////////////////////////////////

layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
//layout(location=3) in vec3 aTangent;
//layout(location=4) in vec3 aBitangent;

struct Light
{
	unsigned int type;
	vec3 color;
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float lin;
	float quadratic;
};
layout(binding = 0, std140) uniform GlobalParams
{
	vec3 uCameraPosition;
	unsigned int uLightCount;
	Light uLight[16];
};

layout(binding = 1,std140) uniform LocalParams
{
	mat4 uWorldMatrix;
	mat4 uWorldViewProjectionMatrix;
};
out vec2 vTexCoord;

out vec3 vPosition; //in world space
out vec3 vNormal; //in world space
out vec3 vViewDir;

void main()
{
	vTexCoord = aTexCoord;
	vPosition = vec3(uWorldMatrix * vec4(aPosition,1.0));
	vNormal = vec3(uWorldMatrix * vec4(aNormal, 0.0));
	vViewDir = uCameraPosition - vPosition;

	gl_Position = uWorldViewProjectionMatrix * vec4(aPosition,1.0);

}

#elif defined(FRAGMENT) ///////////////////////////////////////////////
struct Light
{
	unsigned int type;
	vec3 color;
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float lin;
	float quadratic;

};
in vec2 vTexCoord;
uniform sampler2D uTexture;
uniform sampler2D uSpecularMap;;
uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uMaterialShininess;


in vec3 vPosition;
in vec3 vNormal;
in vec3 vViewDir;

layout(binding = 0, std140) uniform GlobalParams
{
	vec3 uCameraPosition;
	unsigned int uLightCount;
	Light uLight[16];
};

layout(location=0) out vec4 oColor;

void main()
{
	vec3 finalColor = vec3(0.0f);
	for(int i = 0; i<uLightCount;++i){
		float distance = length(uLight[i].position - vPosition);
		float attenuation = 1.0f / (uLight[i].constant + uLight[i].lin * distance + uLight[i].quadratic * (distance * distance) );

		vec3 norm = normalize(vNormal);
		vec3 lightDir = normalize(uLight[i].position - vPosition);
		float diff = max(dot(norm,lightDir),0.0f);
		vec3 diffuse = vec3(texture(uTexture, vTexCoord)) * uLight[i].diffuse * diff * uLight[i].color;
		float spec = 0.0f;
		if(diff > 0.0f){
			vec3 viewDir = normalize(uCameraPosition-vPosition);
			vec3 reflectDir = reflect(-lightDir,norm);
			spec = pow(max(dot(viewDir,reflectDir),0.0f),uMaterialShininess);
		}
		vec3 specular = texture(uSpecularMap, vTexCoord).rgb * uLight[i].specular * spec * uLight[i].color;
		vec4 texColor =  texture(uTexture,vTexCoord);
		vec3 ambient = uLight[i].ambient;
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		finalColor += (ambient * vec3(texture(uTexture, vTexCoord)) * uLight[i].color + diffuse + specular) * texColor.rgb;
	} 
	oColor = vec4(finalColor,1.0f);
}
#endif
#endif



// NOTE: You can write several shaders in the same file if you want as
// long as you embrace them within an #ifdef block (as you can see above).
// The third parameter of the LoadProgram function in engine.cpp allows
// chosing the shader you want to load by name.
