// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")



//##다이렉트를 사용하기 위한 헤더
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma  comment(lib, "d3dx9.lib")

#include <d3dx9math.h>

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>		//##C++입출력객체를 사용하기 위해서 추가한다.

#include <atlimage.h>		//CImage를 사용하기 위한 ㅎ헤더.





#include <Shlwapi.h>
#pragma  comment (lib, "shlwapi.lib")

//##MS xml사용을 위해서 dll을 임포트하자.
// #import <msxml4.dll>
// using namespace MSXML2;		
//MSXML2 ->낮은버전으로 제작을해서 다른 프로젝트에서 손쉽게 로드를 할수 있도록 핮.
