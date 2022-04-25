#pragma once
#include "gameobject.h"
#include "ObjMgr.h"

class CEffect :
	public CGameObject
{
public:
	CEffect(void);
	virtual ~CEffect(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
//	void SetObjectDir(D3DXVECTOR3 vDir) { m_tInfo.vDir = vDir; }
	void SetObjecteDir(eObjectDir eDir) { m_eDir = eDir; }

protected:
	DWORD		m_dwCreateCurTime;
	DWORD		m_dwCreateOldTime;

	FRAME			m_tFrame;
	wstring			m_wstrStateKey;

	INFO*		m_pTargetInfo;
	eObjectDir	m_eTargetDir;

	D3DXVECTOR3		m_vDist;
	float			m_fDistX;
	float			m_fDistY;

	CObjMgr*	m_pObjMgr;
};
