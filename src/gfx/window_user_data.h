#pragma once
#include "camera.h"
#include "imgui_internal.h"

namespace gfx
{
	struct window_user_data
	{
		gfx::camera * cam;
		ImGuiContext * imgui_context;
		std::string window_name;
	};
} // namespace gfx
