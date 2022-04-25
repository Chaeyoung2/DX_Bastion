#pragma once
#include "effect.h"

class CEffectPotion :
	public CEffect
{
public:
	CEffectPotion(void);
	virtual ~CEffectPotion(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);
};
