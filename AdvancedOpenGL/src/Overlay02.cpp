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
#include <map>
struct Character
{
	GLuint texture2D;
	glm::ivec2 size;
	glm::ivec2 bearing;
	FT_Pos advance;
};

struct CoreMouse
{
	double x, y; //Current Mouse Position
	double firstX, firstY; //First Pressed Occured
	bool leftB, midB, rightB;
	friend std::ostream& operator<<(std::ostream& out, const CoreMouse& mouse)
	{
		out << "First" << mouse.firstX << " " << mouse.firstY << " L" << mouse.leftB << " R" << mouse.rightB << " M" << mouse.midB << " Current " << mouse.x << " " << mouse.y << "\n";
		return out;
	}
};
static CoreMouse mouse;

void draw2D(GLFWwindow* window,Shader &shader,double x,double y, double w, double h)
{
	glm::ivec2 wDim;
	glfwGetWindowSize(window, &wDim.x, &wDim.y);
	shader.use();
	shader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setVec2("wDim", wDim);
	float vertices[] =
	{
		x, y,
		x + w, y,
		x, y + h,
		x + w, y + h,
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

void renderText(GLFWwindow* window,std::map<GLchar,Character> &characters, const std::string& string, Shader& shader, int xPos, int yPos,double scale=0.5)
{
	glm::ivec2 wDim;
	glfwGetWindowSize(window, &wDim.x, &wDim.y);
	shader.use();
	shader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setVec2("wDim", wDim);
	shader.setInt("text", 0);
	for (const unsigned int c : string)
	{
		Character ch{ characters[c] };
		int x= xPos+ch.bearing.x*scale;
		int y =yPos + ch.size.y * scale-ch.bearing.y*scale;
		float vertices[] =
		{
			x, y, 0.0f,1.0f,
			x + ch.size.x*scale, y,1.0f,1.0f,
			x, y - ch.size.y*scale,0.0f,0.0f,
			x + ch.size.x*scale, y -ch.size.y*scale,1.0f,0.0f
		};

		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),nullptr);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.texture2D);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		xPos += (ch.advance>>6)*scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
	{
		//std::cout << "Xpos: " << x << "  Ypos: " << y << "\n";
		mouse.x = x;
		mouse.y = y;
		std::cout << mouse;
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)

	{	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	//std::cout << "X" << xPos << " Y" << yPos;
	if (action == GLFW_PRESS)
	{
		if (!(mouse.leftB || mouse.rightB || mouse.midB))
		{
			mouse.firstX = xPos;
			mouse.firstY = yPos;
		}
		//std::cout << " Mouse button: " << button << " pressed\n";
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouse.leftB = true;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouse.rightB = true;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			mouse.midB = true;
			break;
		}

	}
	else
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouse.leftB = false;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			mouse.rightB = false;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			mouse.midB = false;
			break;
		}
		//std::cout << " Mouse button: " << button << " released\n";
	}
	std::cout << mouse;
	});

	

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//FreeType
	FT_Library ft;
	if(FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	FT_Face face;
	if (FT_New_Face(ft, "fonts/BKANT.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	FT_Set_Pixel_Sizes(face, 48,0);
	if (FT_Load_Char(face,'D', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << std::endl;

	

	std::map<GLchar, Character> characters;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	Shader textShader("shader/text/vertex.vs", "shader/text/fragment.fs");
	//FreeType----------End

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.6f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Draw2D
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		Shader shader("shader/2d/vertex.vs", "shader/2d/fragment.fs");
		draw2D(window,shader, 10, 40,100, 30);
		renderText(window, characters, "Heggg The World", textShader, 200, 80,1);
		//Draw2D---End












		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
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
	glViewport(0, 0, width, height);
}