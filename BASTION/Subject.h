#pragma once
#include "Include.h"

class CObserver;
class CSubject
{
protected:
	list<CObserver*>		m_ObserverList;

public:
	void	Subscribe(CObserver* pObserver);			// 관찰자 등록
	void	UnSubscribe(CObserver* pObserver);			// 관찰자 해제
	void	Notify(int iMessage, void* pData = NULL);	// 관찰자에 갱신 명령
	void	Release();
public:
	CSubject(void);
	virtual ~CSubject(void);
};
