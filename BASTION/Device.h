#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice);

private:
	LPDIRECT3D9				m_pD3D; // 컴객체
	LPDIRECT3DDEVICE9		m_pD3dDevice; // 실제 다이렉트
	LPD3DXSPRITE			m_pSprite; // 스프라이트 객체
	LPD3DXFONT				m_pFont;
//	LPD3DXLINE				m_pLine;

	HDC				m_hDC;	

public:
	CDevice(void);
	~CDevice(void);

	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3dDevice; }
	LPD3DXSPRITE	  GetSprite() { return m_pSprite; }
	LPD3DXFONT		  GetFont()   { return m_pFont; }
//	LPD3DXLINE		  GetLine()	  {	return m_pLine;	}


	HRESULT			  Init3D();
	void			  SetParameters(D3DPRESENT_PARAMETERS& d3dp);
	void			  Render_Begin();
	void			  Render_End();
	void			  Render_End(HWND& _hWnd);
	void			  Release();

	void			  ZoomIn();
	void			  ZoomOut();
	int				  m_fX;
	int				  m_fY;
	RECT			  m_tSourRect;
	RECT			  m_tDestRect;

	//bool			  m_bIsMenuToStage1; // 메뉴에서 스테이지1로 갈때. SetScene할때 켜주고, 이때 DestRect를 작게 해준다음에 서서히 줌아웃.

	float			  m_fZoomSpeedX;
	float			  m_fZoomSpeedY;

	float			  m_fTime;
	float			  m_fZoomTime;

	float			  m_fZoomInTime;


	// 자막
	bool					m_bIsMakeCompleteSub1;
};

