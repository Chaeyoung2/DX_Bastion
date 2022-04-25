#pragma once

#include "Include.h"

class CDevice;
class CSceneMgr;
class CTimeMgr;
class CObjMgr;
class CTextureMgr;
class CScene
{
public:
	virtual HRESULT	Initialize(void)PURE;
	virtual eOBJECT_RESULT	Update(void)PURE;
	virtual void			Render(void)PURE;
	virtual void			Release(void)PURE;


public:
	CScene(void);
	virtual ~CScene(void);

protected:
	CTimeMgr*		m_pTimeMgr;
	CObjMgr*		m_pObjMgr;
	CTextureMgr*	m_pTextureMgr;
	CDevice*		m_pDevice;
	CSceneMgr*		m_pSceneMgr;
};
