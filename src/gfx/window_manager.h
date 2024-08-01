#pragma once
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "main_window.h"
#include "plot_window.h"
#include <iostream>

namespace gfx
{
	static window_user_data * get_window_user_data( GLFWwindow * window )
	{
		return static_cast<window_user_data *>( glfwGetWindowUserPointer( window ) );
	}

	static void scroll_callback( GLFWwindow * window, double x, double y )
	{
		window_user_data * user_data = get_window_user_data( window );
		user_data->cam->process_scroll( (float)y );
	}

	static void framebuffer_size_callback( GLFWwindow * window, int width, int height )
	{
		glfwMakeContextCurrent( window );
		glViewport( 0, 0, width, height );
	}

	static void cursor_position_callback( GLFWwindow * window, double x_offset, double y_offset )
	{
		auto current_context = ImGui::GetCurrentContext();

		glfwMakeContextCurrent( window );

		window_user_data * user_data = get_window_user_data( window );

		ImGui::SetCurrentContext( user_data->imgui_context );

		ImGuiIO & io = ImGui::GetIO();

		io.MousePos.x = (float)x_offset;
		io.MousePos.y = (float)y_offset;

		if( ! io.WantCaptureMouse )
		{
			user_data->cam->process_mouse( (float)x_offset, (float)y_offset );
		}

		ImGui::SetCurrentContext( current_context );
	}

	static void mouse_callback( GLFWwindow * window, int button, int action, int mods )
	{
		auto current_context = ImGui::GetCurrentContext();

		glfwMakeContextCurrent( window );

		window_user_data * user_data = get_window_user_data( window );

		ImGui::SetCurrentContext( user_data->imgui_context );

		ImGuiIO & io = ImGui::GetIO();

		if( button == GLFW_MOUSE_BUTTON_LEFT )
		{
			if( GLFW_PRESS == action )
				io.MouseDown[0] = true;
			else if( GLFW_RELEASE == action )
				io.MouseDown[0] = false;
		}

		ImGui::SetCurrentContext( current_context );
	}

	struct window_manager
	{
		window_manager()
		{
			if( ! glfwInit() )
				throw err::spec_exception { __LINE__, __FILE__, "Failed to initialize GLFW", err::exception_type::GFX };

			_main_window = gfx::main_window( 1920, 1080, "scrooge" );

			glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
			glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
			glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

			// glfwSetScrollCallback( _main_window.glfw_window, scroll_callback );
			// glfwSetFramebufferSizeCallback( _main_window.glfw_window, framebuffer_size_callback );
			glfwSetCursorPosCallback( _main_window.glfw_window, cursor_position_callback );
			glfwSetMouseButtonCallback( _main_window.glfw_window, mouse_callback );

			_main_window.set_imgui();

			glfwSetWindowUserPointer( _main_window.glfw_window, &_main_window.window_data );

			std::printf( "%s\n", glGetString( GL_VERSION ) );
			int n_max_attribs;
			glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &n_max_attribs );
			std::printf( "Max number of vertex attributes supported: %d\n", n_max_attribs );
		}

		bool compose()
		{
			switch( _main_window.process() )
			{
			case gfx::MAIN_WINDOW_ACTION::SHUTDOWN:
				shutdown();
				return false;
			case gfx::MAIN_WINDOW_ACTION::NOTHING:
				process_extra_windows();
				break;
			case gfx::MAIN_WINDOW_ACTION::NEW_EXTRA_WINDOW:
				add_extra_window( 1920, 1080, std::format( "Extra window {}", window_counter ).data() );
				break;
			}

			return true;
		}

	private:
		void shutdown()
		{
			for( auto & w: extra_windows )
			{
				w.destroy();
			}

			_main_window.shutdown();
			_main_window.destroy();
			glfwTerminate();
		}

