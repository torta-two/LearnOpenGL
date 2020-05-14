#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 AmbientColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float AmbientStrength;
uniform vec3 AmbientLightColor;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Normal = aNormal;
	AmbientColor = AmbientLightColor * AmbientStrength;
}