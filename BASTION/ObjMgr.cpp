#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"
#include "TextureMgr.h"
#include "Tile.h"
#include "CollisionMgr.h"
#include "Structure.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr(void)
{
	//LoadItemDataFromXml();
}

CObjMgr::~CObjMgr(void)
{
	ReleaseObj();
}

HRESULT CObjMgr::Init(void)
{
	return S_OK;
}

void CObjMgr::AddObject(CGameObject* pObj, eObjectType eID)
{
	m_ObjListArr[eID].push_back(pObj);
}

void CObjMgr::UpdateObj()
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = m_ObjListArr[i].begin();
		OBJITER iter_end = m_ObjListArr[i].end();

		///////////////////////// 컬링 -> 업데이트는 컬링하면 안되겠다..
		// 1) 맵 오브젝트 컬링.
// 		if(i == OBJ_STRUCTURE)
// 		{
// 			for(; iter_begin != iter_end;)
// 			{
// 				if( (*iter_begin)->IsInWindow() == true ) // 윈도우창 안에 있으면
// 				{
// 					if ( OR_DELETE == (*iter_begin)->Update() ) // 업데이트 돌리고
// 					{
// 						Safe_Delete(*iter_begin);
// 						iter_begin = m_ObjListArr[i].erase(iter_begin);
// 					}
// 					else
// 						++iter_begin;
// 				}
// 
// 				else // 윈도우창 안에 없으면 다음으로 넘김.
// 				{
// 					++iter_begin;
// 				}
// 			}
// 		}
// 
// 		else
//		{
			for(; iter_begin != iter_end;)
			{
				if( OR_DELETE == (*iter_begin)->Update() )
				{
					Safe_Delete(*iter_begin);
					iter_begin = m_ObjListArr[i].erase(iter_begin);
				}
				else
					++iter_begin;
			}
		}
//	}

	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_MONSTER], COL_PLAYER_MONSTER);  // 플레이어 몬스터
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_PORTAL], COL_PLAYER_PORTAL);  // 플레이어 포탈
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_MONSTER], COL_HAMMER_MONSTER);  // 해머 몬스터
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_ITEM], COL_PLAYER_ITEM); // 해머 스트럭쳐
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER_BULLET], m_ObjListArr[OBJ_MONSTER], COL_BULLET_MONSTER); // 불렛 몬스터
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER_BULLET], m_ObjListArr[OBJ_STRUCTURE], COL_BULLET_MYOBJECT); // 불렛 스트럭쳐
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_MONSTER_BULLET], m_ObjListArr[OBJ_STRUCTURE], COL_BULLET_MYOBJECT); // 불렛 스트럭쳐
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_MONSTER_BULLET], m_ObjListArr[OBJ_PLAYER], COL_BULLET_PLAYER); // 불렛 플레이어
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_STRUCTURE], COL_HAMMER_MYOBJECT); // 해머 스트럭쳐

}

void CObjMgr::RenderObj()
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = m_ObjListArr[i].begin();
		OBJITER iter_end = m_ObjListArr[i].end();

		// 컬링

		for(; iter_begin != iter_end; ++iter_begin)
		{
			// 해당 씬에서는 컬링 하지 않는다.
			if(g_eScene != SCENE_LOGO && g_eScene != SCENE_LOGO2 
				&& g_eScene != SCENE_MENU)
			{
				// 해당 오브젝트는 컬링 하지 X.
				if(i != OBJ_SCREEN && i != OBJ_UI && i != OBJ_MOUSE && i != OBJ_EFFECT 
					&& i != OBJ_TILE && i != OBJ_GROUND && i != OBJ_ITEM)
				{
					if( (*iter_begin)->IsInWindow() == false) // 창밖에 있으면 렌더 X
						continue;
				}
			}
			

			eRenderID eRenID = (*iter_begin)->GetRenderID();

			if(RENDER_END == (*iter_begin)->GetRenderID() ) // y소팅 안할 애들은 곧바로 출력
			{
				(*iter_begin)->Render(); // 렌더
			}
			else // y소팅 시켜줄 녀석들만 골라서 추린다.
			{
				m_RenderList[eRenID].push_back(*iter_begin);
			}
		}
	}	

	// y소팅 진행.
	m_RenderList[RENDER_OBJ].sort( CompareY<CGameObject*> );	
	m_RenderList[RENDER_STRUCTURE_BACKWARD].sort( CompareY<CGameObject*> );	
	m_RenderList[RENDER_STRUCTURE_FORWARD].sort( CompareY<CGameObject*> );	
	

	for(int i = 0; i < RENDER_END; ++i)
	{
		OBJITER iter = m_RenderList[i].begin();
		OBJITER iter_end = m_RenderList[i].end();

		if(g_bIsUIRender == false)
		{
			if( i == RENDER_UI )
				continue;
		}

		for(; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}

		m_RenderList[i].clear();
	}


}

