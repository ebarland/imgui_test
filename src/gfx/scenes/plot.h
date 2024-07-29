#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../mesh.h"
#include "GLFW/glfw3.h"

namespace gfx
{
	struct plot
	{
		static constexpr std::vector<glm::vec3> verts()
		{
			std::vector<glm::vec3> vertices = std::vector<glm::vec3>( 2000 );

			for( int i = 0; i < 2000; i++ )
			{
				float x = ( i - 1000.0 ) / 100.0;
				vertices[i].x = x;
				vertices[i].y = sin( x * 10.0 ) / ( 1.0 + x * x );
				vertices[i].z = 0.0f;
			}

			return vertices;
		}

		plot() : _mesh { gfx::mesh { verts() } }
		{
		}

		void init()
		{
			_mesh.init();
			_shader_program.build();
		}

		void render( glm::mat4 model, glm::mat4 view, glm::mat4 projection )
		{
			_mesh.draw( GL_LINE_STRIP, _shader_program, model, view, projection );
			render_imgui();
		}

		void render_imgui()
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin( "hello" ); // Create a window called "Hello, world!" and append into it.

			ImGui::Text( "This is some useful text." );	   // Display some text (you can use a format strings too)
			ImGui::SliderFloat( "float", &f, 0.0f, 1.0f ); // Edit 1 float using a slider from 0.0f to 1.0f

			if( ImGui::Button( "Button" ) ) // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				counter2++;
			}

			ImGui::SameLine();
			ImGui::Text( "counter = %d", counter2 );

			// ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate );
			ImGui::End();

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

		float f = 0.0f;
		int counter2 = 0;

		gfx::mesh _mesh;
		gfx::shader_program _shader_program { { gfx::shader { "src/gfx/shaders/plot.vs", gfx::VERTEX }, gfx::shader { "src/gfx/shaders/plot.fs", gfx::FRAGMENT } } };
	};

} // namespace gfx
