#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 20) out;

vec4 quadBezier(vec4 p0, vec4 p1, vec4 p2, float t)
{
	float ex_t = 1-t;
	vec4 result=vec4(0.0f, 0.0f, 0.0f, 1.0f);
	result.x= ex_t*ex_t*p0.x + 2*t*ex_t*p1.x + t*t*p2.x;
	result.y= ex_t*ex_t*p0.y + 2*t*ex_t*p1.y + t*t*p2.y;
	return result;
}

void main()
{
	const unsigned int subs = 0; //Subdivisions
	float seg = 1.0f/(subs+1);
	float t = 0.0f;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	for(int i=0; i<subs; ++i)
	{
		t+=seg;
		gl_Position = quadBezier(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, t);
		EmitVertex();
	}
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}