#pragma once
#include "gameobject.h"

#include "DataSubject.h"

class CObserver;
class CUI :
	public CGameObject
{
protected:
	CObserver*	m_pObserver;
	CDataSubject* m_pDataSubject;
	wstring		m_wstrStateKey;

public:
	CUI(void);
	virtual ~CUI(void);

public:
	virtual HRESULT Initialize(void)PURE;
	virtual eOBJECT_RESULT Update(void)PURE;
	virtual void Render(void)		PURE;
	virtual void Release(void)		PURE;


};
