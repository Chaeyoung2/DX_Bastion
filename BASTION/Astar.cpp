#include "StdAfx.h"
#include "Astar.h"
#include "Tile.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CAStar);


void CAStar::AStarStart(const int& iStartIndex,
						const int& iEndIndex)
{
	// 스테이지에 따른 타일 개수

	switch(g_eScene)
	{
	case SCENE_STAGE1:
		m_iTileX = TILEX;
		m_iTileY = TILEY;
		break;
	case SCENE_DIA:
		m_iTileX = TILEXDIA;
		m_iTileY = TILEYDIA;
		break;
	case SCENE_STAGE2:
		m_iTileX = TILEX;
		m_iTileY = TILEY;
		break;
	}


	if(iStartIndex == iEndIndex)
		return;

	const vector<TILE*>* pVecTile = m_pObjMgr->FindGameObject<CTile>()->GetTile();

	if(m_pVecTile == NULL)
		return;

 	int iTileOption = (*m_pVecTile)[iEndIndex]->byOption;

	if(iTileOption == 0)
		return;
// 
// 	switch(iTileOption)
// 	{
// 	case 0: //이동 불가타일이라면
// 		return;
// 	case 1:
// 		break;
// 	case 2:
// 		{
// 	//		CGameObject* pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetPlayer();
// 	//		dynamic_cast<CPlayer*>(pPlayer)->SetState(PLAYER_DROP);
// 		}
// 		break;
// 	case 5:
// 		break;
// 	default:
// 		return;
// 	}

	Release();

	m_iStartIndex = iStartIndex;
	m_iEndIndex   = iEndIndex;

	MakeRoot();
}

void CAStar::MakeRoot(void)
{
	NODE* pNode = new NODE;

	pNode->iIndex = m_iStartIndex;
	//최초생성된 노드는 부모가 없다.
	pNode->pParent = NULL;
	pNode->fCost   = 0.f;

	//시작점은 CloseList에 담고 시작을 한다.
	m_CloseList.push_back(pNode);

	//const vector<TILE*>* pVecTile = m_pObjMgr->FindGameObject<CTile>()->GetTile();

	if(m_pVecTile == NULL)
		return;

	NODE* pMakeNode = NULL;
	int iIndex		= 0;

	int iTileOption = 0;

	while(true)		//탐색
	{
		//1.타일이 맵상에 존재하는 타일인가?
		//2.타일의 옵션이 갈수 있는 타일인지?
		//3.Open & CloseList에 있는 Index인지?

		//##위쪽 타일
		//iIndex = pNode->iIndex - 40;

		iIndex = pNode->iIndex - m_iTileX * 2;

		
		if( pNode->iIndex >= m_iTileX * 2			&&
			CheckList(iIndex) )
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4 )
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);

				// 자막 타일일 때
				if(iTileOption == 5)
				{

				}
			}
			
		}


		//##아래쪽 타일
		iIndex = pNode->iIndex + m_iTileX * 2;

		if( pNode->iIndex < (m_iTileX * m_iTileY) - (m_iTileX * 2) &&
			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
			
		}


		//##왼쪽 타일
		iIndex = pNode->iIndex - 1;

		if( pNode->iIndex % m_iTileX != 0  && 
			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
		}


		//##오른쪽 타일
		iIndex = pNode->iIndex + 1;

		if( pNode->iIndex % m_iTileX != m_iTileX - 1  &&
			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
		}


		//##오른쪽 위 타일
		iIndex = pNode->iIndex - (m_iTileX - (pNode->iIndex / m_iTileX) % 2 );

		//예외처리가 추가적으로 더 필요하다.
		if( pNode->iIndex >= m_iTileX - 1  &&
			pNode->iIndex % (m_iTileX) != (m_iTileX - 1) &&
			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
		}


		//##오른쪽 아래 타일
		iIndex = pNode->iIndex + (m_iTileX + (pNode->iIndex / m_iTileX) % 2 );

		//예외처리가 추가적으로 더 필요하다.
		if( pNode->iIndex < (m_iTileX * m_iTileY) - m_iTileX  &&
			pNode->iIndex % (m_iTileX) != (m_iTileX - 1) &&
			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
		}


		//##왼쪽 아래 타일
		iIndex = pNode->iIndex + (m_iTileX + (pNode->iIndex / m_iTileX) % 2 ) - 1;

		//예외처리가 추가적으로 더 필요하다.
		if( pNode->iIndex < (m_iTileX * m_iTileY) - m_iTileX  &&
			pNode->iIndex % (m_iTileX) != 0 &&			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
		}


		//##왼쪽 위 타일
		iIndex = pNode->iIndex - (m_iTileX - (pNode->iIndex / m_iTileX) % 2 ) - 1;

		//예외처리가 추가적으로 더 필요하다.
		if( pNode->iIndex >= m_iTileX - 1  &&
			pNode->iIndex % (m_iTileX) != 0 &&
			CheckList(iIndex)						)
		{
			iTileOption = (*m_pVecTile)[iIndex]->byOption;

			if( iTileOption == 1 || iTileOption == 2 || iTileOption == 3 || iTileOption == 4)
			{
				pMakeNode = MakeNode(iIndex, pNode, m_pVecTile);
				m_OpenList.push_back(pMakeNode);
			}
		}

		//비용기준으로 정렬을 한다.
		m_OpenList.sort(Compare);

		list<NODE*>::iterator iter = m_OpenList.begin();

		pNode = (*iter);

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if(pNode->iIndex == m_iEndIndex)
		{
			//경로 루트 생성이 완료.

			while(true)
			{
				m_BestList.push_back(pNode->iIndex);

				pNode = pNode->pParent;

				if(pNode->iIndex == m_iStartIndex)
					break;
			}

			m_BestList.reverse();
			//최종 경로를 얻어낸다.

			break;
		}

	}
}

