#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 wDim;

out vec2 point;
void main()
{
	gl_Position = vec4((2.0f*aPos.x-wDim.x)/wDim.x,(-2.0f*aPos.y+wDim.y)/wDim.y, 0.0f, 1.0f);
	point = vec2(gl_Position.x,gl_Position.y);
}