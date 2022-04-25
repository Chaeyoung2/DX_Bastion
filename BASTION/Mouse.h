#pragma once

#include "Constant.h"

//abstract : 추상클래스로 만들어 주는 키워드.
class CMouse abstract
{
public:
	static D3DXVECTOR3 GetMousePos(void)
	{
		D3DXVECTOR3 vPos;

		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		vPos = D3DXVECTOR3((float)ptMouse.x, (float)ptMouse.y, 0.f);

		return vPos;
	}

};
