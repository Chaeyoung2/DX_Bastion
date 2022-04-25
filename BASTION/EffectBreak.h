#pragma once
#include "effect.h"

class CEffectBreak :
	public CEffect
{
public:
	CEffectBreak(void);
	virtual ~CEffectBreak(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

	void SetBreakID(eBreakID eID) { m_eBreakID = eID; }

private:
	eBreakID	m_eBreakID;
};
