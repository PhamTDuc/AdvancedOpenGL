#include <iostream>
#include <sigslot/signal.hpp>
#include <memory>

int sum = 0;

struct button
{
	sigslot::signal<> clicked;
};

struct MouseEvent 
{
	//double x, y; //Current Mouse
	double firstX, firstY; //First Pressed Occured
	double diffX, diffY;
	bool leftB, midB, rightB;
	Widget*  keep = nullptr;
};


struct CoreApplication
{
	static MouseEvent mouseEvent;
	
	bool isOver(const Widget& widget)
	{
		if (widget.x_m < x && x < (widget.x_m + widget.w_m) && widget.y_m < y && y < (widget.y_m + widget.h_m))
			return true;
		return false;
	}



	void exec_(const Widget *root = nullptr) noexcept
	{
		//Implement Onclick Event
		if (leftB)
		{
			//Tranversal through every Widget from parent
			if isOver(widget)
				widget.clicked(mouseEvent);
		}

		if (leftB && diffX || diffY)
		{
			//Tranversal through every Widget from parent
				if (isOver(widget) && widget.draggable)
					widget.onDragCallback(mouseEvent);

		}
		
		if (mouseEvent.keep && mouseEvent.keep.parent && keep.parent.droppable)
		{
			widget.onDropCallback(mouseEvent);
		}

	}
};



struct Widget 
{
	double x_m, y_m, w_m, h_m;
	bool draggable = true;	

	Widget(Widget* parent = nullptr){}
	void onDragCallback(const MouseEvent &event) {};
	void onDropCallback(const MouseEvent &event) {};
	void draw() {};
};



struct object
{
	bool draggable = true;
	

	void callback()
	{
		std::cout << "Callback function\n";
	}
};
int main() {
	
	std::shared_ptr<object> obj = std::make_shared<object>();
	button btn;
	btn.clicked.connect(&object::callback,obj);


	btn.clicked();
	obj.reset();
	btn.clicked();
	std::cin.get();
}