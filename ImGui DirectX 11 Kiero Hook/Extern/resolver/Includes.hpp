#pragma once

// Disable Asserts
// #define IL2CPP_ASSERT(x) ((void)(x))
#include "Defines.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <unordered_map>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

// Unity
#include "Unity/Includes.hpp"

// IL2CPP API
#include "Data.hpp" // Pointers, Variables, etc...
#include "Cache.hpp"

// IL2CPP
#include "Funcations/Callback.hpp"
#include "Funcations/Class.hpp"
#include "Funcations/Domain.hpp"
#include "Funcations/ResolveCall.hpp"
#include "Funcations/String.hpp"
#include "Funcations/Thread.hpp"
#include "../resolver/Resolver.hpp"

// Unity API - Include here so we have access to some cute IL2CPP Features
#include "Unity/API/_TEMPLATE_API.hpp"
#include "Unity/API/Object.hpp"
#include "Unity/API/Component.hpp"
#include "Unity/API/GameObject.hpp"
#include "Unity/API/Camera.hpp"
#include "Unity/API/LayerMask.hpp"
#include "Unity/API/Rigidbody.hpp"
#include "Unity/API/Transform.hpp"

// Utils
#include "Utils/Helper.hpp"
#include "Utils/Joaat.hpp"
#include "Utils/VFunc.hpp"