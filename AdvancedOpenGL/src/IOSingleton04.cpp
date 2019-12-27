﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string_view>
#include <memory>
#include "Shader.h"
#include "Widget.h"
#include "TextRenderer.h"
#include "Config.h"


class Application
{
private:
	enum MOUSE_STATUS { FREE_MOUSE = 0, BUSY_MOUSE };
public:
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static GUI::Mouse MouseEvent;
	static MOUSE_STATUS mouse_status;
	static GLFWwindow* window;
	static Widget* hot;
	static Widget* active;
	static std::unique_ptr<Widget> root;
	//static TextRenderer textrenderer;
	Application()
	{
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
		//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
		//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IO Implementation", nullptr, nullptr);
	}

	~Application()
	{
		glfwTerminate();
	}

	static void setWindow(const std::string &name)
	{
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str() , nullptr, nullptr);
		if (!window)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			//glfwTerminate();
		}
		else
		{
			glfwMakeContextCurrent(window);
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetCursorPosCallback(window, cursor_position_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetErrorCallback([](int error, const char* description) {std::cout << error << "   Description: " << description << '\n';});
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
		root = std::make_unique<Widget>();

		//Configuration GUI 
		//Configuration GUI 
		GUI::createContext(SCR_WIDTH, SCR_HEIGHT);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		SCR_WIDTH = width;
		SCR_HEIGHT = height;
		GUI::updatewDim(width, height);

	}

	//static void printMouseState()
	//{
	//	std::cout << "MousePos:" << MouseEvent.x << " " << MouseEvent.y << "   LEFT " << MouseEvent.LeftB << "   RIGHT " << MouseEvent.RightB << "   MIDDLE " << MouseEvent.MiddleB << '\n';
	//}

	static void processMouseState()
	{
		if (hot && hot->isOver(MouseEvent))
		{
			hot->onHover(MouseEvent);
			if (!active)
			{
				if (MouseEvent.LeftB || MouseEvent.RightB || MouseEvent.MiddleB)
				{
					active = hot;
					if (MouseEvent.releaseAll)
					{
						glfwGetCursorPos(window, &MouseEvent.firstX, &MouseEvent.firstY);
						MouseEvent.releaseAll = false;
					}
				}
				else
				{
					MouseEvent.releaseAll = true;
				}
			}
		}
		

		if (active)
		{
			if (MouseEvent.LeftB)
			{
				active->onDragCallback(MouseEvent);
			}
			else
			{
				if(active->isOver(MouseEvent))
					active->onClick();
				active->onDropCallback();
				active = nullptr;
			}
		}

	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		//std::cout << xpos << "   " << ypos<<'\n';
		glfwGetCursorPos(window, &MouseEvent.x, &MouseEvent.y);
		tranversal(root.get());
		processMouseState();
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		MouseEvent.LeftB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		MouseEvent.RightB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		MouseEvent.MiddleB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		processMouseState();
		uint8_t rgb[3];
		glReadBuffer(GL_LEFT);
		glReadPixels(MouseEvent.x, SCR_HEIGHT -1- MouseEvent.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &rgb);
		std::cout << "Color RGB:" << +rgb[0] << " " << +rgb[1] << " " << +rgb[2] << "\n";
	}


	
	static void tranversal(Widget* node = nullptr)
	{
		if (mouse_status == FREE_MOUSE && node)
		{
			if (node->isOver(MouseEvent))
				hot = node;
			for (auto& child : node->children)
			{
				tranversal(child);
			}
		}
	}


	void exec_()
	{
		//unsigned int texture = TextRenderer::generateFont(U"abcdefghiklmn", 20);
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			//processMouseState();

			//Render3D
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//Render3D ----- End 
			
			//Render2D
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			root->drawAll();

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwSwapInterval(1);
		}

	}
};

unsigned int Application::SCR_WIDTH;
unsigned int Application::SCR_HEIGHT;
GUI::Mouse Application::MouseEvent;
Application::MOUSE_STATUS Application::mouse_status;

GLFWwindow* Application::window;
Widget* Application::hot = nullptr;
Widget* Application::active = nullptr;
std::unique_ptr<Widget> Application::root;

