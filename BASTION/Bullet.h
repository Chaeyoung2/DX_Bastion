#pragma once
#include "gameobject.h"

class CBullet :
	public CGameObject
{
public:
	CBullet(void);
	virtual ~CBullet(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					MatrixCheck();
	void					FrameCheck();

	void					SettingCollRect();

private:
	FRAME			m_tFrame;
	wstring			m_wstrStateKey;
	int				m_iAtt;
	DWORD			m_dwCreateCurTime;
	DWORD			m_dwCreateOldTime;

public:
	int				GetAtt() { return m_iAtt; }
};
