#pragma once
#include "gameobject.h"

class CDBox :
	public CGameObject
{
public:
	CDBox(void);
	virtual ~CDBox(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void					FrameCheck(void);
	void					MatrixCheck(void);
	void					SettingCollRect(float fFrame);

public:
	void SetFrame(float fFrame)
	{
		m_tFrame.fFrame = fFrame;
	}

private:
	FRAME					m_tFrame;
	bool					m_bIsTransform;
	float					m_fTransSpeed;
	float					m_fMoveSpeed;

	bool					m_bIsTransformUp;

	bool					m_bIsMakeCompleteSub19;
};
