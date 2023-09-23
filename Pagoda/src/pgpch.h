#pragma once

// C++

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <iterator>

// Windows

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>

// Direct3D11

// #include <d3d11.h>        // D3D interface
// #include <dxgi.h>         // DirectX driver interface
// #include <d3dcompiler.h>  // shader compiler

// Direct3D12

#include <wrl/client.h>

using namespace Microsoft::WRL;

#include <directx/d3dx12.h>        // D3D interface
#include <d3d11.h> 
#include <dxgi1_4.h>         // DirectX driver interface
#include <d3dcompiler.h>  // shader compiler

// Pagoda

#include "pg_core.h"