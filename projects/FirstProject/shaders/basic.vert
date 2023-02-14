#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoords;
out vec3 fragColor;
out vec2 texCoords;

uniform mat4 pvm;

void main()
{
   gl_Position = pvm * vec4(aPos, 1.0);
   fragColor = inColor;
   texCoords = inTexCoords;
};