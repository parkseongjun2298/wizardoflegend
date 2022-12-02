// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectX
// �׷��� ��ġ�� �ٷ�� ���� �������̽����� �����ϴ� API
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// Standard Headers
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include "fmod.h"
#pragma comment(lib,"fmodex_vc.lib")
// ������ ���� ������� ���� �ش� 
#include <io.h>
// User Headers
#include "Define.h"
#include "Enum.h"
#include "Extern.h"
#include "Struct.h"
#include "Function.h"
#include <process.h>

// Managers
#include "DeviceMgr.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "ObjectMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Functor.h"


//#ifdef _DEBUG
//#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
//#endif

