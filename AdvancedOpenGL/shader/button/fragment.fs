#version 330 core

uniform vec3 color;

in vec2 rectSize;
in vec2 localPos;
out vec4 FragColor;


//All primitives are centered at the origin 
float sdfRect(vec2 p, vec2 b)
{
	vec2 d = abs(p) -b;
	float outside = length(max(d,vec2(0.0f)));
	//float inside = min(max(d.x, d.y), 0.0f);
	return outside;
}

float sdAnnularShape(float sdf ,float r )
{
  return abs(sdf) - r;
}
void main()
{	
	float radius = 8.0f;
	float blur = 1.0f; //  1.0f<=blur<=radius
	vec2 centeredPos = localPos - 0.5 * rectSize;
	vec2 halfSize = rectSize/2.0f - vec2(radius);
	float sdf =  sdAnnularShape(sdfRect(centeredPos,halfSize), radius);
	
	float opacity = smoothstep(0.0f, 1.0f, abs(clamp(-blur,0.0f,sdf)/blur));
	FragColor = vec4(color,opacity*0.2);
;}
