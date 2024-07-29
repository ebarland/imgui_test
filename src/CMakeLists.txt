link_directories(../external/glfw-3.3.8.bin.WIN64/lib-vc2022/ ../external/glad/src/)
add_executable(imgui_test main.cpp)
target_sources(imgui_test PRIVATE ../external/glad/src/glad.c ../external/imgui/imgui.cpp ../external/imgui/imgui_demo.cpp ../external/imgui/imgui_draw.cpp ../external/imgui/imgui_tables.cpp ../external/imgui/imgui_widgets.cpp ../external/imgui/backends/imgui_impl_glfw.cpp ../external/imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui_test PRIVATE ../external/glfw-3.3.8.bin.WIN64/include/ ../external/glad/include/ ../external/glm/ ../external/imgui/ ../external/imgui/backends/)

target_link_libraries(imgui_test glfw3 OpenGL::GL)
