#version 330 core
layout (lines) in;
layout (line_strip, max_vertices = 22) out;
in VS_OUT 
{
	vec2 aPos;
	vec2 lhandler;
	vec2 rhandler;
}vs_in[];


uniform vec2 wDim;


vec2 cubicBezier(vec2 p0, vec2 p1,vec2 p2,vec2 p3, float t)
{
	float _t = 1-t;
	return  pow(_t,3)*p0+3*pow(_t,2)*t*p1 + 3*_t*pow(t,2)*p2 + pow(t,3)*p3;
	
}

vec4 transform2vec4(vec2 vec)
{
	return vec4((2.0f*vec.x-wDim.x)/wDim.x,(-2.0f*vec.y+wDim.y)/wDim.y, 0.0f, 1.0f);
}

void main()
{
	const unsigned int subs = 20; //Subdivisions
	float seg = 1.0f/(subs+1);
	float t = 0.0f;
	gl_Position = transform2vec4(vs_in[0].aPos);
	EmitVertex();

	for(int i=0; i<subs; ++i)
	{
		t+=seg;
		gl_Position = transform2vec4(cubicBezier(vs_in[0].aPos,vs_in[0].rhandler,vs_in[1].lhandler,vs_in[1].aPos, t));
		EmitVertex();
	}

	gl_Position = transform2vec4(vs_in[1].aPos);
	EmitVertex();
	EndPrimitive();
}