#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window_user_data.h"
#include "../err/spec_exception.h"

namespace gfx
{
	enum MAIN_WINDOW_ACTION
	{
		NOTHING,
		NEW_EXTRA_WINDOW,
		SHUTDOWN
	};

	struct main_window
	{
		main_window() = default;
		main_window( int width, int height, const char * title ) : _width { width }, _height { height }
		{
			glfw_window = glfwCreateWindow( width, height, title, NULL, NULL );

			if( ! glfw_window )
			{
				throw err::spec_exception { __LINE__, __FILE__, "Could not create GLFW window!", err::exception_type::WINDOW };
			}

			glfwMakeContextCurrent( glfw_window );

			if( ! gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
			{
				throw err::spec_exception { __LINE__, __FILE__, "Failed to initialize GLAD", err::exception_type::WINDOW };
			}

			glViewport( 0, 0, width, height );
		}

		MAIN_WINDOW_ACTION process()
		{
			glfwMakeContextCurrent( glfw_window );
			glfwPollEvents();
			process_keyboard();
			render_main_window();

			if( glfwWindowShouldClose( glfw_window ) )
				_action = SHUTDOWN;

			return _action;
		}

		void shutdown()
		{
			glfwSetWindowShouldClose( glfw_window, true );
		}

		void destroy()
		{
			ImGui::SetCurrentContext( window_data.imgui_context );
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			glfwDestroyWindow( glfw_window );
		}

		GLFWwindow * glfw_window;
		gfx::camera cam {};
		gfx::window_user_data window_data {};

		void set_imgui()
		{
			window_data.cam = &cam;
			window_data.imgui_context = ImGui::CreateContext();

			ImGui::SetCurrentContext( window_data.imgui_context );

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();

			ImGuiIO & io = ImGui::GetIO();
			(void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows
			// io.ConfigViewportsNoAutoMerge = true;
			// io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle & style = ImGui::GetStyle();
			if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL( glfw_window, true );
			ImGui_ImplOpenGL3_Init( "#version 330" );
		}

	private:
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

		void render_main_window()
		{
			glClearColor( 0.2, 0.2, 0.2, 1.0 );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			ImGui::SetCurrentContext( window_data.imgui_context );
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

			// glViewport( 0, 0, _width, _height );
			glfwSwapBuffers( glfw_window );
		}

		void process_keyboard()
		{
			_action = NOTHING;

			if( glfwGetKey( glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
			{
				glfwSetWindowShouldClose( glfw_window, true );
				_action = SHUTDOWN;
			}
			if( glfwGetKey( glfw_window, GLFW_KEY_O ) == GLFW_PRESS )
			{
				_action = NEW_EXTRA_WINDOW;
			}

			if( glfwGetKey( glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
				glfwSetWindowShouldClose( glfw_window, true );
		}

		int _width, _height = 0.0;
		MAIN_WINDOW_ACTION _action = MAIN_WINDOW_ACTION::NOTHING;
	};
} // namespace gfx
