#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "Shader.h"
#include <iostream>
#include <string_view>
#include <forward_list>

// settings
static unsigned int SCR_WIDTH = 800;
static unsigned int SCR_HEIGHT = 600;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void window_refresh_callback(GLFWwindow* window);
void draw2D(Shader& shader, const glm::vec3& color , int x, int  y , unsigned int w, unsigned int h);
struct CoreMouse;

struct Button
{
	int x_m, y_m, w_m, h_m;
	bool isdragging = false;
	Shader* shader = nullptr;
	void draw(const glm::vec3& color, const int &diffX = 0, const int &diffY = 0)
	{
		if (shader)
		{
			if (isdragging)
				draw2D(*shader, color, x_m + diffX, y_m + diffY, w_m, h_m);
			else
			{
				draw2D(*shader, color, x_m, y_m, w_m, h_m);
			}
		}

	}


	void update(const int& diffX, const int& diffY)
	{
		x_m += diffX;
		y_m += diffY;
	}
};

struct Widget
{
	int x_m, y_m, w_m, h_m;
	Widget* parent = nullptr;
	std::forward_list<Widget*> children;

	Widget(int x=0,int y=0, int w=0, int h=0,Widget* pa=nullptr):x_m(x),y_m(y),w_m(w),h_m(h),parent(pa){}
	virtual void onDragCallback(GLFWwindow*, double,double,double, double){}
	virtual void onClick(){}
	virtual void onDropCallback(){}
};

struct DragOBJ :public Widget
{
	//static Shader* shader_s;
	//bool draggable = true;
	//double tempX, tempY;
	DragOBJ(int x = 0, int y = 0, int w = 0, int h = 0, Widget* pa = nullptr): Widget(x,y,w,h,pa)
	{
		if (pa)
			pa->children.push_front(this);
		//tempX = x;
		//tempY = y;
	}
	void draw(Shader& shader,const glm::vec3& color)
	{
		draw2D(shader, color, x_m, y_m, w_m, h_m);
	}
	
	virtual void onDragCallback(GLFWwindow* window,double tempX, double tempY,double diffX, double diffY) override
	{
		x_m = tempX + diffX;
		y_m = tempY + diffY;
	}

	virtual void onDropCallback() 
	{
		//x_m = tempX + diffX;
		//y_m = tempY + diffY;
	}

	virtual void onClick()
	{
		//y_m += 50;
		//tempY += 50;
	}
};

//The Origin Coordinates of XY is Top-Left of the screen (0,0) 
//The Origin Coordinates of XY is Top-Left of the screen (0,0) 
Button button{ 100,200,200,80 };
struct CoreMouse
{
	double x, y; //Current Mouse Position
	double firstX, firstY; //First Pressed Occured
	double diffX, diffY;
	double tempX, tempY;
	bool leftB, midB, rightB;
	enum STATUS
	{
		FREE_STATUS = 0,
		PROCESS_STATUS,
	};
	enum STATE
	{
		FIRST_STATE = 0,
		SECOND_STATE

	};

	Widget* active = nullptr;
	STATUS status = FREE_STATUS;
	STATE state = FIRST_STATE;

	bool isOver(const Button& button)
	{
		if (button.x_m  < x && x < (button.x_m + button.w_m ) && button.y_m < y && y < (button.y_m + button.h_m))
			return true;
		return false;
	}

	//void process()
	//{
	//	if (isOver(button) && leftB)
	//	{
	//		//button.update(x - firstX, y - firstY);
	//		//button.draw(glm::vec3(0.0f, 0.0f, 1.0f));
	//		button.isdragging = true;
	//	}
	//	
	//	//dragging = leftB && dragged;
	//	if (button.isdragging && !leftB)
	//	{
	//		button.update(x - firstX, y - firstY);
	//		button.isdragging = false;
	//	}
	//}
	
