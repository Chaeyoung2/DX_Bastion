#pragma once

#include "Subject.h"

class CDataSubject : public CSubject
{
	DECLARE_SINGLETON(CDataSubject)
private:
	map<int, list<void*> >		m_MapData;

public:
	void	AddData(int iMessage, void* pData);
	void	RemoveData(int iMessage, void* pData);
	void	Release(void);

	list<void*>*		GetDataList(int iMessage);

public:
	CDataSubject(void);
	~CDataSubject(void);
};
