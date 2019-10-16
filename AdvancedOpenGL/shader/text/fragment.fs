#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 color;

void main()
{
	FragColor = vec4(color,texture(text,texCoord).r);
}