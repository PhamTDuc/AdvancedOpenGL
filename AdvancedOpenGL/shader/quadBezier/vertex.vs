#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1 ) in vec2 lhandler;
layout (location = 2 ) in vec2 rhandler;
out VS_OUT 
{
	vec2 aPos;
	vec2 lhandler;
	vec2 rhandler;
} vs_out;


void main()
{
	//gl_Position = vec4((2.0f*aPos.x-wDim.x)/wDim.x,(-2.0f*aPos.y+wDim.y)/wDim.y, 0.0f, 1.0f);
	//point = vec2(gl_Position.x,gl_Position.y);
	vs_out.aPos = aPos;
	vs_out.lhandler = lhandler;
	vs_out.rhandler = rhandler;
}