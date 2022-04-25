#pragma once

#include "Include.h"

class CTimeMgr;
class CSoundMgr;
class CDevice;
class CSceneMgr;
class CMainGame
{
private:
	CDevice*			m_pDevice;
	CSceneMgr*			m_pScene;
	
	CTimeMgr*			m_pTimeMgr;

	CSoundMgr*			m_pSoundMgr;

	// FPS
	float				m_fTime;
	int					m_iCount;
	TCHAR				m_szFPS[MIN_STR];
	D3DXMATRIX			m_matFpsTrans;

public:
	CMainGame(void);
	~CMainGame(void);

	HRESULT				Initialize();
	eOBJECT_RESULT		Update();
	void				Render();
	void				Release();

	void				FpsRender();

};
