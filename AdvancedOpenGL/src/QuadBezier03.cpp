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
class QuadCurve
{
private:
	std::vector<glm::vec2> m_points;
	unsigned int VAO, VBO;
public:
	using iterator = std::vector<glm::vec2>::iterator;
	using citerator = std::vector<glm::vec2>::const_iterator;
	using size_type = std::vector<glm::vec2>::size_type;

	QuadCurve(const std::vector<glm::vec2> &points)
	{
		m_points.insert(m_points.end(), points.begin(), points.end());
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec2), &m_points[0],GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void updateBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(glm::vec2), &m_points[0], GL_DYNAMIC_DRAW);		
	}

	void update(unsigned int index,const glm::vec2 &val)
	{
		m_points[index] = val;
		updateBuffer();
	}

	void push_back(const glm::vec2& val)
	{
		m_points.push_back(val);
		updateBuffer();
	}

	void push_back(glm::vec2&& val)
	{
		m_points.push_back(val);
		updateBuffer();
	}

	void pop_back()
	{
		m_points.pop_back();
		updateBuffer();
	}

	template<typename InputIterator>
	iterator insert(citerator position, InputIterator first, InputIterator last)
	{
		m_points.insert(position, first, last);
		updateBuffer();
	}

	iterator erase(citerator first, citerator last)
	{
		m_points.erase(first, last);
		updateBuffer();
	}

	void clear() noexcept
	{
		m_points.clear();
		updateBuffer();
	}

	void reserve(size_type n)
	{
		m_points.reserve(n);
	}

	void draw(Shader &shader,GLenum mode)
	{

		glBindVertexArray(VAO);
		glDrawArrays(mode, 0, m_points.size());
		glBindVertexArray(0);
	}

};

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

	Shader quadBezierShader("shader/quadBezier/vertex.vs", "shader/quadBezier/fragment.fs", "shader/quadBezier/geo.gs");
	//Shader quadBezierShader("shader/quadBezier/vertex.vs", "shader/quadBezier/fragment.fs");
	quadBezierShader.use();
	quadBezierShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
	quadBezierShader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);


	std::vector<glm::vec2> points{ {0,100},{40,10},{60,90},{40,10},{60,90},{200,200}};
	QuadCurve quadcurve(points);


	while(!glfwWindowShouldClose(window))
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
		quadBezierShader.use();
		quadBezierShader.setVec2("wDim", SCR_WIDTH, SCR_HEIGHT);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		quadcurve.draw(quadBezierShader, GL_TRIANGLES);
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