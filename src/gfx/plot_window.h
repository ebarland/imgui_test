#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window_user_data.h"

namespace gfx
{
	struct plot_window
	{
		const char * _title;
		plot_window( plot_window && other )
		{
		}
		plot_window( int width, int height, const char * title, GLFWwindow * main_window ) : _width { width }, _height { height }, _title { title }
		{
			glfw_window = glfwCreateWindow( width, height, title, NULL, main_window );

			if( ! glfw_window )
			{
				throw new err::spec_exception( __LINE__, __FILE__, "Could not create GLFW window!", err::exception_type::GFX );
			}

			glfwMakeContextCurrent( glfw_window );

			glViewport( 0, 0, width, height );
		}

		bool process()
		{
			glfwMakeContextCurrent( glfw_window );
			glfwPollEvents();
			process_keyboard();
			render_plot();

			if( glfwWindowShouldClose( glfw_window ) )
			{
				destroy();
				return true;
			}

			return false;
		}

		void destroy()
		{
			ImGui::SetCurrentContext( window_data.imgui_context );
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			glfwDestroyWindow( glfw_window );
		}

		void render_plot()
		{
			glClearColor( 0.1, 0.0, 0.1, 1.0 );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			ImGui::SetCurrentContext( window_data.imgui_context );

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin( _title ); // Create a window called "Hello, world!" and append into it.

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

			// glViewport( 0, 0, _width, _height );
			glfwSwapBuffers( glfw_window );
		}

		void set_imgui()
		{
			window_data.cam = &cam;
			window_data.imgui_context = ImGui::CreateContext();
			ImGui::SetCurrentContext( window_data.imgui_context );
			ImGui_ImplGlfw_InitForOpenGL( glfw_window, false );
			ImGui_ImplOpenGL3_Init( "#version 330" );
		}

		float f = 0.0f;
		int counter2 = 0;
		gfx::camera cam {};
		GLFWwindow * glfw_window;
		gfx::window_user_data window_data;
		int _width, _height = 0.0;

		void process_keyboard()
		{
			if( glfwGetKey( glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
			{
				glfwSetWindowShouldClose( glfw_window, true );
			}
		}
	};
} // namespace gfx
