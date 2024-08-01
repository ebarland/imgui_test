#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "camera.h"
#include <vector>

#define VECTOR_VERSION true

struct WindowData
{
	ImGuiContext * imguiContext;
	camera * camera;
};

struct MainWindow
{
	GLFWwindow * glfwWindow;
	camera camera {};
	WindowData data;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
	float f = 0.0f;
	int counter = 0;
};

struct ExtraWindow
{
	GLFWwindow * glfwWindow;
	camera camera {};
	WindowData data;

	float f = 0.0f;
	int counter = 0;
};

MainWindow main_window;
std::vector<ExtraWindow> extra_windows;

void mouse_position_callback( GLFWwindow * glfwWindow, double xpos, double ypos )
{
	WindowData * windowData = static_cast<WindowData *>( glfwGetWindowUserPointer( glfwWindow ) );

	if( windowData && windowData->imguiContext )
	{
		ImGui::SetCurrentContext( windowData->imguiContext );

		ImGuiIO & io = ImGui::GetIO();
		io.MousePos = ImVec2( (float)xpos, (float)ypos );

		if( ! io.WantCaptureMouse )
		{
			windowData->camera->process_mouse( xpos, ypos );
		}
	}
}

void mouse_button_callback( GLFWwindow * glfwWindow, int button, int action, int mods )
{
	WindowData * windowData = static_cast<WindowData *>( glfwGetWindowUserPointer( glfwWindow ) );

	if( windowData && windowData->imguiContext )
	{
		ImGui::SetCurrentContext( windowData->imguiContext );

		ImGuiIO & io = ImGui::GetIO();

		if( button >= 0 && button < IM_ARRAYSIZE( io.MouseDown ) )
		{
			if( action == GLFW_PRESS )
				io.MouseDown[button] = true;
			else if( action == GLFW_RELEASE )
				io.MouseDown[button] = false;
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

	// Create ImGui context and set it to the window
	main_window.data.imguiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext( main_window.data.imguiContext );
	main_window.data.camera = &main_window.camera;

	// set callback data for window
	glfwSetWindowUserPointer( main_window.glfwWindow, &main_window.data );

	// Set the mouse callback
	glfwSetCursorPosCallback( main_window.glfwWindow, mouse_position_callback );
	glfwSetMouseButtonCallback( main_window.glfwWindow, mouse_button_callback );

	IMGUI_CHECKVERSION();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL( main_window.glfwWindow, true );
	ImGui_ImplOpenGL3_Init( "#version 330" );
}

ExtraWindow CreateWindow( int width, int height, const char * title )
{
	ExtraWindow window;
	window.glfwWindow = glfwCreateWindow( width, height, title, nullptr, main_window.glfwWindow );

	if( ! window.glfwWindow )
	{
		// Handle error
		throw std::exception { "Failed to create GLFW window" };
	}

	glfwMakeContextCurrent( window.glfwWindow );

	// Create ImGui context and set it to the window
	window.data.imguiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext( window.data.imguiContext );
	window.data.camera = &window.camera;

	// set callback data for window
	glfwSetWindowUserPointer( window.glfwWindow, &window.data );

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
	{
		glfwPollEvents();

		glfwMakeContextCurrent( main_window.glfwWindow );
		ImGui::SetCurrentContext( main_window.data.imguiContext );
		// Start ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if( main_window.show_demo_window )
		{
			ImGui::ShowDemoWindow( &main_window.show_demo_window );
		}

		{
			ImGui::Begin( "start menu!" );
			ImGui::Text( "This is some useful text." );
			ImGui::Checkbox( "Demo Window", &main_window.show_demo_window );
			ImGui::Checkbox( "Another Window", &main_window.show_another_window );
			ImGui::SliderFloat( "float", &main_window.f, 0.0f, 1.0f );
			ImGui::ColorEdit3( "clear color", (float *)&main_window.clear_color );
			if( ImGui::Button( "Button" ) )
			{
				main_window.counter++;
			}
			ImGui::SameLine();
			ImGui::Text( "counter = %d", main_window.counter );
			ImGui::End();
		}

		if( main_window.show_another_window )
		{
			ImGui::Begin( "Another Window", &main_window.show_another_window );
			ImGui::Text( "Hello from another window!" );
			if( ImGui::Button( "Close Me" ) )
			{
				main_window.show_another_window = false;
			}
			ImGui::End();
		}

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize( main_window.glfwWindow, &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		glClearColor( 0.45f, 0.55f, 0.60f, 1.00f );
		glClear( GL_COLOR_BUFFER_BIT );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		glfwSwapBuffers( main_window.glfwWindow );

		for( auto & window: extra_windows )
		{
			glfwMakeContextCurrent( window.glfwWindow );
			ImGui::SetCurrentContext( window.data.imguiContext );
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin( "hello" );
			ImGui::Text( "This is some useful text." );
			ImGui::SliderFloat( "float", &window.f, 0.0f, 1.0f );

			if( ImGui::Button( "Button" ) )
			{
				window.counter++;
			}

			ImGui::SameLine();
			ImGui::Text( "counter = %d", window.counter );
			ImGui::End();
			ImGui::Render();
			glfwGetFramebufferSize( window.glfwWindow, &display_w, &display_h );
			glViewport( 0, 0, display_w, display_h );
			glClearColor( 0.45f, 0.55f, 0.60f, 1.00f );
			glClear( GL_COLOR_BUFFER_BIT );
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

			glfwSwapBuffers( window.glfwWindow );
		}
	}
}

int main( int, char ** )
{
	createMainWindow();

	for( int i = 0; i < 2; ++i )
	{
		extra_windows.emplace_back( CreateWindow( 800, 700, "window" ) );
	}

	renderLoop();
} hehe
