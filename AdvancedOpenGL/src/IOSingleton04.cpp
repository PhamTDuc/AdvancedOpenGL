#include <glad/glad.h>
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

	static void processMouseState()
	{
		std::cout << "MousePos:" << MouseEvent.x << " " << MouseEvent.y << "   LEFT " << MouseEvent.LeftB << "   RIGHT " << MouseEvent.RightB << "   MIDDLE " << MouseEvent.MiddleB << '\n';
	}

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		//std::cout << xpos << "   " << ypos<<'\n';
		glfwGetCursorPos(window, &MouseEvent.x, &MouseEvent.y);
		//processMouseState();
		tranversal(root.get());
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		MouseEvent.LeftB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		MouseEvent.RightB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		MouseEvent.MiddleB = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		//processMouseState();
		std::cout << hot<<'\n';
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
			glClearColor(0.6f, 0.2f, 0.8f, 1.0f);
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


class Button :public Widget
{
public:
	Button(int x = 0, int y = 0, int w = 0, int h = 0,glm::vec3 &color=glm::vec3(0.5f)) : Widget(x, y, w, h,color){}

	void draw() override
	{
		shaperenderer.draw(color_m, glm::vec2(x_m, y_m), glm::vec2(w_m, h_m));
		textrenderer.renderTextAlign(U"Hello the\nworldgp",1.0f,glm::vec3(1.0f,0.0f,0.0f),x_m,y_m,w_m,h_m,Align::BOTTOM_RIGHT);
		//textrenderer.renderTextAlign(U"Hello the\nworldgp",2.0f,glm::vec3(1.0f,1.0f,0.0f),x_m,y_m+50,w_m,h_m,Align::CENTER_CENTER);
	}

	bool isOver(GUI::Mouse &MouseEvent) override
	{
			return (this->x_m < MouseEvent.x && MouseEvent.x < (this->x_m + this->w_m) && this->y_m < MouseEvent.y && MouseEvent.y < (this->y_m + this->h_m));
	}
};


int main()
{
	Application app;
	app.SCR_WIDTH = 400;
	app.SCR_HEIGHT = 600;
	app.setWindow("Hello the world");


	Button btn2(100, 300, 150, 50, glm::vec3(0.0f, 0.5f, 1.0f));
	Button btn3(20, 20, 100, 40, glm::vec3(0.0f, 1.0f, 0.0f));
	Button btn4(200, 100, 120, 120, glm::vec3(1.0f, 1.0f, 0.0f));
	btn2.update(-50, 0);
	btn2.add(btn3);
	app.root->add(btn2);
	app.root->add(btn4);

	app.exec_();

	std::cin.get();
	return 0;
}
