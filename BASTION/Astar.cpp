#include "StdAfx.h"
#include "Astar.h"
#include "Tile.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CAStar);


void CAStar::AStarStart(const int& iStartIndex,
						const int& iEndIndex)
{
	// ���������� ���� Ÿ�� ����

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
// 	case 0: //�̵� �Ұ�Ÿ���̶��
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
	//���ʻ����� ���� �θ� ����.
	pNode->pParent = NULL;
	pNode->fCost   = 0.f;

	//�������� CloseList�� ��� ������ �Ѵ�.
	m_CloseList.push_back(pNode);

	//const vector<TILE*>* pVecTile = m_pObjMgr->FindGameObject<CTile>()->GetTile();

	if(m_pVecTile == NULL)
		return;

	NODE* pMakeNode = NULL;
	int iIndex		= 0;

	int iTileOption = 0;

	while(true)		//Ž��
	{
		//1.Ÿ���� �ʻ� �����ϴ� Ÿ���ΰ�?
		//2.Ÿ���� �ɼ��� ���� �ִ� Ÿ������?
		//3.Open & CloseList�� �ִ� Index����?

		//##���� Ÿ��
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

				// �ڸ� Ÿ���� ��
				if(iTileOption == 5)
				{

				}
			}
			
		}


		//##�Ʒ��� Ÿ��
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


		//##���� Ÿ��
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


		//##������ Ÿ��
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


		//##������ �� Ÿ��
		iIndex = pNode->iIndex - (m_iTileX - (pNode->iIndex / m_iTileX) % 2 );

		//����ó���� �߰������� �� �ʿ��ϴ�.
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


		//##������ �Ʒ� Ÿ��
		iIndex = pNode->iIndex + (m_iTileX + (pNode->iIndex / m_iTileX) % 2 );

		//����ó���� �߰������� �� �ʿ��ϴ�.
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


		//##���� �Ʒ� Ÿ��
		iIndex = pNode->iIndex + (m_iTileX + (pNode->iIndex / m_iTileX) % 2 ) - 1;

		//����ó���� �߰������� �� �ʿ��ϴ�.
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


		//##���� �� Ÿ��
		iIndex = pNode->iIndex - (m_iTileX - (pNode->iIndex / m_iTileX) % 2 ) - 1;

		//����ó���� �߰������� �� �ʿ��ϴ�.
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

		//���������� ������ �Ѵ�.
		m_OpenList.sort(Compare);

		list<NODE*>::iterator iter = m_OpenList.begin();

		pNode = (*iter);

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if(pNode->iIndex == m_iEndIndex)
		{
			//��� ��Ʈ ������ �Ϸ�.

			while(true)
			{
				m_BestList.push_back(pNode->iIndex);

				pNode = pNode->pParent;

				if(pNode->iIndex == m_iStartIndex)
					break;
			}

			m_BestList.reverse();
			//���� ��θ� ����.

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

	//��� �˻�.
	D3DXVECTOR3 vDist = (*pTile)[iIndex]->vPos - (*pTile)[pParent->iIndex]->vPos;

	//�÷��̾� ��ġ���� ������ ����� �Ÿ�
	float fCostPlayer = D3DXVec3Length(&vDist);


	vDist = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	//������忡�� �������������� �Ÿ�.
	float fCostEnd = D3DXVec3Length(&vDist);


	//�÷��̾���ġ���� ��ǥ���������� �Ÿ� �ջ�.
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


	//���⺤�͸� ����� ����.
	D3DXVECTOR3 vDir[4] = 
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//�������͸� ������
	//����Ѻ��Ϳ� ������ �Ǵ� ���͸� �ǹ��Ѵ�.
	D3DXVECTOR3 vNormal[4] = 
	{
		D3DXVECTOR3(vDir[0].y, -vDir[0].x, 0.f),
		D3DXVECTOR3(vDir[1].y, -vDir[1].x, 0.f),
		D3DXVECTOR3(vDir[2].y, -vDir[2].x, 0.f),
		D3DXVECTOR3(vDir[3].y, -vDir[3].x, 0.f)
	};


	//�������͸� �������ͷ� ����� ����.
	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);		//�������ͷ� ������ִ� �Լ�.


	//������ ����.
	//������ ������� ����� Ÿ�Ͽܺο� �ִ�.
	//				  ������ Ÿ�ϳ��ο� �ִ�.

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
	// �÷��̾� �ּ� ����
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
