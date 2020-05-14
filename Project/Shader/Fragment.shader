#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 AmbientColor;

uniform sampler2D texture_diffuse1;

void main()
{
	float diff = dot(Normal, vec3(-0.5f, 0.3f, 0)) * 0.5 + 0.5;
    FragColor = texture(texture_diffuse1, TexCoord) * diff + vec4(AmbientColor, 1);
}