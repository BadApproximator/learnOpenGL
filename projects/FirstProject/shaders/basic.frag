#version 330 core
in vec3 vertColor;
in vec2 texCoords;
in vec3 vertNormal;
in vec3 fragPos; // world position of fragment
out vec4 outColor;

uniform sampler2D ourTexture;
uniform bool wireframeMode;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	vec3 norm = normalize(vertNormal);
	vec3 lightDir = normalize(fragPos - lightPos);

	float diffCoeff = dot(norm, -lightDir);
	vec3 diffuse = diffCoeff * lightColor;

	if (wireframeMode)
		outColor = vec4(vertColor, 1.f);
	else
		outColor = texture(ourTexture, texCoords) * vec4(diffuse, 1.0);
}