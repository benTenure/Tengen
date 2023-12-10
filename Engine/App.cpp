#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>
#include <stb/stb_image.h>
#include <stdio.h>

#include "Shader.h"
#include "Camera.h"
#include "Window.h"


/*
	[Note: Only until Quaternions happen (:<]
	Do transformations in THIS ORDER:
	1. Scale
	2. Rotate
	3. Translate
	 
	which translates to: Transalte * Rotate * Scale (right to left)
*/

/*
	RenderContext:
	- Init
	- Pre-render?
	- Post-render?
*/

/*
	Input:
	- GLFW?
	- might be something better
	- dependent on the backend I choose for windows
*/

/*
	ImGUI:
	- running into a problem where the UI is rendering behind my texture
	- 2 possible solutions:
	-> Render UI last so it's always on top. Guaranteed to work but expensive and wasted operations since it's not taken into consideration for occlusion culling (future feature)
	-> save a depth only reserved for the UI. This would require a depth check and may even work with occlusion culling! But how much work would this take... definitely for the future
*/

static std::string APP_DEFAULT_PROJECT_NAME = "Tengen 0";

static glm::mat4 transform = glm::mat4(1.0f);

static float deltaTime = 0.0f;	// Time between current frame and last frame
static float lastFrame = 0.0f; // Time of last frame

static float fov = 45.0f;

Camera s_mainCamera;

struct Color
{
public:
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float r, g, b, a;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	s_mainCamera.ProcessMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Set up a sub/pub relationship for things that want input? Might be crazy to go through
	s_mainCamera.ProcessScrollWheel(xoffset, yoffset);
}

void MoveRight()
{
	transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f));
}

void MoveLeft()
{
	transform = glm::translate(transform, glm::vec3(-0.1f, 0.0f, 0.0f));
}

void processInput(GLFWwindow* window)
{
	int pressedKey = glfwGetKey(window, GLFW_KEY_ESCAPE);

	// This feels like it should be a switch-statement... I really should try to do that
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		MoveRight();
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		MoveLeft();
	}
}

int main()
{
	Window window(APP_DEFAULT_PROJECT_NAME);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//glm::vec3 clear_color = glm::vec3(0.45f, 0.55f, 0.6f);
	glm::vec3 clear_color = glm::vec3(0.f);

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindowHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330"); // This is for OPENGL 3.2+ and GLSL 150+

	glViewport(0, 0, Window::WINDOW_DEFAULT_WIDTH, Window::WINDOW_DEFAULT_HEIGHT);

	// We register the callback functions after we've created the window and before the render loop is initiated.
	// This will handle resizing the viewport when the user resizes the window
	glfwSetFramebufferSizeCallback(window.GetWindowHandle(), framebuffer_size_callback);

	// Register the callback function for keeping track of the mouse pointer
	glfwSetCursorPosCallback(window.GetWindowHandle(), mouse_callback);

	glfwSetScrollCallback(window.GetWindowHandle(), scroll_callback);

	// Keep the cursor constrained to the window and invisible
	glfwSetInputMode(window.GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Configure global state of GL
	glEnable(GL_DEPTH_TEST);

	Shader defaultShader("Shaders/default.vert", "Shaders/default.frag");
	Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

	// We'll use one cube for both the box and light for now
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(VAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// TODO: Put this in an actual function for OpenGL context. It could actually be helpful
	// Ping the GPU for the max number of vertex attributes it can support
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum num of vertex attributes supported: " << nrAttributes << std::endl;
	
	// Creating a texture
	if (false)
	{
		unsigned int texture1, texture2;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
	
		stbi_set_flip_vertically_on_load(true);

		int width, height, numChannels;
		unsigned char* imageData = stbi_load("Resources/Images/container.jpg", &width, &height, &numChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		stbi_image_free(imageData);

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		imageData = stbi_load("Resources/Images/awesomeface.png", &width, &height, &numChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	// Camera Stuff (Don't keep a global camera please... for the love of God...
	//Camera mainCamera;
	
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	// Main loop
	while (!window.CloseWindow())
	{
		// Time check/update
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Future game loop order:
		// -----------------------
		// 1) User Input
		// 2) Simulation
		// 2) Physics
		// 3) Rendering
		// 4) Draw call

		// Input
		window.ProcessInput();
		s_mainCamera.ProcessInput(window, deltaTime);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::SliderFloat("FOV", &fov, 45.0f, 115.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Render();
		
		//Color defaultColor(0.3f, 0.396f, 0.518f, 1.0f); // Nice gray-blue color background, RGB = 77, 101, 132
		Color defaultColor(0.f, 0.f, 0.3f, 1.0f);
		glClearColor(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a); // state-setting
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using

		s_mainCamera.SetFOV(fov);

		// Draw regular cube
		defaultShader.Use();
		defaultShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		defaultShader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		// Coordinate Stuff (OpenGL uses a right-handed coordinate system, +x to the right, +y up, +z towards me)
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = s_mainCamera.Update();
		glm::mat4 projection = glm::perspective(glm::radians(s_mainCamera.GetFOV()), static_cast<float>(Window::WINDOW_DEFAULT_WIDTH) / static_cast<float>(Window::WINDOW_DEFAULT_HEIGHT), 0.1f, 100.0f);
		defaultShader.SetMat4("model", model);
		defaultShader.SetMat4("view", view);
		defaultShader.SetMat4("projection", projection);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Light cube placement and draw call
		lightShader.Use();

		// set the model, view and projection matrix uniforms
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		// I need 2 different shaders...
		lightShader.SetMat4("model", model);
		lightShader.SetMat4("view", view);
		lightShader.SetMat4("projection", projection);

		// draw the light cube object
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGUI.SwapBuffers equivalent
		window.SwapBuffers();
	}

	// IMGUI cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}