class Message :public Widget
{
private:
	std::u32string_view message;
public:
	Message(std::u32string_view mess,int x=0, int y=0,int w=0, int h=0): Widget(x,y,w,h, glm::vec3(0.4, 0.47, 0.92)), message(mess){}
	void draw() override
	{
		textrenderer.renderTextAlign(message,.8f, glm::vec3(0.42f, 0.074f, 0.81f), x_m, y_m, w_m, h_m, Align::CENTER_CENTER);
	}
};

class Frame :public Widget
{
public:
	Frame(int x=0, int y=0, int w =0, int h =0): Widget(x,y,w,h,glm::vec3(0.4,0.75,0.92)){}
	void draw() override
	{
		shaperenderer.draw(color_m, glm::vec2(x_m, y_m), glm::vec2(w_m, h_m));
	}

	bool isOver(GUI::Mouse& event) override
	{
		return (this->x_m < event.x && event.x < (this->x_m + this->w_m) && this->y_m < event.y && event.y < (this->y_m + this->h_m));
	}

	void onDragCallback(GUI::Mouse& event) override
	{
		
		//std::cout << "First X:" << event.firstX << "  First Y:" << event.firstY << "\n";
		this->update(event.x-event.firstX,event.y-event.firstY);
		event.firstX = event.x;
		event.firstY = event.y;
	}

	void onHover(GUI::Mouse& event) override
	{
		Message* msg = new Message(U"Hello the world frame", 0, 0, 100, 50);
		this->add(*msg);
	}
};



class Button :public Widget
{
private:
	std::u32string label_m;
public:
	Button(std::u32string_view label,int x = 0, int y = 0, int w = 0, int h = 0) : Widget(x, y, w, h, glm::vec3(0.4, 0.47, 0.92)), label_m(label) {}
	void draw() override
	{
		shaperenderer.draw(color_m, glm::vec2(x_m, y_m), glm::vec2(w_m, h_m));
		textrenderer.renderTextAlign(label_m, .8f, glm::vec3(0.42f,0.074f,0.81f), x_m, y_m, w_m, h_m, Align::CENTER_CENTER);
	}

	bool isOver(GUI::Mouse& event) override
	{
		return (this->x_m < event.x && event.x < (this->x_m + this->w_m) && this->y_m < event.y && event.y < (this->y_m + this->h_m));
	}

	void onClick() override
	{
		std::cout << "Is clicked\n";
		Message* msg =new Message(U"Hello",150,0,100,50);
		this->add(*msg);
	}

	void onHover(GUI::Mouse& event) override
	{
		Message* msg = new Message(U"Hello the world", 200, 0, 100, 50);
		this->add(*msg);
	}
};



int main()
{
	Application app;
	app.SCR_WIDTH = 400;
	app.SCR_HEIGHT = 600;
	app.setWindow("Hello the world");


	//Button btn2(100, 300, 150, 50, glm::vec3(0.0f, 0.5f, 1.0f));
	//Button btn3(20, 20, 100, 40, glm::vec3(0.0f, 1.0f, 0.0f));
	//Button btn4(200, 100, 120, 120, glm::vec3(1.0f, 1.0f, 0.0f));
	//btn2.update(-50, 0);
	//btn2.add(btn3);
	//app.root->add(btn2);
	//app.root->add(btn4);

	Frame frame(0, 0, 150, 200);
	Frame frame2(0, 300, 150, 200);

	Button btn1(U"Red Color",5, 10, 140, 36);
	Button btn2(U"Green Color",5, 50, 140, 36);
	Button btn3(U"Blue Color",5, 90, 140, 36);
	Button btn4(U"Yellow Colorg",5, 130, 140, 36);
	Button btn5(U"Magneta Color",5, 10, 140, 36);


	frame.add(btn1);
	frame.add(btn2);
	frame.add(btn3);
	frame.add(btn4);

	frame2.add(btn5);
	app.root->add(frame);
	app.root->add(frame2);

	app.exec_();

	std::cin.get();
	return 0;
}
