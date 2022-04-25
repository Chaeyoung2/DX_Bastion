#include "StdAfx.h"
#include "DataSubject.h"

IMPLEMENT_SINGLETON(CDataSubject)


CDataSubject::CDataSubject(void)
{
}

CDataSubject::~CDataSubject(void)
{
	Release();
}

void CDataSubject::AddData( int iMessage, void* pData )
{
	if(pData)
	{
		map<int, list<void*>>::iterator iter =
			m_MapData.find(iMessage);

		if(iter == m_MapData.end())
		{
			m_MapData[iMessage] = list<void*>(); // 임시 객체
		}

		m_MapData[iMessage].push_back(pData);
		Notify(iMessage, pData);
	}
}

void CDataSubject::RemoveData( int iMessage, void* pData )
{
	map<int, list<void*>>::iterator mapiter =
		m_MapData.find(iMessage);

	if(mapiter != m_MapData.end())
	{
		list<void*>& rList = m_MapData[iMessage];

		for(list<void*>::iterator iter = rList.begin();
			iter != rList.end(); )
		{
			if( (*iter) == pData )
			{
				iter = rList.erase(iter);
			}
			else
				++iter;
		}
	}
}

void CDataSubject::Release( void )
{
	for(map<int, list<void*>>::iterator iter = m_MapData.begin();
		iter != m_MapData.end(); ++iter)
	{
		iter->second.clear();		// 리스트 삭제
	}
	m_MapData.clear(); // 맵 삭제
}

list<void*>* CDataSubject::GetDataList( int iMessage )
{
	map<int, list<void*>>::iterator	iter= m_MapData.find(iMessage);

	if(iter == m_MapData.end())
		return NULL;

	return &iter->second;
}
