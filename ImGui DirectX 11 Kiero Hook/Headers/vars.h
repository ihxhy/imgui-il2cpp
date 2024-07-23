#pragma once
#include "includes.h"

namespace vars {

	inline bool CameraFovChanger{ false };
	inline float CameraFov{ 120.0f };
	inline bool WallHack = false;
	inline Vector2 screen_size = { 0, 0 };
	inline Vector2 screen_center = { 0, 0 };
	inline D3D11_VIEWPORT viewport;
	inline ImColor PlayerSnaplineColor = ImColor(255.0f / 255, 255.0f / 255, 255.0f / 255);
	inline bool PlayerSnapline = false;
	inline ImVec4 Rainbow = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	inline int currentplayeridx = 0;
}