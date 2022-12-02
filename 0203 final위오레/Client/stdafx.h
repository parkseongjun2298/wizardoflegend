// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectX
// 그래픽 장치를 다루기 위한 인터페이스들을 제공하는 API
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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
// 저수준 파일 입출력을 위한 해더 
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

