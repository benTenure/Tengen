#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdio.h>

#include "App.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"
#include <vector>


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

static glm::mat4 g_transform = glm::mat4(1.0f);

static float g_deltaTime = 0.0f;	// Time between current frame and last frame
static float g_lastFrame = 0.0f; // Time of last frame

static float g_fov = 45.0f;

// Mouse input variables (hate this :seething:)
static float g_lastX = Window::WINDOW_DEFAULT_WIDTH / 2.0f;
static float g_lastY = Window::WINDOW_DEFAULT_HEIGHT / 2.0f;
static bool g_firstMouse = true;

Camera g_mainCamera;

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
	float xposf = static_cast<float>(xpos);
	float yposf = static_cast<float>(ypos);

	if (g_firstMouse) // initially set to true. Still needed???
	{
		g_lastX = xposf;
		g_lastY = yposf;
		g_firstMouse = false;
	}

	float xoffset = xposf - g_lastX;
	float yoffset = g_lastY - yposf; // reversed since y-coordinates range from bottom to top
	g_lastX = xposf;
	g_lastY = yposf;

	g_mainCamera.ProcessMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Set up a sub/pub relationship for things that want input? Might be crazy to go through
	g_mainCamera.ProcessScrollWheel(xoffset, yoffset);
}

void MoveRight()
{
	g_transform = glm::translate(g_transform, glm::vec3(0.1f, 0.0f, 0.0f));
}

void MoveLeft()
{
	g_transform = glm::translate(g_transform, glm::vec3(-0.1f, 0.0f, 0.0f));
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int LoadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_NONE;

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

/*
	I tried putting this logic into a function so I could use it again later but it doesn't work?
	Maybe leaving the context of the shader makes it so this function doesn't know that the 
	defaultShader is being used
*/
void DrawManyCubes(const std::vector<glm::vec3> &cubePositions, Shader defaultShader, unsigned int VAO)
{
	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < cubePositions.size(); i++)
	{
		defaultShader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		defaultShader.SetMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
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

	//Shader defaultShader("Shaders/default.vert", "Shaders/default.frag");
	Shader defaultShader("Shaders/simple.vert", "Shaders/simple.frag");
	Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

	// We'll use one cube for both the box and light for now
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// TODO: Put this in an actual function for OpenGL context. It could actually be helpful
	// Ping the GPU for the max number of vertex attributes it can support
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum num of vertex attributes supported: " << nrAttributes << std::endl;
	
	// Creating a texture
	std::string diffusePath = "Resources/Images/container2.png";
	unsigned int diffuseMap = LoadTexture(diffusePath.c_str());
	
	std::string specularPath = "Resources/Images/container2_specular.png";
	unsigned int specularMap = LoadTexture(specularPath.c_str());
	
	//std::string emissionPath = "Resources/Images/matrix.jpg";
	//unsigned int emissionMap = LoadTexture(emissionPath.c_str());

	// Camera Stuff (Don't keep a global camera please... for the love of God...
	//Camera mainCamera;
	
	glm::vec3 lightPos(0.0f, 2.0f, 0.0f);

	//defaultShader.Use();
	//defaultShader.SetInt("material.diffuse", 0);
	//defaultShader.SetInt("material.specular", 1);
	//defaultShader.SetInt("material.emission", 2);

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	std::string path = "Resources/Models/backpack/backpack.obj";
	Model backpack(path.c_str());

	// Main loop
	while (!window.CloseWindow())
	{
		// Time check/update
		float currentFrame = static_cast<float>(glfwGetTime());
		g_deltaTime = currentFrame - g_lastFrame;
		g_lastFrame = currentFrame;

		// Future game loop order:
		// -----------------------
		// 1) User Input
		// 2) Simulation
		// 2) Physics
		// 3) Rendering
		// 4) Draw call

		// Input
		window.ProcessInput();
		g_mainCamera.ProcessInput(window, g_deltaTime);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (false)
		{
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::SliderFloat("FOV", &g_fov, 45.0f, 115.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
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

		g_mainCamera.SetFOV(g_fov);

		// Draw regular cube
		defaultShader.Use();

		// Coordinate Stuff (OpenGL uses a right-handed coordinate system, +x to the right, +y up, +z towards me)
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = g_mainCamera.Update();
		glm::mat4 projection = glm::perspective(glm::radians(g_mainCamera.GetFOV()), static_cast<float>(Window::WINDOW_DEFAULT_WIDTH) / static_cast<float>(Window::WINDOW_DEFAULT_HEIGHT), 0.1f, 100.0f);
		defaultShader.SetMat4("model", model);
		defaultShader.SetMat4("view", view);
		defaultShader.SetMat4("projection", projection);
		//defaultShader.SetVec3("viewPos", g_mainCamera.GetPosition());

		//defaultShader.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		//defaultShader.SetFloat("material.shininess", 320.0f);

		glm::vec3 ambient(0.2f, 0.2f, 0.2f);
		glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
		glm::vec3 specular(1.0f, 1.0f, 1.0f);

		// Directional Light
		//defaultShader.SetVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		//defaultShader.SetVec3("dirLight.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
		//defaultShader.SetVec3("dirLight.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		//defaultShader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		// Point Light
		//defaultShader.SetVec3("pointLights[0].position", pointLightPositions[0]);
		//defaultShader.SetFloat("pointLights[0].constant", 1.0f);
		//defaultShader.SetFloat("pointLights[0].linear", 0.09f);
		//defaultShader.SetFloat("pointLights[0].quadratic", 0.032f);
		//defaultShader.SetVec3("pointLights[0].ambient", ambient);
		//defaultShader.SetVec3("pointLights[0].diffuse", diffuse);
		//defaultShader.SetVec3("pointLights[0].specular", specular);

		// Flashlight (SpotLight)
		//defaultShader.SetVec3("spotLight.position", g_mainCamera.GetPosition());
		//defaultShader.SetVec3("spotLight.direction", g_mainCamera.GetFront());
		//defaultShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//defaultShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		//defaultShader.SetFloat("spotLight.constant", 1.0f);
		//defaultShader.SetFloat("spotLight.linear", 0.09f);
		//defaultShader.SetFloat("spotLight.quadratic", 0.032f);
		//defaultShader.SetVec3("spotLight.ambient", ambient);
		//defaultShader.SetVec3("spotLight.diffuse", diffuse);
		//defaultShader.SetVec3("spotLight.specular", specular);

		backpack.Draw(defaultShader);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);*/
		
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMap);

		//DrawManyCubes(cubePositions, defaultShader, VAO);
		
		if (false)
		{
			glBindVertexArray(VAO);

			for (unsigned int i = 0; i < cubePositions.size(); i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				defaultShader.SetMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw light cube (not changing?)
		if (false)
		{
			// Light cube placement and draw call
			lightShader.Use();

			for (int i = 0; i < 4; i++)
			{
				// set the model, view and projection matrix uniforms
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f));

				// I need 2 different shaders...
				lightShader.SetMat4("model", model);
				lightShader.SetMat4("view", view);
				lightShader.SetMat4("projection", projection);

				// draw the light cube object
				glBindVertexArray(lightVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

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