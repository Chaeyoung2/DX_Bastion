#pragma once
#include "effect.h"

class CEffectAttack1 :
	public CEffect
{
public:
	CEffectAttack1(void);
	virtual ~CEffectAttack1(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);
};
