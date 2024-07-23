#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "vars.h"
#include "vec.h"
#include "vec2.h"
#include "il2cpp.h"
#include "../Extern/resolver/Resolver.hpp"
#include "../Extern/kiero/minhook/include/MinHook.h"
#include "../Extern/kiero/kiero.h"
#include "../Extern/imgui/imgui.h"
#include "../Extern/imgui/imgui_impl_win32.h"
#include "../Extern/imgui/imgui_impl_dx11.h"


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;
