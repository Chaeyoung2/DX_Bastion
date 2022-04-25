#include "StdAfx.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "Player.h"
#include "Monster.h"
#include "EffectHammer.h"
#include "Bullet.h"
#include "MapObject.h"
#include "Item.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "DataSubject.h"

CCollisionMgr::CCollisionMgr(void)
{
}

CCollisionMgr::~CCollisionMgr(void)
{
}

void CCollisionMgr::CollisionRect(OBJLIST& DstList, OBJLIST& SrcList, eCollisionID eID /*= COL_END*/)
{
	// 각 리스트를 순회한다.
	OBJITER dst_begin = DstList.begin();
	OBJITER dst_end = DstList.end();

	RECT rcTemp = {};

//	if(true == g_bIsSceneChange)
//		return;
	if ( DstList.empty() == true)
		return;

	if( dst_begin == dst_end)
		return;

	for(; dst_begin != dst_end; ++dst_begin)
	{
		OBJITER src_begin = SrcList.begin();
		OBJITER src_end = SrcList.end();

		if (src_begin == src_end)
			return;

		if ( SrcList.empty() == true)
			return;

		if((*dst_begin)->GetDeadState())
			continue;

		for(; src_begin != src_end; ++src_begin )
		{
			if( (*src_begin)->GetDeadState() )
				continue;

			// 1) 오브젝트 둘 다 히트박스로 충돌하는 경우
			if(IntersectRect(&rcTemp, ((*dst_begin)->GetCollRect()), ((*src_begin)->GetCollRect())))
			{
				switch(eID)
				{
				case COL_PLAYER_MONSTER:
					{
						int iDamage = dynamic_cast<CMonster*>(*src_begin)->GetData().iAtt;
						dynamic_cast<CPlayer*>(*dst_begin)->SetDamage(iDamage);
						dynamic_cast<CPlayer*>(*dst_begin)->SetInvincible(true); // 한대 맞았으니까 잠시 무적 상태 해놔야됨
					}
					break;
				case COL_BULLET_MONSTER:
					{
						if(dynamic_cast<CMonster*>(*src_begin)->GetIsInvincible() == false)
						{
							dynamic_cast<CMonster*>(*src_begin)->SetMonColID(MONCOL_BULLET);
							int iAtt = dynamic_cast<CBullet*>(*dst_begin)->GetAtt();
							dynamic_cast<CMonster*>(*src_begin)->SetDamage(iAtt);
							dynamic_cast<CMonster*>(*src_begin)->SetIsDamaged(true);
							dynamic_cast<CMonster*>(*src_begin)->SetIsInvincible(true);
							(*dst_begin)->SetIsDead(true);
							break;
						}
					}
				
				case COL_PLAYER_ITEM:
					{
						// 해머 습득
						if( dynamic_cast<CItem*>(*src_begin)->GetItemNumber() == 3)
						{
							if( dynamic_cast<CItem*>(*src_begin)->GetIsGetCompleted() == false ) // 습득이 안된 상태일 때.
							{
								dynamic_cast<CPlayer*>(*dst_begin)->SetIsMeetHammerInit(false);
								dynamic_cast<CItem*>(*src_begin)->SetIsGetCompleted(true);
								dynamic_cast<CPlayer*>(*dst_begin)->SetState(PLAYER_HAMMER);
								(*src_begin)->SetIsDead(true);
							}
						}
						// 건 습득
						if( dynamic_cast<CItem*>(*src_begin)->GetItemNumber() == 4)
						{
							if( dynamic_cast<CItem*>(*src_begin)->GetIsGetCompleted() == false ) // 습득이 안된 상태일 때.
							{
								dynamic_cast<CPlayer*>(*dst_begin)->SetIsMeetGunInit(false);
								dynamic_cast<CItem*>(*src_begin)->SetIsGetCompleted(true);
								dynamic_cast<CPlayer*>(*dst_begin)->SetState(PLAYER_GUN);
								(*src_begin)->SetIsDead(true);
							}
						}
						
					}
					break;
				case COL_BULLET_PLAYER:
					{
						if(dynamic_cast<CPlayer*>(*src_begin)->GetInvincible() == false)
						{
							int iAtt = dynamic_cast<CBullet*>(*dst_begin)->GetAtt();
							dynamic_cast<CPlayer*>(*src_begin)->SetDamage(iAtt);
							/*CDataSubject::GetInstance()->Notify(PLAYER_DATA, &m_tData);*/
							dynamic_cast<CPlayer*>(*src_begin)->SetIsDamaged(true);
							dynamic_cast<CPlayer*>(*src_begin)->SetInvincible(true);
							(*dst_begin)->SetIsDead(true);
							break;
						}

					}
					break;
	

				case COL_PLAYER_PORTAL:
					{
						if( dynamic_cast<CPlayer*>(*dst_begin)->GetReadyToPortal() == true)
						{
// 							if( CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON) )
// 							{
							g_bIsFadeOut = true;
							g_bIsFadeIn = false;	
							dynamic_cast<CPlayer*>(*dst_begin)->SetReadyToPortal(false);

							int iWhereToGo =  dynamic_cast<CPlayer*>(*dst_begin)->GetWhereToGo();

							switch(iWhereToGo)
							{
							case 1:
								CSceneMgr::GetInstance()->SetChangeScene(SCENE_DIA);
								dynamic_cast<CPlayer*>(*dst_begin)->SetWhereToGo(2);
								break;
							case 2:
								CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE2);
								dynamic_cast<CPlayer*>(*dst_begin)->SetWhereToGo(3);
								break;
							}
							
															
/*							}*/	
						}
					}
					// 여기서부터
					break;
				}
			}

			// 2) 플레이어 무기 박스 - 오브젝트 충돌 박스
			
			switch(eID)
			{
			case COL_HAMMER_MONSTER: // 몬스터가 플레이어 해머에 맞았을 때
				{
					if(dynamic_cast<CMonster*>(*src_begin)->GetIsInvincible() == false)
					{
						if(IntersectRect(&rcTemp, 
							(dynamic_cast<CPlayer*>(*dst_begin)->GetHammerRect()), ((*src_begin)->GetCollRect())))
						{
							dynamic_cast<CMonster*>(*src_begin)->SetMonColID(MONCOL_HAMMER);
							int iAtt = dynamic_cast<CPlayer*>(*dst_begin)->GetData().iAtt;
							dynamic_cast<CMonster*>(*src_begin)->SetDamage(iAtt);
							dynamic_cast<CMonster*>(*src_begin)->SetIsDamaged(true);
							dynamic_cast<CMonster*>(*src_begin)->SetIsInvincible(true);
						}
					}
				}
				break;
			case COL_HAMMER_MYOBJECT:
				{
					// 충돌 처리 1 (이펙트 생성) 조건 : 무적 상태가 아니고, 데미지 허용이어야 
					if( (dynamic_cast<CMapObject*>(*src_begin))->GetObjInfo()->bIsInvincible == false
						&& (dynamic_cast<CMapObject*>(*src_begin))->GetObjInfo()->bIsAgreeToDamage == true)
					{
						// IntersectRect로 해머렉트와 각 오브젝트마다의 충돌렉트로 비교
						if( IntersectRect(&rcTemp, dynamic_cast<CPlayer*>(*dst_begin)->GetHammerRect(),
							&(dynamic_cast<CMapObject*>(*src_begin))->GetObjInfo()->tCollRect))
						{
							dynamic_cast<CMapObject*>(*src_begin)->SetIsDamaged(true);
							dynamic_cast<CMapObject*>(*src_begin)->SetIsInvincible(true);
							dynamic_cast<CMapObject*>(*src_begin)->SetCollCount(1);

							// 스트럭쳐의 collcount가 4 이상이 되면 bIsDead = true 처리.
							if( dynamic_cast<CMapObject*>(*src_begin)->GetObjInfo()->iCollCount >= 4 )
							{
								(*src_begin)->SetIsDead(true);
							}
							
						}
					}
					break;
				}

			case COL_BULLET_MYOBJECT:
				{
					if( (dynamic_cast<CMapObject*>(*src_begin))->GetObjInfo()->bIsInvincible == false
						&& (dynamic_cast<CMapObject*>(*src_begin))->GetObjInfo()->bIsAgreeToDamage == true)
					{
						// IntersectRect로 해머렉트와 각 오브젝트마다의 충돌렉트로 비교
						if( IntersectRect(&rcTemp, (*dst_begin)->GetCollRect(),
							&(dynamic_cast<CMapObject*>(*src_begin))->GetObjInfo()->tCollRect))
						{
							(*dst_begin)->SetIsDead(true);
							dynamic_cast<CMapObject*>(*src_begin)->SetIsDamaged(true);
							dynamic_cast<CMapObject*>(*src_begin)->SetIsInvincible(true);
							dynamic_cast<CMapObject*>(*src_begin)->SetCollCount(1);

							// 스트럭쳐의 collcount가 4 이상이 되면 bIsDead = true 처리.
							if( dynamic_cast<CMapObject*>(*src_begin)->GetObjInfo()->iCollCount >= 4 )
							{
								(*src_begin)->SetIsDead(true);
							}

						}
					}
				}
				break;
			}
		}
	}
}

