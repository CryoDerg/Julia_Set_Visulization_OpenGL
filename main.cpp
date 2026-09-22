/*
	Reference here for more info:
	https://learnopengl.com/Getting-started/Hello-Triangle
*/

#include <iostream>
#include <math.h>
#include <random>

#include "shader.h"
#include "meshSurface.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

float runtime = 0.0f;

float vertices[] = {
     1.0f,  1.0f, 0.0f, // top right
     1.0f, -1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f, // top left 
};
float scaledVertices[] = {
    -0.5f, -0.5f, 0.0f, // top right
    -0.5f, -1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, -0.5f, 0.0f, // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

int width, height;
double mousePos[2];
double offset[2] = {0,1.77};
double zoom = 0.25;
int steps = 100;
float animTime = 0.0f;
bool animPlaying = true;

bool spaceDown = false;


/*
double* complexPow(double* base, double ex, double* resultPtr) {
	if (ex == 0.0) {
		resultPtr[0] = 1.0;
		resultPtr[1] = 0.0;
		return resultPtr;
	}

	double length = sqrt(pow(base[0], 2) + pow(base[1], 2));
	if (length == 0.0) {
		resultPtr[0] = 0.0;
		resultPtr[1] = 0.0;
		return resultPtr;
	}

	double theta = std::atan2(base[1], base[0]);
	double rp = pow(length, ex);
	double angle = ex * theta;

	resultPtr[0] = rp * cos(angle);
	resultPtr[1] = rp * sin(angle);
	return resultPtr;
}

int mandlebrot(int steps, double* Z, double* C) {
	double length = sqrt(pow(Z[0], 2) + pow(Z[1], 2));
	if (length > 2 || steps == 0) {
		return steps;
	}
	
	double next[2];
	complexPow(Z, 2.0, next);
	next[0] += C[0];
	next[1] += C[1];
	return mandlebrot(steps - 1, next, C);
}
*/

void processInput(GLFWwindow *window) {
   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

	if (!spaceDown && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		animPlaying = not animPlaying;
		spaceDown = true;
	}
		
	if (spaceDown && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		spaceDown = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		animTime -= 0.1;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		animTime += 0.1;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		int s = 50;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			s = 1;
		steps += s * yoffset;
		if (steps < 0) steps = 0;
		std::cout << steps << "\n";
	}
	else {
		std::cout << yoffset << "\n";
		zoom += yoffset * (zoom/10);
		std::cout << zoom << "\n";
		//move toward mouse
		/*
		float mx, my;
		mx = 2.0 * (mousePos[0] / width) - 0.5;
		my = 2.0 * (mousePos[1] / width) - 0.5;
		offset[0] += yoffset * (zoom/10);
		offset[1] += yoffset * (zoom/10);
		std::cout << "Offset: (" << offset[0] << ", " << offset[1] << ")\n";
		*/
	}	
}






int main() {
	std::random_device rd;  // a seed source for the random number engine
   std::mt19937 gen(rd());

	// Init glfw
	glfwInit();
	
	// Create window and init glad
	GLFWwindow* win = glfwCreateWindow(1920, 1080, "TEST", glfwGetPrimaryMonitor(), NULL);
	glfwMakeContextCurrent(win); // Set win to be the current context
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetScrollCallback(win, scroll_callback);

	gladLoadGL(glfwGetProcAddress);
	
	glfwSwapInterval(1); // Let the screen update before updating again

   glfwGetFramebufferSize(win, &width, &height);

	// Link the shaders into a program object that can be used by OpenGL
	Shader sp_sMandle("../Shaders/vertexShader.vert", "../Shaders/mandlebrotShader.frag");
	Shader sp_sJulia("../Shaders/vertexShader.vert", "../Shaders/juliaShader.frag");

	Mesh screen;
	screen.setVertices(vertices, sizeof(vertices) / sizeof(float), GL_STATIC_DRAW);
	screen.setIndices(indices, sizeof(indices) / sizeof(int), GL_STATIC_DRAW);
	screen.addAttribute(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	Mesh subScreen;
	subScreen.setVertices(scaledVertices, sizeof(scaledVertices) / sizeof(float), GL_STATIC_DRAW);
	subScreen.setIndices(indices, sizeof(indices) / sizeof(int), GL_STATIC_DRAW);
	subScreen.addAttribute(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	

	// Get uniforms in shaders
	int uniform_julia_offset = sp_sJulia.getUniform("offset");
	int uniform_julia_zoom = sp_sJulia.getUniform("zoom");
	int uniform_julia_steps = sp_sJulia.getUniform("steps");
	int uniform_julia_time = sp_sJulia.getUniform("time");

	int uniform_mandle_offset = sp_sMandle.getUniform("offset");
	int uniform_mandle_zoom = sp_sMandle.getUniform("zoom");
	int uniform_mandle_steps = sp_sMandle.getUniform("steps");
	int uniform_mandle_time = sp_sMandle.getUniform("time");
	sp_sMandle.use();
	glUniform1i(uniform_mandle_steps, 5);
	glUniform2f(uniform_mandle_offset, 6.0, 6.8);
	glUniform1f(uniform_mandle_zoom, 1/8.0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw primitives with only their wireframe

	//glBindVertexArray(screen.getVertexArray());
	// Rendering Loop
	while (!glfwWindowShouldClose(win)) {
		// Get the current width and height of the window
      glfwGetFramebufferSize(win, &width, &height);
      const float ratio = width / (float) height;

		glViewport(0, 0, width, width);

		float timeValue = glfwGetTime();
		float dt = timeValue - runtime;
		runtime = timeValue;
		if (1/dt < 20.0f) {
			std::cout << "High Load Detected at T+" << timeValue << "\nFPS : " << 1 / dt << "\nFrametime : " << dt << "\n\n";
		}

		if (animPlaying)
			animTime += dt;

		processInput(win);
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);
		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			offset[0] -= 2 * (xpos - mousePos[0])/(zoom * width);
			offset[1] += 2 * (ypos - mousePos[1])/(zoom * width);
			std::cout << xpos - mousePos[0] << ", " << ypos - mousePos[1] << "\n";
		}

		if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			steps += 1;
		}
		mousePos[0] = xpos;
		mousePos[1] = ypos;

		// Clear the Color Buffer to wipe the screen
		glClear(GL_COLOR_BUFFER_BIT);
		//vertices[0] = 0.5f;
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		sp_sJulia.use();
		
		glUniform1f(uniform_julia_time, animTime / 10.0);
		glUniform2f(uniform_julia_offset, float(offset[0]), float(offset[1]));
		glUniform1f(uniform_julia_zoom, zoom);
		glUniform1i(uniform_julia_steps, steps);

		screen.draw();

		sp_sMandle.use();
		
		//glUniform2f(uniform_mandle_offset, float(offset[0]), float(offset[1]));
		//glUniform1f(uniform_mandle_zoom, zoom);
		glUniform1i(uniform_mandle_steps, steps);
		glUniform1f(uniform_mandle_time, animTime / 10.0);

		//subScreen.draw();

		glfwSwapBuffers(win); // Swap the back render buffer with the front buffer to display the render.
    	glfwPollEvents(); // Process any incoming events 
	}

	glfwTerminate(); // Close GLFW
}