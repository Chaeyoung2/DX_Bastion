#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice);

private:
	LPDIRECT3D9				m_pD3D; // �İ�ü
	LPDIRECT3DDEVICE9		m_pD3dDevice; // ���� ���̷�Ʈ
	LPD3DXSPRITE			m_pSprite; // ��������Ʈ ��ü
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

	//bool			  m_bIsMenuToStage1; // �޴����� ��������1�� ����. SetScene�Ҷ� ���ְ�, �̶� DestRect�� �۰� ���ش����� ������ �ܾƿ�.

	float			  m_fZoomSpeedX;
	float			  m_fZoomSpeedY;

	float			  m_fTime;
	float			  m_fZoomTime;

	float			  m_fZoomInTime;


	// �ڸ�
	bool					m_bIsMakeCompleteSub1;
};

