#pragma once
#include "item.h"

class CGun :
	public CItem
{
public:
	CGun(void);
	virtual ~CGun(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

};
