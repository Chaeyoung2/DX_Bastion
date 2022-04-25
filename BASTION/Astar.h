#pragma once

#include "Include.h"
#include "ObjMgr.h"

class CPlayer;
class CAStar
{
	DECLARE_SINGLETON(CAStar);

private:
	list<NODE*>		m_OpenList;		//조사할 대상.
	list<NODE*>		m_CloseList;	//이미 조사한 대상.
	list<int>		m_BestList;		//가야할 최단 경로를 보관한다.

	int				m_iStartIndex;
	int				m_iEndIndex;

	CObjMgr*		m_pObjMgr;
	CPlayer*		m_pPlayer;

	const vector<TILE*>*  m_pVecTile;

	int			m_iTileX;
	int			m_iTileY;

public:
	list<int>*		GetBestList(void)
	{
		return &m_BestList;
	}

public:
	void		AStarStart(const int& iStartIndex, const int& iEndIndex);
	void		MakeRoot(void);
	NODE*		MakeNode(int iIndex, NODE* pParent, const vector<PTILE>* pTile);

	bool		CheckList(const int& iIndex);

public:
	static bool Compare(const NODE* pNode1, const NODE* pNode2);


public:
	void		Release(void);

	bool		Picking(const D3DXVECTOR3& vPos, const TILE* pTile);

	int			GetTileIndex(const D3DXVECTOR3& vPos);

	int			AStarStartPos(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vEndPos);



public:
	CAStar(void);
	~CAStar(void);
};
