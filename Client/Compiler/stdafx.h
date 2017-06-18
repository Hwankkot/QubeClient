// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#define NOMINMAX
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// STL ���
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>

// DirectX
#include <d3dx9.h>
#include <d3d9.h>


//fmod���� ���̺귯�� ����
#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#pragma comment(lib, "fmodex_vc.lib")


// Debug
#include <crtdbg.h>

#include <assert.h> // for Debuging

#define  DIRECTINPUT_VERSION 0x0800
#include <dinput.h>