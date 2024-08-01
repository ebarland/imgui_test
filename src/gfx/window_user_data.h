#pragma once
#include "imgui_internal.h"
#include "../gfx/camera.h"

namespace gfx
{
	struct window_user_data
	{
		gfx::camera * cam;
		ImGuiContext * imgui_context;
	};
} // namespace gfx
