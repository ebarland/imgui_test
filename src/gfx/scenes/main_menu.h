#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../mesh.h"
#include "GLFW/glfw3.h"

namespace gfx
{
	struct main_menu
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

		void init()
		{
			_mesh.init();
			_shader_program.build();
		}

		void render( glm::mat4 model, glm::mat4 view, glm::mat4 projection )
		{
			_mesh.draw( GL_TRIANGLES, _shader_program, model, view, projection );
			render_imgui();
		}

		void render_imgui()
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if( show_demo_window )
			{
				ImGui::ShowDemoWindow( &show_demo_window );
			}

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin( "start menu!" ); // Create a window called "Hello, world!" and append into it.

				ImGui::Text( "This is some useful text." );			 // Display some text (you can use a format strings too)
				ImGui::Checkbox( "Demo Window", &show_demo_window ); // Edit bools storing our window open/close state
				ImGui::Checkbox( "Another Window", &show_another_window );

				ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );			   // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3( "clear color", (float *)&clear_color ); // Edit 3 floats representing a color

				if( ImGui::Button( "Button" ) ) // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					counter++;
				}

				ImGui::SameLine();
				ImGui::Text( "counter = %d", counter );

				// ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate );
				ImGui::End();
			}

			// 3. Show another simple window.
			if( show_another_window )
			{
				ImGui::Begin( "Another Window", &show_another_window ); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text( "Hello from another window!" );
				if( ImGui::Button( "Close Me" ) )
					show_another_window = false;
				ImGui::End();
			}

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			// For this specific demo app we could also call glfwMakeContextCurrent(window) directly)

			ImGuiIO & io = ImGui::GetIO();
			if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				GLFWwindow * backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent( backup_current_context );
			}
		}

		gfx::mesh _mesh { { glm::vec3 { -0.5f, -0.5f, 0.0f }, glm::vec3 { 0.5f, -0.5f, 0.0f }, glm::vec3 { 0.0f, 0.5f, 0.0f } } };
		gfx::shader_program _shader_program { { gfx::shader { "src/gfx/shaders/triangle.vs", gfx::VERTEX }, gfx::shader { "src/gfx/shaders/triangle.fs", gfx::FRAGMENT } } };
	};
} // namespace gfx
