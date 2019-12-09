#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 wDim;
uniform vec2 offset;
uniform vec2 translate;
uniform vec2 scale;

void main()
{
	gl_Position = vec4((2.0f*(aPos.x*scale.x+translate.x+offset.x)-wDim.x)/wDim.x,(-2.0f*(aPos.y*scale.y+translate.y+offset.y)+wDim.y)/wDim.y, 0.0f, 1.0f);
}