// 			case COL_HAMMER_MYOBJECT:
// 				{
// 					list<PMYOBJ>* pStructureList = dynamic_cast<CStructure*>(*src_begin)->GetStructureList();
// 					list<PMYOBJ>::iterator iter = pStructureList->begin();
// 					list<PMYOBJ>::iterator iter_end = pStructureList->end();
// 
// 					for(; iter != iter_end; ++iter)
// 					{
// 						if( (*iter)->bIsInvincible == false && (*iter)->bIsAgreeToDamage == true
// 							&& (*iter)->iCollCount <= 4)
// 						{
// 							if( IntersectRect(&rcTemp, dynamic_cast<CPlayer*>(*dst_begin)->GetHammerRect(),
// 								&( (*iter)->tCollRect )) )
// 							{
// 								(*iter)->bIsDamaged = true;
// 								(*iter)->bIsInvincible = true;
// 								(*iter)->iCollCount++;
// 								return;
// 							}
// 						}
// 						
// 					}

// 					if(dynamic_cast<CMonster*>(*src_begin)->GetIsInvincible() == false)
// 					{
// 						if(IntersectRect(&rcTemp, 
// 							(dynamic_cast<CPlayer*>(*dst_begin)->GetHammerRect()), ((*src_begin)->GetCollRect())))
// 						{
// 							dynamic_cast<CMonster*>(*src_begin)->SetMonColID(MONCOL_HAMMER);
// 							int iAtt = dynamic_cast<CPlayer*>(*dst_begin)->GetData().iAtt;
// 							dynamic_cast<CMonster*>(*src_begin)->SetDamage(iAtt);
// 							dynamic_cast<CMonster*>(*src_begin)->SetIsDamaged(true);
// 							dynamic_cast<CMonster*>(*src_begin)->SetIsInvincible(true);
// 						}
// 					}
//				}
//				break;
// 			}
// 		}
// 	}
// }

 //void CCollisionMgr::CollisionRectEX(OBJLIST& DstList, OBJLIST& SrcList, eCollisionID eID /*= COL_END*/)
 //{
	// // 각 리스트를 순회한다.
	// OBJITER dst_begin = DstList.begin();
	// OBJITER dst_end = DstList.end();	

	// int iMoveX = 0, iMoveY = 0;

	// for(; dst_begin != dst_end; ++dst_begin)
	// {
	//	 OBJITER src_begin = SrcList.begin();
	//	 OBJITER src_end = SrcList.end();

	//	 //if((*dst_begin)->GetDead())
	//	 //	continue;

	//	 for(; src_begin != src_end; ++src_begin )
	//	 {

	//		 //if((*src_begin)->GetDead())
	//		 //	continue;

	//		 if(CheckRect((*dst_begin), (*src_begin), &iMoveX, &iMoveY))
	//		 {

	//			 if(iMoveX > iMoveY)
	//			 {
	//				 int fX = static_cast<int>((*src_begin)->GetInfo().fX);
	//				 int fY = static_cast<int>((*src_begin)->GetInfo().fY);

	//				 if(fY < (*dst_begin)->GetInfo().fY)
	//					 iMoveY *= -1;

	//				 (*src_begin)->SetPos(static_cast<float>(fX), static_cast<float>(fY) + static_cast<float>(iMoveY));
	//			 }
	//			 else
	//			 {
	//				 int fX = static_cast<int>((*src_begin)->GetInfo().fX);
	//				 int fY = static_cast<int>((*src_begin)->GetInfo().fY);

	//				 if(fX < (*dst_begin)->GetInfo().fX)
	//					 iMoveX *= -1;

	//				 (*src_begin)->SetPos(static_cast<float>(fX) + static_cast<float>(iMoveX), static_cast<float>(fY));
	//			 }
	//		 }	
	//	 }
	// } 
 //}

bool CCollisionMgr::CheckRect(CGameObject* pDst, CGameObject* pSrc, int* pMoveX, int* pMoveY)
{
	int iWidth =  static_cast<int>((pDst->GetInfo()->vSize.x) * 0.5f + (pSrc->GetInfo()->vSize.x) * 0.5f);
	int iHeigth =  static_cast<int>((pDst->GetInfo()->vSize.y) * 0.5f + (pSrc->GetInfo()->vSize.y) * 0.5f);

	int iDistX = static_cast<int>(abs(pDst->GetInfo()->vPos.x - pSrc->GetInfo()->vPos.x));
	int iDistY = static_cast<int>(abs(pDst->GetInfo()->vPos.y - pSrc->GetInfo()->vPos.y));

	if((iWidth >= iDistX) && (iHeigth >= iDistY))
	{
		// 충돌 되면 파고든 x, y거리를 구해주어야 한다.
		// 각 축의 반지름 합에서 거리를 빼면 파고든 x, y를 구할 수 있다.
		*pMoveX = iWidth - iDistX;
		*pMoveY = iHeigth - iDistY;

		return true;
	}

	return false;
}
