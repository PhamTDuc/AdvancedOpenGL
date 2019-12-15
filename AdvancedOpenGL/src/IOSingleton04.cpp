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
#include "Widget.h"
#include "TextRenderer.h"
#include "ShapeRenderer.h"


class Application
{
private:
	struct Mouse
	{
		double firstX, firstY;
		double x, y;
		bool LeftB, MiddleB, RightB;
	};

	enum MOUSE_STATUS{FREE_MOUSE=0, BUSY_MOUSE};

public:
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static Mouse MouseEvent;
	static MOUSE_STATUS mouse_status;
	static GLFWwindow* window;
	static Widget* hot;
	static Widget* active;
	static std::unique_ptr<Widget> root;
	//static TextRenderer textrenderer;
	static Shader ShapeShader;
	static Shader TextShader;
	static unsigned int texture;
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

		root = std::make_unique<Widget>(0,0,SCR_WIDTH,SCR_HEIGHT,glm::vec3(0.0f,0.5f,0.5f));

		//For render 2D Rectangle
		//For render 2D Rectangle
		ShapeShader = Shader("shader/button/vertex.vs", "shader/button/fragment.fs");
		ShapeShader.use();
		ShapeShader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);
		
		

		//For render 2D Text
		//For render 2D Text
		TextShader = Shader("shader/text/vertex.vs", "shader/text/fragment.fs");
		TextShader.use();
		TextShader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);

		TextRenderer::createContext();
		ShapeRenderer::createContext();

		Widget::textrenderer.generateFont(U"\u2301abcdefghijklmnopqrstuvwxyzếớ ", 15);
		//textrenderer.generateFont(U"\u2301abcdefghijklmnopqrstuvwxyzếớ ", 40);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		root->w_m = width;
		root->h_m = height;
		SCR_WIDTH = width;
		SCR_HEIGHT = height;
		ShapeShader.use();
		ShapeShader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);

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


	static bool isOver(Widget* node = nullptr)
	{
		return (node->x_m < MouseEvent.x && MouseEvent.x < (node->x_m + node->w_m) && node->y_m < MouseEvent.y && MouseEvent.y < (node->y_m + node->h_m));
	}

	static void tranversal(Widget* node = nullptr)
	{
		if (mouse_status == FREE_MOUSE && node)
		{
			if (isOver(node))
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
			TextShader.use();
			TextShader.setVec2("wDim", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
			TextShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));

			root->drawAll(TextShader,2.0f);
			//textrenderer.renderText(U"hello thế giới\nhello", TextShader,1.5,50);
			//Render2D ----- End


			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwSwapInterval(1);
		}

	}
};

unsigned int Application::SCR_WIDTH;
unsigned int Application::SCR_HEIGHT;
Application::Mouse Application::MouseEvent;
Application::MOUSE_STATUS Application::mouse_status;

GLFWwindow* Application::window;
Widget* Application::hot = nullptr;
Widget* Application::active = nullptr;
std::unique_ptr<Widget> Application::root;
Shader Application::TextShader;
Shader Application::ShapeShader;
unsigned int Application::texture;


struct Button :public Widget
{
	Button(int x = 0, int y = 0, int w = 0, int h = 0,glm::vec3 &color=glm::vec3(0.5f),Widget* parent = nullptr, std::u32string_view name = U"") : Widget(x, y, w, h,color,parent,name)
	{
		if (parent)
		{ 
			parent->children.emplace_back(this);
			x_m += parent->x_m;
			y_m += parent->y_m;
		}
	}
};


int main()
{
	Application app;
	app.SCR_WIDTH = 400;
	app.SCR_HEIGHT = 600;
	app.setWindow("Hello the world");


	//Button btn(30, 0, 100, 40, glm::vec3(0.5f, 0.0f, 0.0f), app.root.get());
	Button btn2(0, 0, 100, 40, glm::vec3(1.0f, 0.0f, 0.0f),app.root.get(),U"xin chao");
	//Button btn3(20, 20, 100, 40, glm::vec3(0.0f, 1.0f, 0.0f), &btn2,U"the gioi");
	//Button btn4(80, 20, 100, 20, glm::vec3(0.0f, 0.0f, 1.0f), &btn3,U"xin\nchao\nthe\ngioi\nmoi");
	//glm::vec2 vh = app.textrenderer.getVHBBox(U"hello thế giới\nhello",1.5);
	//Button btn5(50, 0, vh.x, vh.y, glm::vec3(0.0f, 0.0f, 1.0f), app.root.get());
	//btn2.update(100, 100);

	app.exec_();

	std::cin.get();
	return 0;
}
