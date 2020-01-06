#pragma once
#include "Core.h"
namespace GUI
{
	void createContext(unsigned int x,unsigned int y)
	{
		ShapeRenderer::createContext(x, y);
		TextRenderer::createContext(x, y);
		Widget::textrenderer.generateFont(U"abcdefghiklmnopqrstuvwxyz ABCDEFGHIKLMNOPQRSTUVWXYZ0123456789", 20);
	}

	void updatewDim(unsigned int x, unsigned int y)
	{
		ShapeRenderer::updatewDim(x, y);
		TextRenderer::updatewDim(x, y);
	}
}