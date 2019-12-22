#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec2 wDim;
uniform vec2 translate;
uniform vec2 scale;

out vec2 rectSize;
out vec2 localPos;
void main()
{
	localPos=vec2(aPos.x*scale.x,aPos.y*scale.y);
	gl_Position = vec4((2.0f*(localPos.x+translate.x)-wDim.x)/wDim.x,(-2.0f*(localPos.y+translate.y)+wDim.y)/wDim.y, 0.0f, 1.0f);
	rectSize = scale;
}