NODE* CAStar::MakeNode(int iIndex, 
					   NODE* pParent, 
					   const vector<PTILE>* pTile)
{
	NODE* pNode = new NODE;

	pNode->iIndex  = iIndex;
	pNode->pParent = pParent;

	//비용 검사.
	D3DXVECTOR3 vDist = (*pTile)[iIndex]->vPos - (*pTile)[pParent->iIndex]->vPos;

	//플레이어 위치에서 인접한 노드의 거리
	float fCostPlayer = D3DXVec3Length(&vDist);


	vDist = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	//인접노드에서 도착지점까지의 거리.
	float fCostEnd = D3DXVec3Length(&vDist);


	//플레이어위치에서 목표지점까지의 거리 합산.
	pNode->fCost = fCostPlayer + fCostEnd;

	return pNode;
}

bool CAStar::CheckList(const int& iIndex)
{
	for(list<NODE*>::iterator iter = m_CloseList.begin(); 
		iter != m_CloseList.end(); ++iter)
	{
		if( (*iter)->iIndex == iIndex )
			return false;
	}

	for(list<NODE*>::iterator iter = m_OpenList.begin(); 
		iter != m_OpenList.end(); ++iter)
	{
		if( (*iter)->iIndex == iIndex )
			return false;
	}

	return true;
}

bool CAStar::Compare(const NODE* pNode1, const NODE* pNode2)
{
	return (pNode1->fCost < pNode2->fCost);
}

void CAStar::Release(void)
{
	for_each(m_OpenList.begin(), m_OpenList.end(), DeleteObj());
	m_OpenList.clear();

	for_each(m_CloseList.begin(), m_CloseList.end(), DeleteObj());
	m_CloseList.clear();

	m_BestList.clear();
}

bool CAStar::Picking(const D3DXVECTOR3& vPos
					 , const TILE* pTile)
{
	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3(pTile->vPos.x, 
		pTile->vPos.y - TILECY / 2.f, 
		0.f),

		D3DXVECTOR3(pTile->vPos.x + TILECX / 2.f, 
		pTile->vPos.y, 
		0.f),

		D3DXVECTOR3(pTile->vPos.x, 
		pTile->vPos.y + TILECY / 2.f, 
		0.f),

		D3DXVECTOR3(pTile->vPos.x - TILECX / 2.f, 
		pTile->vPos.y, 
		0.f)
	};


	//방향벡터를 만들어 보자.
	D3DXVECTOR3 vDir[4] = 
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들자
	//어느한벡터에 직각이 되는 벡터를 의미한다.
	D3DXVECTOR3 vNormal[4] = 
	{
		D3DXVECTOR3(vDir[0].y, -vDir[0].x, 0.f),
		D3DXVECTOR3(vDir[1].y, -vDir[1].x, 0.f),
		D3DXVECTOR3(vDir[2].y, -vDir[2].x, 0.f),
		D3DXVECTOR3(vDir[3].y, -vDir[3].x, 0.f)
	};


	//법선벡터를 단위벡터로 만들어 주자.
	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);		//단위벡터로 만들어주는 함수.


	//내적을 하자.
	//내적의 결과값이 양수면 타일외부에 있다.
	//				  음수면 타일내부에 있다.

	for(int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vDirTemp = vPos - vPoint[i];
		D3DXVec3Normalize(&vDirTemp, &vDirTemp);

		float fDotResult = D3DXVec3Dot(&vDirTemp, &vNormal[i]);

		if(fDotResult > 0.f)
			return false;
	}

	return true;
}

int CAStar::GetTileIndex(const D3DXVECTOR3& vPos)
{
	//const vector<TILE*>* pVecTile = CObjMgr::GetInstance()->FindGameObject<CTile>()->GetTile();

	for(size_t i = 0; i < m_pVecTile->size(); ++i)
	{
		if(Picking(vPos, (*m_pVecTile)[i]))
		{
			return i;
		}
	}
	return -1;
}

int CAStar::AStarStartPos(const D3DXVECTOR3& vStartPos
						   , const D3DXVECTOR3& vEndPos)
{
	// 플레이어 주소 받음
	m_pPlayer = m_pObjMgr->FindGameObject<CPlayer>();
	m_pVecTile = m_pObjMgr->FindGameObject<CTile>()->GetTile();
	m_iStartIndex = GetTileIndex(vStartPos);
	m_iEndIndex   = GetTileIndex(vEndPos);

	if(m_iEndIndex == -1 || m_iStartIndex == -1)
		return -1;

	AStarStart(m_iStartIndex, m_iEndIndex);

	int iOption = (*m_pVecTile)[m_iEndIndex]->byOption;

	return iOption;
}

CAStar::CAStar(void)
:m_pObjMgr(CObjMgr::GetInstance()),
m_iStartIndex(0), m_iEndIndex(0)

{
}

CAStar::~CAStar(void)
{
	Release();
}
