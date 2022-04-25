#pragma once
#include "effect.h"

class CEffectSpin :
	public CEffect
{
public:
	CEffectSpin(void);
	virtual ~CEffectSpin(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);
};
