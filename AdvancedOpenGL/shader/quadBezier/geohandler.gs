#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 3) out;
in VS_OUT 
{
	vec2 aPos;
	vec2 lhandler;
	vec2 rhandler;
}vs_in[];


uniform vec2 wDim;


vec4 transform2vec4(vec2 vec)
{
	return vec4((2.0f*vec.x-wDim.x)/wDim.x,(-2.0f*vec.y+wDim.y)/wDim.y, 0.0f, 1.0f);
}

void main()
{
	gl_Position = transform2vec4(vs_in[0].lhandler);
	EmitVertex();
	gl_Position = transform2vec4(vs_in[0].aPos);
	EmitVertex();
	gl_Position = transform2vec4(vs_in[0].rhandler);
	EmitVertex();
	EndPrimitive();
}