#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scenes/main_menu.h"
#include "window_user_data.h"

namespace gfx
{
	enum MAIN_WINDOW_ACTION
	{
		NOTHING,
		NEW_EXTRA_WINDOW,
		SHUTDOWN
	};

	enum PROGRAM_STATE
	{
		MAIN_MENU
	};

	struct main_window
	{
		main_window() = default;
		main_window( int width, int height, const char * title ) : _width { width }, _height { height }
		{
			glfw_window = glfwCreateWindow( width, height, title, NULL, NULL );

			if( ! glfw_window )
			{
				throw err::scrooge_exception { __LINE__, __FILE__, "Could not create GLFW window!", err::exception_type::WINDOW };
			}

			glfwMakeContextCurrent( glfw_window );

			if( ! gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
			{
				throw err::scrooge_exception { __LINE__, __FILE__, "Failed to initialize GLAD", err::exception_type::WINDOW };
			}

			glViewport( 0, 0, width, height );

			_main_menu.init();
		}

		MAIN_WINDOW_ACTION process( PROGRAM_STATE state, float delta_time )
		{
			glfwMakeContextCurrent( glfw_window );

			glfwPollEvents();

			process_keyboard( delta_time );

			switch( state )
			{
			case MAIN_MENU:
				render_main_menu();
				break;
			default:
				break;
			}

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
		void render_main_menu()
		{
			glClearColor( 0.2, 0.2, 0.2, 1.0 );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			// glViewport( 0, 0, _width / 2.0, _height / 2.0 );
			glm::mat4 identity_matrix = glm::mat4( 1.0f );
			glm::mat4 model = glm::mat4( 1.0f );
			glm::mat4 view = cam.get_view_matrix();
			glm::mat4 projection = glm::perspective( glm::radians( cam.zoom_level ), (float)_width / _height, 0.1f, 100000.0f );
			// glm::mat4 projection = glm::ortho( 0.0f, (float)_width, 0.0f, (float)_height, 0.1f, 100000.0f );

			ImGui::SetCurrentContext( window_data.imgui_context );
			_main_menu.render( model, view, projection );

			// glViewport( 0, 0, _width, _height );
			glfwSwapBuffers( glfw_window );
		}

		void process_keyboard( float delta_time )
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

			// if( glfwGetKey( _window, GLFW_KEY_UP ) == GLFW_PRESS )
			// 	set_mix_val( 0.001 );
			//
			// if( glfwGetKey( _window, GLFW_KEY_DOWN ) == GLFW_PRESS )
			// 	set_mix_val( -0.001 );

			if( glfwGetKey( glfw_window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
				delta_time = delta_time * 10;

			// we already have a normalized direction vector, so just add to the position
			if( glfwGetKey( glfw_window, GLFW_KEY_E ) == GLFW_PRESS )
				cam.process_keyboard( gfx::camera_movement::FORWARD, delta_time );
			if( glfwGetKey( glfw_window, GLFW_KEY_D ) == GLFW_PRESS )
				cam.process_keyboard( gfx::camera_movement::BACKWARD, delta_time );

			// do a cross product of the direction vector front and up to get the perpendicular axis, then normalize to create a pure direction vector
			if( glfwGetKey( glfw_window, GLFW_KEY_S ) == GLFW_PRESS )
				cam.process_keyboard( gfx::camera_movement::LEFT, delta_time );
			if( glfwGetKey( glfw_window, GLFW_KEY_F ) == GLFW_PRESS )
				cam.process_keyboard( gfx::camera_movement::RIGHT, delta_time );

			if( glfwGetKey( glfw_window, GLFW_KEY_SPACE ) == GLFW_PRESS )
				cam.process_keyboard( gfx::camera_movement::UP, delta_time );
			if( glfwGetKey( glfw_window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS )
				cam.process_keyboard( gfx::camera_movement::DOWN, delta_time );
		}

		gfx::main_menu _main_menu {};
		int _width, _height = 0.0;
		MAIN_WINDOW_ACTION _action = MAIN_WINDOW_ACTION::NOTHING;
	};
} // namespace gfx
