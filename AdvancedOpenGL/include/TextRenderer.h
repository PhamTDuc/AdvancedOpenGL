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
		glm::ivec2 bearing;
		unsigned int advanced;
	};

	std::uint16_t dims;
	std::map<char32_t, Character> Atlas;
	unsigned int texture;
	unsigned int text_size;

	static unsigned int textVAO, textVBO;
public:
	//Clear Previous Map and Regenerate GlyphsTexture and Map
	static void createContext()
	{
		glGenBuffers(1, &textVBO);
		glGenVertexArrays(1, &textVAO);

		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, nullptr, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	unsigned int generateFont(std::u32string_view string, unsigned int text_size = 20)
	{

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		FT_Face face;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		FT_Set_Pixel_Sizes(face, text_size, 0);
		if (FT_Load_Char(face, 'D', FT_LOAD_RENDER))
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << std::endl;
		//FreeTypeLoad----------End
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FT_GlyphSlot slot = face->glyph;
		if (FT_Load_Char(face, 'A', FT_LOAD_RENDER))
			std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";

		this->dims = glm::ceil(glm::sqrt((float)string.size()));
		this->text_size = text_size;
		

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, text_size * dims, text_size * dims, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		Character ch;


		for (std::uint32_t i = 0; i < string.size(); ++i)
		{
			if (FT_Load_Char(face, string[i], FT_LOAD_RENDER))
				std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
			else
			{
				ch.pos.x = glm::mod((float)i, (float)dims);
				ch.pos.y = i / dims;
				ch.bearing = glm::ivec2(slot->bitmap_left, slot->bitmap_top);
				ch.advanced = slot->advance.x >> 6;
				glTexSubImage2D(GL_TEXTURE_2D, 0, text_size * ch.pos.x, text_size * ch.pos.y, slot->bitmap.width, slot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
				Atlas.emplace(string[i], ch);
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		this->texture = texture;
		return texture;
	}

	
	void renderText(std::u32string_view string, Shader& shader, float scale = 2, unsigned int x = 0, unsigned y = 0)
	{
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, nullptr, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		shader.setInt("text", 0);

		float step = 0.995f /dims;
		int x_run = x, y_run = y;
		for (auto& ch : string)
		{
			if (ch == '\n')
			{
				x_run = x;
				y_run += this->text_size * scale;
			}
			else
			{
				int xPos = x_run + Atlas[ch].bearing.x * scale;
				int yPos = y_run + this->text_size * scale - Atlas[ch].bearing.y * scale;
				float vertices[] = {
					xPos, yPos, Atlas[ch].pos.x * step, Atlas[ch].pos.y * step,
					xPos + this->text_size * scale, yPos, (Atlas[ch].pos.x + 1.0f) * step, Atlas[ch].pos.y * step,
					xPos, yPos + this->text_size * scale, Atlas[ch].pos.x * step, (Atlas[ch].pos.y + 1.0f) * step,
					xPos + this->text_size * scale, yPos + this->text_size * scale, (Atlas[ch].pos.x + 1.0f) * step, (Atlas[ch].pos.y + 1.0f) * step
					};

				glBindVertexArray(textVAO);
				glBindBuffer(GL_ARRAY_BUFFER, textVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				x_run += Atlas[ch].advanced * scale;
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glm::uvec2 getVHBBox(std::u32string_view string, float scale = 2)
	{


		float step = 0.995f / 6.0f;
		unsigned int w = 0, h = 0;
		int x_run = 0, y_run = 0;
		for (auto& ch : string)
		{
			if (ch == '\n')
			{
				w = w < x_run ? x_run : w;
				y_run += this->text_size * scale;
				x_run = 0;
			}
			else
			{
				x_run += Atlas[ch].advanced * scale;
			}
		}
		w = w < x_run ? x_run : w;
		h = y_run + this->text_size * scale;
		return glm::uvec2(w, h);
	}
};