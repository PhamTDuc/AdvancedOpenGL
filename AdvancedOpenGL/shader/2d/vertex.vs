#version 330 core
layout (location = 0) in vec2 aPos;
//uniform mat4 proj2D;
uniform vec2 wDim;
void main()
{
	//aPos.x = (2.0f*aPos.x-wDim.x)/wDim.x;
	//aPos.y = (2.0f*aPos.y-wDim.y)/wDim.y;
	gl_Position = vec4((2.0f*aPos.x-wDim.x)/wDim.x,(-2.0f*aPos.y+wDim.y)/wDim.y, 0.0f, 1.0f);
}