		// void add_extra_window( int width, int height, const char * title )
		// {
		// 	if( window_counter == 0 )
		// 	{
		// 		extra_window0 = gfx::plot_window( width, height, title, _main_window.glfw_window );
		// 		// glfwSetScrollCallback( extra_windows[extra_windows.size() - 1].glfw_window, scroll_callback );
		// 		// glfwSetFramebufferSizeCallback( extra_windows[extra_windows.size() - 1].glfw_window, framebuffer_size_callback );
		// 		glfwSetCursorPosCallback( extra_window0.glfw_window, cursor_position_callback );
		// 		glfwSetMouseButtonCallback( extra_window0.glfw_window, mouse_callback );
		//
		// 		extra_window0.set_imgui();
		//
		// 		glfwSetWindowUserPointer( extra_window0.glfw_window, &extra_window0.window_data );
		// 	}
		// 	if( window_counter == 1 )
		// 	{
		// 		extra_window1 = gfx::plot_window( width, height, title, _main_window.glfw_window );
		// 		// glfwSetScrollCallback( extra_windows[extra_windows.size() - 1].glfw_window, scroll_callback );
		// 		// glfwSetFramebufferSizeCallback( extra_windows[extra_windows.size() - 1].glfw_window, framebuffer_size_callback );
		// 		glfwSetCursorPosCallback( extra_window1.glfw_window, cursor_position_callback );
		// 		glfwSetMouseButtonCallback( extra_window1.glfw_window, mouse_callback );
		//
		// 		extra_window1.set_imgui();
		//
		// 		glfwSetWindowUserPointer( extra_window1.glfw_window, &extra_window1.window_data );
		// 	}
		//
		// 	++window_counter;
		//
		// 	if( window_counter == 2 )
		// 		return;
		// }

		// void process_extra_windows( float delta_time )
		// {
		// 	std::vector<int> remove_indices;
		//
		// 	if( window_counter > 0 )
		// 	{
		// 		extra_window0.process( delta_time );
		// 		if( window_counter > 1 )
		// 		{
		// 			extra_window1.process( delta_time );
		// 		}
		// 	}
		//
		// 	// for( auto i = 0; i < extra_windows.size(); ++i )
		// 	// {
		// 	// 	if( extra_windows[i].process( delta_time ) )
		// 	// 		remove_indices.push_back( i );
		// 	// }
		// 	//
		// 	// for( auto i = 0; i < remove_indices.size(); ++i )
		// 	// 	extra_windows.erase( extra_windows.begin() + remove_indices[i] );
		// }

		void add_extra_window( int width, int height, const char * title )
		{
			extra_windows.push_back( gfx::plot_window( width, height, title, _main_window.glfw_window ) );
			// glfwSetScrollCallback( extra_windows[extra_windows.size() - 1].glfw_window, scroll_callback );
			// glfwSetFramebufferSizeCallback( extra_windows[extra_windows.size() - 1].glfw_window, framebuffer_size_callback );
			glfwSetCursorPosCallback( extra_windows.back().glfw_window, cursor_position_callback );
			glfwSetMouseButtonCallback( extra_windows.back().glfw_window, mouse_callback );

			extra_windows.back().set_imgui();

			glfwSetWindowUserPointer( extra_windows.back().glfw_window, &extra_windows.back().window_data );

			std::cout << "created window " << extra_windows.back()._title << " with data " << &extra_windows.back().window_data << std::endl;

			++window_counter;
		}
		void process_extra_windows()
		{
			std::vector<int> remove_indices;

			for( auto i = 0; i < extra_windows.size(); ++i )
			{
				if( extra_windows[i].process() )
					remove_indices.push_back( i );
			}

			for( auto i = 0; i < remove_indices.size(); ++i )
				extra_windows.erase( extra_windows.begin() + remove_indices[i] );
		}

		gfx::main_window _main_window;
		// gfx::plot_window extra_window0;
		// gfx::plot_window extra_window1;
		std::vector<gfx::plot_window> extra_windows;

		int window_counter = 0;
	};

} // namespace gfx
