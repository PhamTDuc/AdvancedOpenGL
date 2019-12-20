#pragma once
#include "Shader.h"

class ShapeRenderer
{
private:
	static unsigned int VAO, VBO;
	static const float vertices[8];
public:
	static Shader ShapeShader;
	static void createContext(unsigned int w, unsigned int h);
	void draw(const glm::vec3& ,const glm::vec2&,const glm::vec2&);
	static void updatewDim(unsigned int w, unsigned int h);
};