	Widget* tranversalPostOrder(Widget* node = nullptr, GLFWwindow* window = nullptr)
	{
		if (status == FREE_STATUS && node)
		{
			for (Widget* child : node->children)
			{
				tranversalPostOrder(child);
			}

			if (node->x_m < x && x < (node->x_m + node->w_m) && node->y_m < y && y < (node->y_m + node->h_m))
			{
				//std::cout << "Hello" << node << std::endl;
				if (leftB)
				{
					status = PROCESS_STATUS;
					active = node;
				}
			}
		}
		if (status == PROCESS_STATUS )
		{
			//std::cout << active;
			if (leftB)
			{
				double diffX = x - firstX;
				double diffY = y - firstY;
				if (diffX || diffY){}
					//std::cout << diffX << "   " << diffY<<'\n';
				{
					if (state==FIRST_STATE)
					{
						tempX = node->x_m;
						tempY = node->y_m;
						state = SECOND_STATE;
						//std::cout << tempX << "  " << tempY;
					}
					active->onDragCallback(window,tempX,tempY, diffX, diffY);	
				}
			}else
			{
				//if (status == PROCESS_STATUS)
				//active->onClick();
				//else if(status ==PROCESSING)
				active->onDropCallback();
				active = nullptr;
				status = FREE_STATUS;
				state = FIRST_STATE;
			}
			//std::cout << active;
			//return active;
		}
		
		return nullptr;
	}
};

static CoreMouse mouse;
enum class Align
{ LeftAlign=0, 
  RightAlign,
};

void draw2D(Shader &shader, const glm::vec3& color= glm::vec3(1.0f, 0.0f, 0.0f), int x=0, int  y=0, unsigned int w=100,unsigned int h=20)
//X,Y is Top-Left coordinates in pixels
{
	shader.use();
	shader.setVec3("color", color);
	shader.setVec2("wDim", SCR_WIDTH,SCR_HEIGHT);
	shader.setVec2("transform", glm::vec2(x, y));
	float vertices[] =
	{
		0, 0,
		w, 0,
		0, h,
		w, h,
	};
	
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glBindVertexArray(0);
}


void renderText(const std::u32string_view &str,const FT_Face &face, GLFWwindow* window, Shader& shader,const glm::vec3& color= glm::vec3(1.0f, 0.0f, 0.0f),unsigned int x=0, unsigned int y=0,Align align=Align::LeftAlign)
//X,Y is Bottom-Left coordinates in pixels
//Y = Ytop-left  + textHeight(pixels)
{
	float scale = 0.5;
	static GLuint texture;
	glGenTextures(1, &texture);
	static FT_GlyphSlot slot = face->glyph;
	// Set texture options
	glBindTexture(GL_TEXTURE_2D, texture);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE0);
	shader.use();
	shader.setVec3("color", color);
	shader.setInt("text", 0);

	switch (align)
	{
	case Align::LeftAlign:
		for (int i = 0; i < str.length(); ++i)
		{
			if (FT_Load_Char(face, str[i], FT_LOAD_RENDER))
				std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, slot->bitmap.width, slot->bitmap.rows, 0,
				GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer
			);
			shader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);

			unsigned int w = slot->bitmap.width, h = slot->bitmap.rows;
			float bearX = slot->bitmap_left;
			float bearY = -slot->bitmap_top;

			float vertices[] =
			{
				x + bearX, y + bearY, 0.0f, 0.0f,
				x + bearX + w, y + bearY, 1.0f,0.0f,
				x + bearX, y + bearY + h, 0.0f,1.0f,
				x + bearX + w, y + bearY + h, 1.0f,1.0f
			};

			unsigned int VAO, VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			x += slot->advance.x >> 6;
		}
		break;
	case Align::RightAlign:
		for (int i = str.length() - 1; i >= 0; --i)
		{
			if (FT_Load_Char(face, str[i], FT_LOAD_RENDER))
				std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, slot->bitmap.width, slot->bitmap.rows, 0,
				GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer
			);
			shader.use();
			shader.setVec3("color", color);
			shader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);
			shader.setInt("text", 0);

			unsigned int w = slot->bitmap.width, h = slot->bitmap.rows;
			float bearX = slot->bitmap_left;
			float bearY = -slot->bitmap_top;

			float vertices[] =
			{
				x - bearX, y + bearY, 1.0f, 0.0f,
				x - bearX - w, y + bearY, 0.0f,0.0f,
				x - bearX, y + bearY + h, 1.0f,1.0f,
				x - bearX - w, y + bearY + h, 0.0f,1.0f
			};

			unsigned int VAO, VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			x -= slot->advance.x >> 6;
		}
		break;
	}
	

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


