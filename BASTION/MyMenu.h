#pragma once
#include "scene.h"

class CMyMenu : public CScene
{
public:
	CMyMenu(void);
	virtual ~CMyMenu(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	D3DXMATRIX				m_matBoyWorld;

	void					MenuBoyMatrixCheck();
	void					PressSpaceAlphaBlending();

	void					MenuButtonWithMouse();

	void					AddBandibul();

private:

	float					m_fMenuBoyTransValue;
	float					m_fMenuBoySpeed;
	float					m_fInitSpeed;
	
	float					m_fMenuBoyY;

	bool					m_bIsMenuBoyRisingComplete;
	bool					m_bIsMenuBoyUp;

	bool					m_bIsMouseActivatedAtMenu;



	FRAME					m_tExitFrame;
	FRAME					m_tNewGameFrame;
	int						m_iPressSpaceAlphaValue;
	bool					m_bIsPressSpaceAlphaPlus;

	bool					m_bIsNewGameActivated;
	bool					m_bIsExitActivated;

	RECT					m_tNewGameRect;
	RECT					m_tExitRect;

// 	DWORD					m_dwNextCurTime;
// 	DWORD					m_dwNextOldTime;

	float					m_fTime;
	float					m_fRenderTime;
	float					m_fBandibulTime;

	bool					m_bIsNextScene;

	bool					m_bIsMakeCompleteSub0;


};
