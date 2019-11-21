#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 instances;
uniform vec2 wDim;


void main()
{
	gl_Position = vec4((2.0f*(aPos.x + instances.x)-wDim.x)/wDim.x,(-2.0f*(aPos.y + instances.y)+wDim.y)/wDim.y, 0.0f, 1.0f);
}