int main()
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

	// glfw window creation
	// --------------------
	
	GLFWwindow* window1= glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window1 == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//GLFWwindow* window2 = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SecondWindow", NULL, window1);
	//if (window2 == NULL)
	//{
	//	std::cout << "Failed to create GLFW window" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}


	GLFWwindow* window = window1;
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowRefreshCallback(window1, nullptr);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
	{
		//std::cout << "Xpos: " << x << "  Ypos: " << y << "\n";
		mouse.x = x;
		mouse.y = y;
		//mouse.diffX = x - mouse.firstX;
		//mouse.diffY = y - mouse.firstY;
		//std::cout << mouse.diffX<<"\n";

		//std::cout << mouse.diffX<<std::endl;
		//mouse.active();
		//std::cout << mouse.x << " " << mouse.y<<"    "<<mouse.diffX<<" "<<mouse.diffY<<"\n";
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)

	{
		
	////std::cout << "X" << xPos << " Y" << yPos;
	//if (action == GLFW_PRESS)
	//{
	//	if (!(mouse.leftB || mouse.rightB || mouse.midB))
	//	{
	//		//mouse.firstX = xPos;
	//		//mouse.firstY = yPos;
	//		//mouse.leftB = true;
	//		glfwGetCursorPos(window, &mouse.firstX, &mouse.firstY);
	//	}
	//	//std::cout << " Mouse button: " << button << " pressed\n";
	//	switch (button)
	//	{
	//	case GLFW_MOUSE_BUTTON_LEFT:
	//		mouse.leftB = true;
	//		break;
	//	case GLFW_MOUSE_BUTTON_RIGHT:
	//		mouse.rightB = true;
	//		break;
	//	case GLFW_MOUSE_BUTTON_MIDDLE:
	//		mouse.midB = true;
	//		break;
	//	}

	//}
	//else
	//{
	//	switch (button)
	//	{
	//	case GLFW_MOUSE_BUTTON_LEFT:
	//		mouse.leftB = false;
	//		break;
	//	case GLFW_MOUSE_BUTTON_RIGHT:
	//		mouse.rightB = false;
	//		break;
	//	case GLFW_MOUSE_BUTTON_MIDDLE:
	//		mouse.midB = false;
	//		break;
	//	}
	//	//std::cout << " Mouse button: " << button << " released\n";
	//	//mouse.leftB = false;
	//	//mouse.active();
	//}
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT && !mouse.leftB)
		{
			glfwGetCursorPos(window, &mouse.firstX, &mouse.firstY);
			mouse.leftB = true;
		}

		if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
		{
			mouse.leftB = false;
		}




	});

	

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//FreeTypeLoad
	FT_Library ft;
	if(FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	FT_Set_Pixel_Sizes(face, 0,24);
	if (FT_Load_Char(face,'D', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << std::endl;
	//FreeTypeLoad----------End

	//Shader
		Shader textShader("shader/text/vertex.vs", "shader/text/fragment.fs");
		Shader shader("shader/2d/vertex.vs", "shader/2d/fragment.fs");
		button.shader = &shader;
	//Shader----------End

	//SetupWidget
		Widget root{};
		DragOBJ drag1{30,30,100,20,&root};
		DragOBJ drag2{30,60,110,25,&root};
		DragOBJ drag3{30,90,100,20,&root};

		std::cout << root.children.front();


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		//glfwPollEvents();
		glfwWaitEvents();
		processInput(window);
		//mouse.process();
		mouse.tranversalPostOrder(&root, window);


		// input----------end

		// render
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.6f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render----------end


		//Draw2D
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//if (button.isdragging && !mouse.leftB)
		//{
		//	button.update(mouse.x - mouse.firstX, mouse.y - mouse.firstY);
		//	button.isdragging = false;
		//}

		button.draw(glm::vec3(0.0f, 0.5f, 1.0f),mouse.x-mouse.firstX,mouse.y-mouse.firstY);
		draw2D(shader,glm::vec3(0.0f,1.0f,0.0f),10, 0,100, 30);
		drag1.draw(shader,glm::vec3(0.5f, 0.5f, 1.0f));
		drag2.draw(shader,glm::vec3(1.0f, 0.5f, 1.0f));
		drag3.draw(shader,glm::vec3(0.5f, 1.0f, 1.0f));
		//std::cout << drag1.x_m<<std::endl;

		//DrawText
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//renderText(U"Xin chào các bạn VN", face, window, textShader,glm::vec3(0.0f,0.0f,1.0f),SCR_WIDTH, 0+24,Align::RightAlign); //24 is textHeight(pixels) and 0 is Ytop-left
		//renderText(U"Xin chào VN", face, window, textShader,glm::vec3(1.0f,0.0f,1.0f),0, 20+24,Align::LeftAlign); //24 is textHeight(pixels) and 20 is Ytop-left
		//DrawText----------End


		//Draw2D----------End












		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwSwapInterval(1);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}
