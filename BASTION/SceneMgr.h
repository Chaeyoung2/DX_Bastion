#pragma once

#include "Include.h"

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr);

private:
	CScene*			m_pScene;

	bool			m_bChangeCheck;
	eSceneType		m_eSceneType;

public:
	HRESULT			Initialize();
	eOBJECT_RESULT	Update();
	void			Render();
	void			Release();

	void			SetScene(eSceneType eType);
	void			SetChangeScene(eSceneType eType);

public:
	CSceneMgr(void);
	~CSceneMgr(void);
};
