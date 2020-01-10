#pragma once
#include "Widget.h"
class Menu :public Widget
{
private:
	struct MenuItem
	{
		std::u32string_view label;
		Menu* subMenu = nullptr;
		MenuItem(std::u32string_view label, Menu* subMenu = nullptr)
		{
			this->label = label;
			this->subMenu = subMenu;
		}
	};

	unsigned int  n_items;
	int hoverItem = -1;
	std::vector<MenuItem> items;
	const static unsigned int itemHeight = 25;
	Menu* onHoverMenu = nullptr;
	void update(int diffX, int diffY)
	{
		this->x_m += diffX;
		this->y_m += diffY;
		for (auto item : items)
		{
			if (item.subMenu)
				item.subMenu->update(diffX, diffY);
		}
	}

	bool isOverMenu(GUI::Mouse& event)
	{
		return (this->x_m < event.x && event.x < (this->x_m + this->w_m) && this->y_m < event.y && event.y < (this->y_m + this->h_m));
	}

	void resetSubMenuItem()
	{
		if (onHoverMenu)
		{
			onHoverMenu->hoverItem = -1;
			onHoverMenu->onHoverMenu = nullptr;
		}
	}
public:
	Menu() : Widget(0, 0, 120, 0, glm::vec3(0.78)), n_items(0) {}

	void addItem(std::u32string_view label, Menu* subMenu = nullptr)
	{

		++n_items;
		if(subMenu)
			subMenu->update(w_m - 2, h_m);
		h_m += itemHeight;
		items.emplace_back(label, subMenu);
	}


	void draw() override
	{
		shaperenderer.draw(color_m, glm::vec2(x_m, y_m), glm::vec2(w_m, h_m));
		if(hoverItem !=-1)
			shaperenderer.draw(glm::vec3(1.0f), glm::vec2(x_m, y_m + itemHeight * hoverItem), glm::vec2(w_m, itemHeight));
		for (int i = 0; i < n_items; ++i)
		{
			textrenderer.renderTextAlign(items[i].label, .7f, glm::vec3(0.42f, 0.074f, 0.81f), x_m, y_m + itemHeight * i, w_m, itemHeight, Align::CENTER_CENTER);
		}
		if (onHoverMenu)
			onHoverMenu->draw();
	}

	bool isOver(GUI::Mouse& event) override
	{
		if (isOverMenu(event))
			return true;
		else
			if (onHoverMenu)
				return onHoverMenu->isOver(event);
		hoverItem = -1;
		return false;
	}


	void exec_(GUI::Mouse& event) 
	{
		if (isOverMenu(event))
		{
			hoverItem = (event.y - this->y_m) / itemHeight;
			onHoverMenu = items[this->hoverItem].subMenu;
			resetSubMenuItem();
			if (event.isClicked)
			{
				std::cout << "Menu "<<this<<" HoverItem: " << hoverItem << '\n';
				event.isClicked = false;
			}

		}

		if (this->onHoverMenu)
			this->onHoverMenu->exec_(event);
	}


	void onDragCallback(GUI::Mouse& event) override
	{
		//std::cout << "First X:" << event.firstX << "  First Y:" << event.firstY << "\n";
		this->update(event.x - event.firstX, event.y - event.firstY);
	}
};
