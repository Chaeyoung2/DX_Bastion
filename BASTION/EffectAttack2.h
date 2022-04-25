#pragma once
#include "effect.h"

class CEffectAttack2 :
	public CEffect
{
public:
	CEffectAttack2(void);
	virtual ~CEffectAttack2(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);
};
