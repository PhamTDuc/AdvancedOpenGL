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
	virtual void drawShape(Shader& shader)
	{
		shaperenderer.draw(shader, color_m, glm::vec2(x_m, y_m), glm::vec2(w_m, h_m));
	}

	virtual void drawText(Shader& shader, float scale)
	{
		textrenderer.renderText(this->name_m, shader, scale, x_m, y_m,glm::uvec2(0,43));
	}

	virtual void draw(Shader& shader,float scale)
	{
		drawText(shader, scale);
		//drawShape(shader);
	}

	void drawAll(Shader& shader,float scale)
	{
		//Draw Parent
		draw(shader, scale);
	
		//Draw Children
		for (auto& child : children)
		{
			child->drawAll(shader,scale);
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
