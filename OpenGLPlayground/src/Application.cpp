#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Initialize glew */
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW ERROR! Glew is not ok..." << std::endl;

	/* Log openGL version */
	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Generate vertex buffer */
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Create renderer
		Renderer renderer;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

		while (!glfwWindowShouldClose(window))									/// Loop until the user closes the window
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();													// Clear screen

			ImGui_ImplOpenGL3_NewFrame();										// ImGui new frame
			ImGui_ImplGlfw_NewFrame();											// X
			ImGui::NewFrame();													// X

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Title");
				if (currentTest != testMenu)
				{
					if (ImGui::Button("<-"))
					{
						delete currentTest;
						currentTest = testMenu;
					}
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();													// ImGUI draw
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());				// X
			
			glfwSwapBuffers(window);											// Swap front and back buffers
			glfwPollEvents();													// Poll for and process events
		}
		// Test cleanup
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	
	

	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// OpenGL/GLFW cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}