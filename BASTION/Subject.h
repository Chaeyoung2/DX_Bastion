#pragma once
#include "Include.h"

class CObserver;
class CSubject
{
protected:
	list<CObserver*>		m_ObserverList;

public:
	void	Subscribe(CObserver* pObserver);			// ������ ���
	void	UnSubscribe(CObserver* pObserver);			// ������ ����
	void	Notify(int iMessage, void* pData = NULL);	// �����ڿ� ���� ���
	void	Release();
public:
	CSubject(void);
	virtual ~CSubject(void);
};
