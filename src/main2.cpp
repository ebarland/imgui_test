#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "gfx/camera.h"
#include <vector>

#define VECTOR_VERSION true

struct WindowData
{
	ImGuiContext * imguiContext;
	gfx::camera camera {};
};

struct MainWindow
{
	GLFWwindow * glfwWindow;
	WindowData data;
};

struct Window
{
	GLFWwindow * glfwWindow;
	WindowData data;
};

MainWindow main_window;
std::vector<Window> windows;

void mouse_position_callback( GLFWwindow * glfwWindow, double xpos, double ypos )
{
	WindowData * windowData = static_cast<WindowData *>( glfwGetWindowUserPointer( glfwWindow ) );

	if( windowData && windowData->imguiContext )
	{
		ImGui::SetCurrentContext( windowData->imguiContext );

		ImGuiIO & io = ImGui::GetIO();

		io.MousePos.x = (float)xpos;
		io.MousePos.y = (float)ypos;

		if( ! io.WantCaptureMouse )
		{
			windowData->camera.process_mouse( xpos, ypos );
		}
	}

	// glfwMakeContextCurrent( glfwWindow );
	//
	// WindowData * windowData = static_cast<WindowData *>( glfwGetWindowUserPointer( glfwWindow ) );
	//
	// ImGui::SetCurrentContext( windowData->imguiContext );
	//
	// ImGuiIO & io = ImGui::GetIO();
	//
	// io.MousePos.x = (float)xpos;
	// io.MousePos.y = (float)ypos;
	//
	// if( ! io.WantCaptureMouse )
	// {
	// 	// windowData->camera->process_mouse( (float)xpos, (float)ypos );
	// }
}

void mouse_button_callback( GLFWwindow * glfwWindow, int button, int action, int mods )
{
	WindowData * windowData = static_cast<WindowData *>( glfwGetWindowUserPointer( glfwWindow ) );

	if( windowData && windowData->imguiContext )
	{
		ImGui::SetCurrentContext( windowData->imguiContext );

		ImGuiIO & io = ImGui::GetIO();

		// bool MouseDown[5];
		//  Mouse buttons: 0=left, 1=right, 2=middle + extras (ImGuiMouseButton_COUNT == 5). Dear ImGui mostly uses left and right buttons. Other buttons allow us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
		if( button == GLFW_MOUSE_BUTTON_LEFT )
		{
			if( GLFW_PRESS == action )
				io.MouseDown[0] = true;
			else if( GLFW_RELEASE == action )
				io.MouseDown[0] = false;
		}
	}
}

void createMainWindow()
{
	if( ! glfwInit() )
		throw std::exception { "Failed to initialize GLFW" };

	main_window.glfwWindow = glfwCreateWindow( 200, 200, "main", nullptr, nullptr );

	if( ! main_window.glfwWindow )
	{
		throw std::exception { "Failed to create GLFW window" };
	}

	glfwMakeContextCurrent( main_window.glfwWindow );

	if( ! gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		throw std::exception( "failed to init GLAD" );
	}

	main_window.data.imguiContext = ImGui::CreateContext();
	glfwSetWindowUserPointer( main_window.glfwWindow, &main_window.data );
	// glfwSetCursorPosCallback( main_window.glfwWindow, mouse_position_callback );
	glfwSetMouseButtonCallback( main_window.glfwWindow, mouse_button_callback );

	IMGUI_CHECKVERSION();

	ImGuiIO & io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows

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
	ImGui_ImplGlfw_InitForOpenGL( main_window.glfwWindow, true );
	ImGui_ImplOpenGL3_Init( "#version 330" );
}

Window CreateWindow( int width, int height, const char * title )
{
	Window window;
	window.glfwWindow = glfwCreateWindow( width, height, title, nullptr, main_window.glfwWindow );

	if( ! window.glfwWindow )
	{
		// Handle error
		throw std::exception { "Failed to create GLFW window" };
	}

	glfwMakeContextCurrent( window.glfwWindow );

	// Create ImGui context and set it to the window
	window.data.imguiContext = ImGui::CreateContext();
	glfwSetWindowUserPointer( window.glfwWindow, &window.data );

	ImGui::SetCurrentContext( window.data.imguiContext );

	// Set the mouse callback
	glfwSetCursorPosCallback( window.glfwWindow, mouse_position_callback );
	glfwSetMouseButtonCallback( window.glfwWindow, mouse_button_callback );

	ImGui_ImplGlfw_InitForOpenGL( window.glfwWindow, false );
	ImGui_ImplOpenGL3_Init( "#version 330" );

	return window;
}

void renderLoop()
{
	while( ! glfwWindowShouldClose( main_window.glfwWindow ) )
	{ // Assume first window for the loop condition

		glfwMakeContextCurrent( main_window.glfwWindow );
		ImGui::SetCurrentContext( main_window.data.imguiContext );

		// Start ImGui frame
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

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize( main_window.glfwWindow, &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		glClearColor( 0.45f, 0.55f, 0.60f, 1.00f );
		glClear( GL_COLOR_BUFFER_BIT );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		ImGuiIO & io = ImGui::GetIO();
		if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			GLFWwindow * backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent( backup_current_context );
		}

		glfwSwapBuffers( main_window.glfwWindow );

		for( auto & window: windows )
		{
			glfwMakeContextCurrent( window.glfwWindow );
			ImGui::SetCurrentContext( window.data.imguiContext );

			// Start ImGui frame
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

			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize( window.glfwWindow, &display_w, &display_h );
			glViewport( 0, 0, display_w, display_h );
			glClearColor( 0.45f, 0.55f, 0.60f, 1.00f );
			glClear( GL_COLOR_BUFFER_BIT );
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

			ImGuiIO & io = ImGui::GetIO();
			if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
			{
				GLFWwindow * backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent( backup_current_context );
			}

			glfwSwapBuffers( window.glfwWindow );
		}

		glfwPollEvents();
	}
}

int main( int, char ** )
{
	createMainWindow();

	for( int i = 0; i < 2; ++i )
	{
		windows.emplace_back( CreateWindow( 800, 700, "window" ) );
	}

	renderLoop();
}
