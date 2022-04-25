#pragma once
#include "scene.h"

class CTimeMgr;
class CObjMgr;
class CTextureMgr;
class CDevice;
class CSceneMgr;
class CLogo2 :
	public CScene
{
public:
	CLogo2(void);
	virtual ~CLogo2(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					AddBandibul();
	void					MenuLogoMatrixCheck();

/*	void					PressSpaceAlphaBlending();*/
	void					PressSpaceFrameCheck();

	float					m_fTime;

	D3DXMATRIX				m_matMenuLogoWorld;

	float					m_fTransYValue;
	float					m_fTransYSpeed;

	bool					m_bIsFallingLogo;

// 	int						m_iPressSpaceAlphaValue;
// 	bool					m_bIsPressSpaceAlphaPlus;
	FRAME					m_tSpaceFrame;

};
