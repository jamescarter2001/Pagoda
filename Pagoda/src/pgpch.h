#pragma once

// C++

#include <algorithm>
#include <atomic>
#include <codecvt>
#include <fstream>
#include <functional>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <locale>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>

// Windows

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>

// Direct3D

#include <wrl/client.h>

using namespace Microsoft::WRL;

#include <directx/d3dx12.h>        // D3D interface
#include <d3d11.h> 
#include <dxgi1_4.h>         // DirectX driver interface
#include <d3dcompiler.h>  // shader compiler

// Pagoda

#include "pg_core.h"
#include "math/pg_math.h"