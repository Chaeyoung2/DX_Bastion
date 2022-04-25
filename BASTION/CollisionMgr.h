#pragma once

#include "Include.h"

class CCollisionMgr
{
public:
	CCollisionMgr(void);
	~CCollisionMgr(void);

	static void CollisionRect(OBJLIST& DstList, OBJLIST& SrcList, eCollisionID eID = COL_END);
//	static void CollisionRectEX(OBJLIST& DstList, OBJLIST& SrcList, eCollisionID eID = COL_END);

	static bool CheckRect(CGameObject* pDst, CGameObject* pSrc, int* pMoveX, int* pMoveY);


};
