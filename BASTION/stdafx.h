// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")



//##���̷�Ʈ�� ����ϱ� ���� ���
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
#include <fstream>		//##C++����°�ü�� ����ϱ� ���ؼ� �߰��Ѵ�.

#include <atlimage.h>		//CImage�� ����ϱ� ���� �����.





#include <Shlwapi.h>
#pragma  comment (lib, "shlwapi.lib")

//##MS xml����� ���ؼ� dll�� ����Ʈ����.
// #import <msxml4.dll>
// using namespace MSXML2;		
//MSXML2 ->������������ �������ؼ� �ٸ� ������Ʈ���� �ս��� �ε带 �Ҽ� �ֵ��� �F.
