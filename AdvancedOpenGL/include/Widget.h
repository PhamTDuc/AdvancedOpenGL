#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Widget
{
public:
	int x_m, y_m, w_m, h_m;
	glm::vec3 color_m;
	Widget* parent = nullptr;
	std::vector<Widget*> children;
	Widget(int x = 0, int y = 0, int w = 0, int h = 0,const glm::vec3 &color=glm::vec3(0.5f), Widget* pa = nullptr) :x_m(x), y_m(y), w_m(w), h_m(h),color_m(color), parent(pa) 
	{}

	virtual void onDragCallback(GLFWwindow*, double, double, double, double) {}
	virtual void onClick() {}
	virtual void onDropCallback() {}
	void draw(Shader& shader, unsigned int VAO)
	{
		shader.use();
		shader.setVec3("color", color_m);
		shader.setVec2("translate", glm::vec2(x_m, y_m));
		shader.setVec2("scale", glm::vec2(w_m, h_m));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	

	void drawAll(Shader& shader, unsigned int VAO)
	{
		//Draw Parent
		draw(shader,VAO);
	
		//Draw Children
		for (auto& child : children)
		{
			child->drawAll(shader,VAO);
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