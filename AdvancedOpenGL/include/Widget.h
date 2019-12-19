#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "TextRenderer.h"
#include "ShapeRenderer.h"

class Widget
{
private:
public:
	static TextRenderer textrenderer;
	static ShapeRenderer shaperenderer;
	int x_m, y_m, w_m, h_m;
	std::u32string name_m;
	glm::vec3 color_m;
	Widget* parent = nullptr;
	std::vector<Widget*> children;
	Widget(int x = 0, int y = 0, int w = 0, int h = 0,const glm::vec3 &color=glm::vec3(0.5f), Widget* pa = nullptr, std::u32string_view name = U"") :x_m(x), y_m(y), w_m(w), h_m(h),color_m(color),parent(pa), name_m(name) {}


	virtual void onDragCallback(GLFWwindow*, double, double, double, double) {}
	virtual void onClick() {}
	virtual void onDropCallback() {}


	virtual void draw(Shader& textshader,float scale, Shader& shapeshader)
	{
		shaperenderer.draw(shapeshader, color_m, glm::vec2(x_m, y_m), glm::vec2(w_m, h_m));
		glm::vec2 vhbbox = textrenderer.getVHBBox(this->name_m, scale);
		//textrenderer.renderText(this->name_m, textshader, scale, x_m+(w_m-vhbbox.x)/2, 25, glm::uvec2(0, 0));
		textrenderer.renderTextAlign(this->name_m, textshader, scale, x_m, y_m, w_m, h_m, TextRenderer::TextAlign::CENTER_CENTER);
	}

	void drawAll(Shader& textshader,float scale,Shader& shapeshader)
	{
		//Draw Parent
		draw(textshader, scale, shapeshader);
	
		//Draw Children
		for (auto& child : children)
		{
			child->drawAll(textshader,scale,shapeshader);
		}
	}

	void update(int diffX, int diffY)
	{
		this->x_m += diffX;
		this->y_m += diffY;
		for (auto& child : children)
		{
			child->update(diffX, diffY);
		}
	}

};
