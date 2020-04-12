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
	{
		std::cout << "GLEW ERROR! Glew is not ok..." << std::endl;
	}

	/* Log openGL version */
	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Generate vertex buffer */
	{
		float positions[16]
		{
			 -50.0f, -50.0f, 0.0f, 0.0f, // 1
			  50.0f, -50.0f, 1.0f, 0.0f, // 2
			  50.0f,  50.0f, 1.0f, 1.0f, // 3
			 -50.0f,  50.0f, 0.0f, 1.0f  // 4
		};

		unsigned int indices[6]
		{
			0, 1, 3,
			1, 2, 3
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;											// Generate a vertex array.
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));		// Generate a vertex buffer.
		VertexBufferLayout layout;								// Make a vertex buffer layout.
		layout.Push<float>(2);									// Specify the vertex buffer layouts structure.
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);								// Add the vertex buffer to the vertex array and specify the buffer memory layout of that buffer.

		IndexBuffer ib(indices, 6);								// Generate index buffer.

		// Projection matrix
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);		// Orthographic projection matrix.
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));	// View matrix.
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));	// Model matrix.

		glm::mat4 mvp = proj * view * model;										// Model view projection matrix (MVP), the calculation is in reverse since OpenGL uses column major matix ordering. 

		// Create shaders
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture("res/textures/logo_orange.png");
		texture.Bind(0);
		shader.SetUniform1i("u_Texture", 0);

		// Unbind stuff
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
	
		float r = 0.0f;
		float inc = 0.05f;

		glm::vec3 translationA(200,200,0);
		glm::vec3 translationB(400,400, 0);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();			// Clear the screen

		   // Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			
			//model = glm::translate(glm::mat4(1.0f), translationA);
			//mvp = proj * view * model;

			

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);
			}


			if (r > 1.0f)
				inc = -0.05f;
			else if (r < 0.0f)
				inc = 0.05f;

			r += inc;

			// Create imGui window
			{
				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
				//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				//ImGui::Checkbox("Another Window", &show_another_window);
				ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

																							  //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
				//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					//counter++;
				//ImGui::SameLine();
				//ImGui::Text("counter = %d", counter);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			//Render
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}