void CObjMgr::ReleaseObj()
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = m_ObjListArr[i].begin();
		OBJITER iter_end = m_ObjListArr[i].end();

		if(i == OBJ_PLAYER || i == OBJ_SCREEN || i == OBJ_UI || i == OBJ_MOUSE)
			continue;

		for(; iter_begin != iter_end; ++iter_begin)
		{
			Safe_Delete(*iter_begin);
		}
		m_ObjListArr[i].clear();
	}
}

void CObjMgr::ReleaseObj(eObjectType eType)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		if((eObjectType)i == eType)
			continue;

		OBJITER iter_begin = m_ObjListArr[i].begin();
		OBJITER iter_end = m_ObjListArr[i].end();

		for(; iter_begin != iter_end; ++iter_begin)
		{
			Safe_Delete(*iter_begin);
		}
		m_ObjListArr[i].clear();
	}
}

OBJLIST* CObjMgr::GetList(void)
{
	return m_ObjListArr;
}

const vector<PTILE>* CObjMgr::GetTile()
{
	return dynamic_cast<CTile*>(m_ObjListArr[OBJ_TILE].front())->GetTile();
}

// void CObjMgr::LoadItemDataFromXml(void)
// {
// 	MSXML2::IXMLDOMDocument2Ptr pDoc = NULL;
// 
// 	pDoc.CreateInstance( __uuidof(DOMDocument40) );
// 
// 	pDoc->async			  = VARIANT_FALSE;
// 	pDoc->validateOnParse = VARIANT_TRUE;
// 	pDoc->resolveExternals= VARIANT_TRUE;
// 
// 	if(pDoc->load( (_variant_t)"..\\xml\\ItemPath.xml" ) == E_FAIL)
// 	{
// 		MessageBox(g_hWnd, L"xml 로드 실패!!", L"에러!", MB_OK);
// 		return ;
// 	}
// 
// 
// 	//찾은 노드들을 저장할 인터페이스
// 	MSXML2::IXMLDOMNodeListPtr	pElementList = NULL;
// 
// 	//xml 내용을 임시로 저장할 변수
// 	CString strTemp = L"";
// 
// 	
// 	//루프체크 어ㅘ 검색을 위한 변수.
// 	int iCount = 0;
// 	int iTotal = 0;
// 
// 
// 	pElementList = pDoc->selectNodes(L"//ITEM");
// 
// 	iTotal = pElementList->Getlength();
// 
// 	for(int i = 0; i < iTotal; ++i)
// 	{
// 		ITEM_DATA* pData = new ITEM_DATA;
// 
// 		pData->wstrName = pElementList->Getitem(i)->selectNodes(L"//NAME")->Getitem(i)->Gettext();
// 		pData->wstrObjKey = pElementList->Getitem(i)->selectNodes(L"//OBJKEY")->Getitem(i)->Gettext();
// 		pData->wstrImagePath = pElementList->Getitem(i)->selectNodes(L"//PATH")->Getitem(i)->Gettext();
// 
// 		pData->iAtt = atoi(pElementList->Getitem(i)->selectNodes(L"//ATT")->Getitem(i)->Gettext());
// 		pData->iArmor = atoi(pElementList->Getitem(i)->selectNodes(L"//ARMOR")->Getitem(i)->Gettext());
// 		pData->iHpRegen = atoi(pElementList->Getitem(i)->selectNodes(L"//HPREGEN")->Getitem(i)->Gettext());
// 		pData->iPrice = atoi(pElementList->Getitem(i)->selectNodes(L"//PRICE")->Getitem(i)->Gettext());
// 		pData->eType = (eItemType)atoi(pElementList->Getitem(i)->selectNodes(L"//TYPE")->Getitem(i)->Gettext());
// 		
// 		m_ItemDataList.push_back(pData);
// 	}
// 
// 	//아이템 이미지도 로드를 한다.
// 
// 	for(list<ITEM_DATA*>::iterator iter = m_ItemDataList.begin();
// 		 iter != m_ItemDataList.end(); ++iter)
// 	{
// 		CTextureMgr::GetInstance()->InsertTexture( (*iter)->wstrImagePath.c_str(), 
// 													(*iter)->wstrObjKey.c_str(),
// 													TEX_SINGLE);
// 	}
// 
// 	int iA = 10;
// 
// 	/*
// 	for(list<ITEM_DATA*>::iterator iter = m_ItemDataList.begin();
// 	iter != m_ItemDataList.end(); ++iter)
// 	{
// 		safe_delete(*iter);
// 	}
// 	*/
// 	
//}
