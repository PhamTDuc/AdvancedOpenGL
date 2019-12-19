#version 330 core
in vec2 texCoord;
in vec2 absPos;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 color;
uniform vec2 margin;

vec4 filled(float distance, float linewidth, float antialias, vec4 fill)
{
	float t = linewidth/2.0f - antialias;
	float signed_distance = distance;
	float border_distance = abs(signed_distance) - t;
	float alpha = border_distance / antialias;
	alpha = exp(-alpha * alpha);

	if(border_distance <0.0f)
		return fill;
	else if(signed_distance < 0.0f)
		return fill;
	else 
		return vec4(fill.rgb, alpha*fill.a);
}

void main()
{
	if((margin.x !=0.0f && absPos.x > margin.x) || (margin.y!=0.0f && absPos.y > margin.y))
		FragColor = vec4(0.0f,0.0f,0.0f,0.0f);
	else
		FragColor = vec4(color,texture(text,texCoord).r);
	//FragColor = filled(0.5,20.0,10.0, vec4(color,texture(text,texCoord).r));
	//FragColor = vec4(color, 1.0f);
}