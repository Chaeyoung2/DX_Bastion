#pragma once
#include "gameobject.h"

class CMyMouse :
	public CGameObject
{
public:
	CMyMouse(void);
	virtual ~CMyMouse(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void					FrameCheck(void);
	void					MatrixCheck(void);

private:
	FRAME			m_tFrame;
	wstring			m_wstrStateKey;

	float			m_fAngle;

	float			m_fTime;
};


