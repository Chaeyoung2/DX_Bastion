#include "StdAfx.h"
#include "UIObserver.h"
#include "DataSubject.h"

CUIObserver::CUIObserver(void)
{
}

CUIObserver::~CUIObserver(void)
{
}

void CUIObserver::Update( int iMessage, void* pData )
{
	list<void*>*	pDataList = 
		CDataSubject::GetInstance()->GetDataList(iMessage);

	if(NULL == pDataList)
		return;

	list<void*>::iterator	iter = 
		find(pDataList->begin(), pDataList->end(), pData);

	switch(iMessage)
	{
	case PLAYER_DATA:
		m_tData = *((DATA*)(*iter));
		break;

	case PLAYER_MATRIX:
		m_matPlayer = *((D3DXMATRIX*)(*iter));
		break;
	}
}
