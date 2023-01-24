#version 330
in vec2 ShadertextureCoord;
in vec3 normal;
in vec3 FragPos;
in vec3 cameraFragPos;

uniform float sunAlpha;
uniform vec3 sunDirection;
uniform int flashLightEnabled;
uniform int countOflights;
uniform vec4 lights[6];
uniform int objectType;
uniform vec3 eyePos;
uniform vec3 eyeDirection;

uniform sampler2D MTexture;
uniform vec3 lightColor;
uniform float ambientRegulator;
uniform int waterFrame;
uniform int fogEnabled;

uniform vec3 	pointPosition;
uniform vec3 	pointColor;
uniform float pointIntensity;

out vec4 color;

//================================================================================================
float ambient = 0.5;
float specularStrength = 0.8;
float diffuseStrength = 0.8;
//================================================================================================
float getPointLight()
{
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(FragPos - pointPosition );
	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength;

	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;
	float distance = length(pointPosition - FragPos);
	float attenuation = 1.0 / (0.5 * distance + 0.5 * distance * distance);

	return (diffuse + specular) * attenuation;
}
//================================================================================================
float getFlashlight()
{
	vec3 lightDir = normalize(eyePos - FragPos);
	float spotAngle = max(0.0f, dot(-lightDir, normalize(eyeDirection)));

	return (spotAngle < 0.95f) ? 0.0f : pow(spotAngle, 102);
}
//================================================================================================
float flashlighPhong()
{
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(eyePos - FragPos);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	return ((diffuse + ambient  + specular) * getFlashlight());
}
//================================================================================================
float fogLight()
{
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2(-0.1 * 0.1 * z * z *LOG2);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	return fogFactor;
}
//================================================================================================
float directionPhong()
{
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(sunDirection);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;
	return (diffuse + ambient  + specular);
}
//================================================================================================
vec4 waterAnimationTexture()
{
	float xPos  = ShadertextureCoord.x / 4;
	float yPos  = ShadertextureCoord.y / 4;
	float division = waterFrame / 4;
	xPos += (waterFrame % 4) * 0.25f;
	yPos += floor(division) * 0.25f;

	return texture(MTexture, vec2(xPos, yPos));
}
//================================================================================================
void main()
{
	vec3 lighting =	directionPhong() * lightColor;

	if(objectType != 5)
	{
		if(objectType == 1)
		{
			color = vec4(lightColor, 1.0f) * texture(MTexture, ShadertextureCoord) * (1 - sunAlpha);
		}
		else
		{
			if (flashLightEnabled != 0)
				lighting += flashlighPhong() * lightColor;

			color = vec4(lighting, 1.0f) * texture(MTexture, ShadertextureCoord);

			color += getPointLight() * pointIntensity * vec4(pointColor, 1.0f);
		}
	}
	if(objectType == 5)
		color = vec4(lighting, 1.0f) * waterAnimationTexture();

	if(fogEnabled != 0)
		color = mix(vec4(0.87f, 0.87f, 0.87f, 0.1f), color, fogLight());
}
