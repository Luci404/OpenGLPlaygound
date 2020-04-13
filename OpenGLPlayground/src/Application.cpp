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
	GLFWwindow* window;																	// Create a window ptr
		
	if (!glfwInit())																	// Initialize GLFW														
		return -1;																		// If initialization failed, end application

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);										// Set the major version of OpenGL to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);										// Set the minor version of OpenGL to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);						// Use OpenGL Core
			
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);						// Create window.
	if (!window)																		// If the window did not get created propperly
	{
		glfwTerminate();																// Terminate GLFW
		return -1;																		// End application
	}

	glfwMakeContextCurrent(window);														// Set window.
	glfwSwapInterval(1);																// Set swap interval

	if (glewInit() != GLEW_OK)															// Initialize glew 
		std::cout << "GLEW ERROR! Glew is not ok..." << std::endl;						// If not initialize correctly: log error.

	std::cout << glGetString(GL_VERSION) << std::endl;									// Log openGL version 

	{
		GLCall(glEnable(GL_BLEND));														// Enable blending	
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));						// Set blending function

		Renderer renderer;																// Create a new renderer

		IMGUI_CHECKVERSION();													
		ImGui::CreateContext();															// Create ImGui context
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();														// Set ImGui color theme

		ImGui_ImplGlfw_InitForOpenGL(window, true);										// Init ImGui for OpenGL
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

		test::Test* currentTest = nullptr;												// Create a pointer to the currentTest
		test::TestMenu* testMenu = new test::TestMenu(currentTest);						// Create a pointer to the testMenu
		currentTest = testMenu;															// Set the currentTest to be the testMenu

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");					// Register 'TestClearColor' to the testMenu
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");						// Register 'TestTexture2D' to the testMenu

		while (!glfwWindowShouldClose(window))											// Loop until the user closes the window
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));								// Set the clear color
			renderer.Clear();															// Clear screen

			ImGui_ImplOpenGL3_NewFrame();												// ImGui new frame
			ImGui_ImplGlfw_NewFrame();													// ImGui new frame
			ImGui::NewFrame();															// ImGui new frame

			if (currentTest)															// If there is a currentTest, call it's functions
			{
				currentTest->OnUpdate(0.0f);											// Call the currentTest's OnUpdate function
				currentTest->OnRender();												// Call the currentTest's OnRender function
				ImGui::Begin("Title");													// Start new ImGui window
				if (currentTest != testMenu)											// If the currentTest is not the test menu
				{
					if (ImGui::Button("<-"))											// Create a back button
					{																	
						delete currentTest;												// Delete the currentTest
						currentTest = testMenu;											// Set the currentTest to be the testMenu
					}
				}
				currentTest->OnImGuiRender();											// Call the currentTest's OnImGuiRender function
				ImGui::End();															// End (unbind) ImGui window
			}

			ImGui::Render();															// ImGUI draw
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());						// ImGUI draw
			
			glfwSwapBuffers(window);													// Swap front and back buffers
			glfwPollEvents();															// Poll for and process events
		}
		delete currentTest;																// Delete the current test
		if (currentTest != testMenu)													// Delete the testMenu, if currentTest is something else
			delete testMenu;															// Delete the testMenu
	}
	
	ImGui_ImplOpenGL3_Shutdown();														// ImGui cleanup OpenGL bindings
	ImGui_ImplGlfw_Shutdown();															// ImGui cleanup GLFW bindings
	ImGui::DestroyContext();															// ImGui cleanup the context

	glfwDestroyWindow(window);															// GLFW cleanup window
	glfwTerminate();																	// Terminate the GLFW context

	return 0;																			// End the application
}