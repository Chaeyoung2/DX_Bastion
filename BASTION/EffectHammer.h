#pragma once
#include "effect.h"

class EffectHammer :
	public CEffect
{
public:
	EffectHammer(void);
	virtual ~EffectHammer(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

};
