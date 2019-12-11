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
#include <vector>

// settings
static unsigned int SCR_WIDTH = 800;
static unsigned int SCR_HEIGHT = 600;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//The Origin Coordinates of XY is Top-Left of the screen (0,0) 
//The Origin Coordinates of XY is Top-Left of the screen (0,0) 
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

//FreeTypeLoad
unsigned int generateFont(std::u32string_view string, unsigned int text_size = 20)
{

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	FT_Set_Pixel_Sizes(face, text_size, 0);
	if (FT_Load_Char(face, 'D', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << std::endl;
	//FreeTypeLoad----------End
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	FT_GlyphSlot slot = face->glyph;
	if (FT_Load_Char(face, 'A', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";

	std::uint16_t dims = glm::ceil(glm::sqrt((float)string.size()));


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, text_size * dims , text_size * dims, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	
	for (std::uint32_t i = 0; i < string.size();++i)
	{
		if (FT_Load_Char(face, string[i], FT_LOAD_RENDER))
			std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
		glTexSubImage2D(GL_TEXTURE_2D, 0, text_size * glm::mod((float)i, (float)dims) , text_size * (i / dims), slot->bitmap.width, slot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

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

	{   double xPos, yPos;
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

	Shader shader("shader/text/vertex.vs","shader/text/fragment.fs");
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float step = 0.985f / 4.0f;
	glm::vec2 pos(3, 2);
	float vertices[] = {
		10.0f, 10.0f, pos.x * step, pos.y * step,
		60.0f, 10.0f, (pos.x + 1.0f) * step, pos.y * step,
		10.0f, 60.0f, pos.x * step, (pos.y + 1.0f) * step,
		60.0f, 60.0f, (pos.x + 1.0f) * step, (pos.y + 1.0f)*step
	};

	//float vertices[] = {
	//10.0f, 10.0f, 0.0f, 0.0f,
	//160.0f, 10.0f, 1.0f , 0.0f,
	//10.0f, 160.0f, 0.0f, 1.0f,
	//160.0f, 160.0f, 1.0f, 1.0f
	//};


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int texture = generateFont(U"aăâgcdđeêfOÔ");


	while (!glfwWindowShouldClose(window))
	{
		// input
		glfwPollEvents();
		processInput(window);
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
		shader.use();
		shader.setVec2("wDim", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
		shader.setVec3("color", glm::vec3(0.5f,0.0f,0.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader.setInt("text", 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//Draw2D----------End

		glfwSwapBuffers(window);
	}
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