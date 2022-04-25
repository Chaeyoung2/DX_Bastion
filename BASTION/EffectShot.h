#pragma once
#include "effect.h"

class CEffectShot :
	public CEffect
{
public:
	CEffectShot(void);
	virtual ~CEffectShot(void);
public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

private:
	eBreakID	m_eBreakID;
};
