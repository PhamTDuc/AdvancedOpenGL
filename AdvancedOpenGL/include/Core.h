#pragma once
namespace GUI
{
	struct Mouse
	{
		double firstX, firstY;
		double x, y;
		bool LeftB, MiddleB, RightB;
		bool releaseAll = true;;
		bool isClicked;
	};
}