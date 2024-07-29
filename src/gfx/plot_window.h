#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scenes/plot.h"
#include "window_user_data.h"

namespace gfx
{
	struct plot_window
	{
		std::string _title;
		// plot_window() = default;
		plot_window( int width, int height, const char * title, GLFWwindow * main_window ) : _width { width }, _height { height }, _title { title }
		{
			glfw_window = glfwCreateWindow( width, height, title, NULL, main_window );

			if( ! glfw_window )
			{
				throw new err::scrooge_exception( __LINE__, __FILE__, "Could not create GLFW window!", err::exception_type::GFX );
			}

			glfwMakeContextCurrent( glfw_window );

			glViewport( 0, 0, width, height );

			_plot.init();
		}

		bool process( float delta_time )
		{
			glfwMakeContextCurrent( glfw_window );

			glfwPollEvents();

			process_keyboard( delta_time );

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
			glfwDestroyWindow( glfw_window );
		}

		void render_plot()
		{
			glClearColor( 0.1, 0.0, 0.1, 1.0 );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			// glViewport( 0, 0, _width / 2.0, _height / 2.0 );
			glm::mat4 identity_matrix = glm::mat4( 1.0f );
			glm::mat4 model = glm::mat4( 1.0f );
			glm::mat4 view = cam.get_view_matrix();
			glm::mat4 projection = glm::perspective( glm::radians( cam.zoom_level ), (float)_width / _height, 0.1f, 100000.0f );
			// glm::mat4 projection = glm::ortho( 0.0f, (float)_width, 0.0f, (float)_height, 0.1f, 100000.0f );

			try
			{
				ImGui::SetCurrentContext( window_data.imgui_context );
				_plot.render( model, view, projection );
			}
			catch( std::exception e )
			{
				std::printf( "THROWN FROM RENDERER" );
				throw err::scrooge_exception( __LINE__, __FILE__, e.what(), err::exception_type::GFX );
			}

			// glViewport( 0, 0, _width, _height );
			glfwSwapBuffers( glfw_window );
		}

		GLFWwindow * glfw_window;
		gfx::camera cam {};
		gfx::window_user_data window_data;

		void set_imgui()
		{
			window_data.cam = &cam;
			window_data.imgui_context = ImGui::CreateContext();
			window_data.window_name = _title;
			ImGui::SetCurrentContext( window_data.imgui_context );
			ImGui_ImplGlfw_InitForOpenGL( glfw_window, false );
			ImGui_ImplOpenGL3_Init( "#version 330" );
		}

	private:
		void process_keyboard( float delta_time )
		{
			// _action = NOTHING;
			//
			// if( glfwGetKey( glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
			// {
			// 	glfwSetWindowShouldClose( glfw_window, true );
			// 	_action = SHUTDOWN;
			// }
			// if( glfwGetKey( glfw_window, GLFW_KEY_O ) == GLFW_PRESS )
			// {
			// 	_action = NEW_EXTRA_WINDOW;
			// }

			if( glfwGetKey( glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
			{
				glfwSetWindowShouldClose( glfw_window, true );
			}

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

		gfx::plot _plot {};
		int _width, _height = 0.0;
	};
} // namespace gfx
