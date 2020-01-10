#pragma once
#include <vector>
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "TextRenderer.h"
#include "ShapeRenderer.h"
#include "Core.h"



class Widget
{
public:
	static TextRenderer textrenderer;
	static ShapeRenderer shaperenderer;
	int x_m, y_m, w_m, h_m;
	glm::vec3 color_m;
	Widget* parent = nullptr;
	Widget* menu = nullptr;
	std::vector<Widget*> children;
public:
	Widget(int x = 0, int y = 0, int w = 0, int h = 0,const glm::vec3 &color=glm::vec3(0.5f)) :x_m(x), y_m(y), w_m(w), h_m(h),color_m(color){}

	virtual void onDragCallback(GUI::Mouse &event) {}
	virtual void onHover(GUI::Mouse &event){}
	virtual void onClick() {}
	virtual void onDropCallback() {}
	virtual void draw(){}

	void drawAll()
	{
		//Draw Parent
		this->draw();
		if (menu)
			menu->draw();

		//Draw Children
		for (auto& child : children)
		{
			child->drawAll();
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

	void add(Widget& widget)
	{
		widget.update(this->x_m, this->y_m);
		widget.parent = this;
		children.emplace_back(&widget);
	}

	void popback()
	{
		children.pop_back();
	}
	virtual bool isOver(GUI::Mouse &mouse) { return false;}
	virtual void exec_(GUI::Mouse& event) {};
};
