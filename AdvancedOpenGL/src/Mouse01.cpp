#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct CoreMouse
{
	double x, y; //Current Mouse Position
	double firstX, firstY; //First Pressed Occured
	bool leftB, midB, rightB;
	friend std::ostream& operator<<(std::ostream& out,const CoreMouse& mouse)
	{
		out << "First" << mouse.firstX << " " << mouse.firstY << " L" << mouse.leftB << " R" << mouse.rightB << " M" << mouse.midB << " Current " << mouse.x << " " << mouse.y << "\n";
		return out;
	}
};

static CoreMouse mouse;

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

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.6f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		











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