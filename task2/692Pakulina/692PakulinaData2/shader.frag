#version 330

uniform sampler2D iceTex;
uniform sampler2D mramorTex;
uniform sampler2D sandTex;
uniform sampler2D snowTex;

uniform sampler2D maskTex;

struct LightInfo
{
	vec3 pos;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

uniform LightInfo light;

in vec3 normalCamSpace;
in vec4 posCamSpace;
in vec2 texCoord;
out vec4 fragColor;

const vec3 KsSnow = vec3(1.0, 1.0, 1.0);
const vec3 KsIce = vec3(0.5, 0.5, 0.5);
const vec3 KsSand = vec3(0.8, 0.8, 0.8);
const vec3 KsMramor = vec3(0.2, 0.2, 0.2);
const float shininess = 128.0;

void main()
{
	vec4 maskColor = texture(maskTex, texCoord).rgba;


	vec3 iceColor = texture(iceTex, texCoord).rgb * maskColor.r;
	vec3 sandColor = texture(sandTex, texCoord).rgb * maskColor.g;
	vec3 snowColor = texture(snowTex, texCoord).rgb * maskColor.b;
	vec3 mramorColor = texture(mramorTex, texCoord).rgb * maskColor.a;


	vec3 normal = normalize(normalCamSpace);
	vec3 viewDirection = normalize(-posCamSpace.xyz);

	vec3 lightDirCamSpace = normalize(light.pos - posCamSpace.xyz);

	float NdotL = max(dot(normal, lightDirCamSpace.xyz), 0.0);

	vec3 color = (iceColor + snowColor + mramorColor + sandColor) * (light.La + light.Ld * NdotL);

	vec3 Ks = KsIce * maskColor.r + KsSand * maskColor.g + KsSnow * maskColor.b + KsMramor * maskColor.a;

	if (NdotL > 0.0)
	{
		vec3 halfVector = normalize(lightDirCamSpace.xyz + viewDirection);
		float blinnTerm = max(dot(normal, halfVector), 0.0);
		blinnTerm = pow(blinnTerm, shininess);

		color += light.Ls * Ks * blinnTerm;
	}

	fragColor = vec4(color, 1.0);
}

