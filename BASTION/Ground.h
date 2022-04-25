#pragma once
#include "gameobject.h"

class CGround :
	public CGameObject
{
public:
	CGround(void);
	virtual ~CGround(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	////////////////// 오브젝트 공통
	void					MatrixCheck( list<PMYOBJ>::iterator iter
		, list<PMYOBJ>::iterator iter_end );

	////////////////// FUNC
public:
	void					LoadData();

	////////////////// 
private:
	list<PMYOBJ>	m_GroundList;
	wstring			m_wstrStateKey;
};
