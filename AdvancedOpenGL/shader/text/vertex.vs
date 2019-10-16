#version 330 core
layout (location = 0) in vec4 aPos;
uniform vec2 wDim;
out vec2 texCoord;

void main()
{
	gl_Position = vec4((2.0f*aPos.x-wDim.x)/wDim.x,(-2.0f*aPos.y+wDim.y)/wDim.y, 0.0f, 1.0f);
	texCoord = aPos.zw;
}