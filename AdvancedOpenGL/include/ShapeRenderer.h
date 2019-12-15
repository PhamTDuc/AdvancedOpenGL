#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include <iostream>
#include <string_view>
#include <forward_list>
#include <memory>
#include <map>
#include "Shader.h"

class ShapeRenderer
{
private:
	static unsigned int VAO, VBO;
	static const float vertices[8];
public:
	static void createContext();
	void ShapeRenderer::draw(Shader& shader, glm::vec3 color, glm::vec2 translate, glm::vec2 scale);

};