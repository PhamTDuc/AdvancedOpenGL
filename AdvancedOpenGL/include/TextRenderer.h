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

class TextRenderer
{
private:
	struct Character
	{
		glm::ivec2 pos;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advanced;
	};

	std::uint16_t dims;
	std::map<char32_t, Character> Atlas;
	unsigned int texture;
	unsigned int text_size;

	static unsigned int textVAO, textVBO;
public:
	static void createContext();
	//Clear Previous Map and Regenerate GlyphsTexture and Map
	unsigned int generateFont(std::u32string_view string, unsigned int text_size = 20);
	void renderText(std::u32string_view string, Shader& shader, float scale = 2, unsigned int x = 0, unsigned y = 0, unsigned int w=0);
	glm::uvec2 getVHBBox(std::u32string_view string, float scale = 2);
};
