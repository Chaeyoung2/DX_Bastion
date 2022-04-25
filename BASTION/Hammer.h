#pragma once
#include "item.h"

class CHammer :
	public CItem
{
public:
	CHammer(void);
	virtual ~CHammer(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
//	void					FrameCheck(void);
//	void					MatrixCheck(void);
};
