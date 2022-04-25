#pragma once
#include "gameobject.h"

class CSubTitle :
	public CGameObject
{
public:
	CSubTitle(void);
	virtual ~CSubTitle(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

	void SetFrame(float fFrame)
	{
		m_tFrame.fFrame = fFrame;
	}

private:
	FRAME					m_tFrame;
	wstring					m_wstrStateKey;

	float					m_fCreateTime;

	bool					m_bIsShowed;
};
