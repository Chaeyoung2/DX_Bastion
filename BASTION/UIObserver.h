#pragma once
#include "observer.h"

class CUIObserver
	: public CObserver
{
private:
	DATA		m_tData;
	D3DXMATRIX	m_matPlayer;

public:
	CUIObserver(void);
	virtual ~CUIObserver(void);

public:
	const DATA*			 GetData(void) { return &m_tData; }
	const D3DXMATRIX*	 GetMatrix(void) { return &m_matPlayer; }
public:
	virtual void Update(int iMessage, void* pData);
};
