#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "camera.h"
#include <vector>

struct WindowData
{
	ImGuiContext * imguiContext;
	camera * camera;
};

static void mouse_position_callback( GLFWwindow * glfwWindow, double xpos, double ypos )
{
	ImGuiIO & io = ImGui::GetIO();
	io.MousePos = ImVec2( (float)xpos, (float)ypos );

	if( ! io.WantCaptureMouse )
	{
		WindowData * windowData = static_cast<WindowData *>( glfwGetWindowUserPointer( glfwWindow ) );
		if( windowData && windowData->imguiContext )
		{
			windowData->camera->process_mouse( xpos, ypos );
		}
	}
}

static void mouse_button_callback( GLFWwindow * glfwWindow, int button, int action, int mods )
{
	ImGuiIO & io = ImGui::GetIO();

	if( button >= 0 && button < IM_ARRAYSIZE( io.MouseDown ) )
	{
		if( action == GLFW_PRESS )
			io.MouseDown[button] = true;
		else if( action == GLFW_RELEASE )
			io.MouseDown[button] = false;
	}
}

struct MainWindow
{
	MainWindow( int width, int height, const char * title ) : width { width }, height { height }
	{
		if( ! glfwInit() )
			throw std::exception { "Failed to initialize GLFW" };

		glfwWindow = glfwCreateWindow( width, height, title, nullptr, nullptr );

		if( ! glfwWindow )
		{
			throw std::exception { "Failed to create GLFW window" };
		}

		glfwMakeContextCurrent( glfwWindow );

		if( ! gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
		{
			throw std::exception( "failed to init GLAD" );
		}

		// Create ImGui context and set it to the window
		imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext( imguiContext );

		// set callback data for window
		data.imguiContext = imguiContext;
		data.camera = &camera;
		glfwSetWindowUserPointer( glfwWindow, &data );

		// Set the mouse callback
		glfwSetCursorPosCallback( glfwWindow, mouse_position_callback );
		glfwSetMouseButtonCallback( glfwWindow, mouse_button_callback );

		IMGUI_CHECKVERSION();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL( glfwWindow, false );
		ImGui_ImplOpenGL3_Init( "#version 330" );
	}

	int width, height;

	GLFWwindow * glfwWindow;
	ImGuiContext * imguiContext;
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
	ExtraWindow( int width, int height, const char * title, GLFWwindow * mainWindow ) : width { width }, height { height }

	{
		glfwWindow = glfwCreateWindow( width, height, title, nullptr, mainWindow );

		if( ! glfwWindow )
		{
			throw std::exception { "Failed to create GLFW window" };
		}

		glfwMakeContextCurrent( glfwWindow );

		// Create ImGui context and set it to the window
		imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext( imguiContext );

		// set callback data for window
		data.imguiContext = imguiContext;
		data.camera = &camera;
		glfwSetWindowUserPointer( glfwWindow, &data );

		// Set the mouse callback
		glfwSetCursorPosCallback( glfwWindow, mouse_position_callback );
		glfwSetMouseButtonCallback( glfwWindow, mouse_button_callback );

		ImGui_ImplGlfw_InitForOpenGL( glfwWindow, false );
		ImGui_ImplOpenGL3_Init( "#version 330" );
	}

	int width, height;

	GLFWwindow * glfwWindow;
	ImGuiContext * imguiContext;
	camera camera {};
	WindowData data;

	// Move constructor
	ExtraWindow( ExtraWindow && other ) noexcept : glfwWindow( other.glfwWindow ), imguiContext( other.imguiContext ), camera( std::move( other.camera ) ), f( other.f ), counter( other.counter )
	{
		data.imguiContext = imguiContext;
		data.camera = &camera;

		other.glfwWindow = nullptr;
		other.imguiContext = nullptr;
		other.data.imguiContext = nullptr;
		other.data.camera = nullptr;
	}

	// Move assignment operator
	ExtraWindow & operator=( ExtraWindow && other ) noexcept
	{
		if( this != &other )
		{
			glfwWindow = other.glfwWindow;
			imguiContext = other.imguiContext;
			camera = std::move( other.camera );
			data.imguiContext = imguiContext;
			data.camera = &camera;
			f = other.f;
			counter = other.counter;

			other.glfwWindow = nullptr;
			other.imguiContext = nullptr;
			other.data.imguiContext = nullptr;
			other.data.camera = nullptr;
		}
		return *this;
	}

	// Disable copying
	ExtraWindow( const ExtraWindow & ) = delete;
	ExtraWindow & operator=( const ExtraWindow & ) = delete;

	~ExtraWindow()
	{
		if( glfwWindow )
		{
			glfwDestroyWindow( glfwWindow );
		}
		if( data.imguiContext )
		{
			ImGui::DestroyContext( data.imguiContext );
		}
	}

	// variables for imgui componets
	float f = 0.0f;
	int counter = 0;
};

void renderLoop()
{
	MainWindow main_window { 800, 600, "main" };
	ExtraWindow extra_window { 800, 700, "window", main_window.glfwWindow };

	// std::vector<ExtraWindow> extra_windows;
	// for( int i = 0; i < 1; ++i )
	// {
	// 	extra_windows.emplace_back();
	// }

	while( ! glfwWindowShouldClose( main_window.glfwWindow ) )
	{
		// Render main window
		glfwMakeContextCurrent( main_window.glfwWindow );
		glfwPollEvents();

		ImGui::SetCurrentContext( main_window.data.imguiContext );
		glViewport( 0, 0, main_window.width, main_window.height );
		glClearColor( 0.45f, 0.55f, 0.60f, 1.00f );
		glClear( GL_COLOR_BUFFER_BIT );
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
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
		glfwSwapBuffers( main_window.glfwWindow );

		// Render extra windows
		// for( auto & extra_window: extra_windows )
		// {
		glfwMakeContextCurrent( extra_window.glfwWindow );
		glfwPollEvents();

		ImGui::SetCurrentContext( extra_window.data.imguiContext );
		glViewport( 0, 0, extra_window.width, extra_window.height );
		glClearColor( 0.15f, 0.95f, 0.60f, 1.00f );
		glClear( GL_COLOR_BUFFER_BIT );
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin( "hello" );
		ImGui::Text( "This is some useful text." );
		ImGui::SliderFloat( "float", &extra_window.f, 0.0f, 1.0f );

		if( ImGui::Button( "Button" ) )
		{
			extra_window.counter++;
		}

		ImGui::SameLine();
		ImGui::Text( "counter = %d", extra_window.counter );
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		glfwSwapBuffers( extra_window.glfwWindow );
		// }
	}
}

int main( int, char ** )
{
	renderLoop();
}
