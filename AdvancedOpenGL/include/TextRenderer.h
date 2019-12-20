#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include <string_view>
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
	enum class TextAlign
	{
		TOP_LEFT = 0, TOP_CENTER, TOP_RIGHT,
		CENTER_LEFT, CENTER_CENTER, CENTER_RIGHT,
		BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
	};
	typedef TextRenderer::TextAlign Align;
	static Shader TextShader;
	static void createContext(unsigned int w, unsigned int h);
	static void updatewDim(unsigned int w, unsigned int h);
	//Clear Previous Map and Regenerate GlyphsTexture and Map
	unsigned int generateFont(std::u32string_view string, unsigned int text_size = 20);
	void renderText(std::u32string_view string, float scale = 2,const glm::vec3 & color=glm::vec3(1.0f), int x = 0, int y = 0, const glm::uvec2 &margin=glm::uvec2(0));
	void renderTextAlign(std::u32string_view string,float scale = 2,const glm::vec3 &color = glm::vec3(1.0f), int x=0, int y=0, unsigned int w=0, unsigned int h=0,Align align=Align::TOP_LEFT,const glm::uvec2 &margin=glm::uvec2(0));
	glm::uvec2 getVHBBox(std::u32string_view string, float scale = 2);
};
typedef TextRenderer::TextAlign Align;