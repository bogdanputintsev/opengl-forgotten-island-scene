
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexShaderNormal;
layout(location = 2) in vec2 textureCoord;


uniform mat4 viewMatrix;
uniform mat4 transform;

out vec2 ShadertextureCoord;
out vec3 FragPos;
out vec3 normal;
out vec3 cameraFragPos;

void main()
{
	gl_Position = viewMatrix * transform * vec4(position, 1.0f);
	FragPos = vec3(transform * vec4(position, 1.0));
	normal = mat3(transpose(inverse(transform))) * vertexShaderNormal;

	ShadertextureCoord = textureCoord;
}
