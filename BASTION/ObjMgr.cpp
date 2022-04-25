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

		///////////////////////// �ø� -> ������Ʈ�� �ø��ϸ� �ȵǰڴ�..
		// 1) �� ������Ʈ �ø�.
// 		if(i == OBJ_STRUCTURE)
// 		{
// 			for(; iter_begin != iter_end;)
// 			{
// 				if( (*iter_begin)->IsInWindow() == true ) // ������â �ȿ� ������
// 				{
// 					if ( OR_DELETE == (*iter_begin)->Update() ) // ������Ʈ ������
// 					{
// 						Safe_Delete(*iter_begin);
// 						iter_begin = m_ObjListArr[i].erase(iter_begin);
// 					}
// 					else
// 						++iter_begin;
// 				}
// 
// 				else // ������â �ȿ� ������ �������� �ѱ�.
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

	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_MONSTER], COL_PLAYER_MONSTER);  // �÷��̾� ����
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_PORTAL], COL_PLAYER_PORTAL);  // �÷��̾� ��Ż
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_MONSTER], COL_HAMMER_MONSTER);  // �ظ� ����
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_ITEM], COL_PLAYER_ITEM); // �ظ� ��Ʈ����
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER_BULLET], m_ObjListArr[OBJ_MONSTER], COL_BULLET_MONSTER); // �ҷ� ����
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER_BULLET], m_ObjListArr[OBJ_STRUCTURE], COL_BULLET_MYOBJECT); // �ҷ� ��Ʈ����
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_MONSTER_BULLET], m_ObjListArr[OBJ_STRUCTURE], COL_BULLET_MYOBJECT); // �ҷ� ��Ʈ����
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_MONSTER_BULLET], m_ObjListArr[OBJ_PLAYER], COL_BULLET_PLAYER); // �ҷ� �÷��̾�
	CCollisionMgr::CollisionRect(m_ObjListArr[OBJ_PLAYER], m_ObjListArr[OBJ_STRUCTURE], COL_HAMMER_MYOBJECT); // �ظ� ��Ʈ����

}

void CObjMgr::RenderObj()
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = m_ObjListArr[i].begin();
		OBJITER iter_end = m_ObjListArr[i].end();

		// �ø�

		for(; iter_begin != iter_end; ++iter_begin)
		{
			// �ش� �������� �ø� ���� �ʴ´�.
			if(g_eScene != SCENE_LOGO && g_eScene != SCENE_LOGO2 
				&& g_eScene != SCENE_MENU)
			{
				// �ش� ������Ʈ�� �ø� ���� X.
				if(i != OBJ_SCREEN && i != OBJ_UI && i != OBJ_MOUSE && i != OBJ_EFFECT 
					&& i != OBJ_TILE && i != OBJ_GROUND && i != OBJ_ITEM)
				{
					if( (*iter_begin)->IsInWindow() == false) // â�ۿ� ������ ���� X
						continue;
				}
			}
			

			eRenderID eRenID = (*iter_begin)->GetRenderID();

			if(RENDER_END == (*iter_begin)->GetRenderID() ) // y���� ���� �ֵ��� ��ٷ� ���
			{
				(*iter_begin)->Render(); // ����
			}
			else // y���� ������ �༮�鸸 ��� �߸���.
			{
				m_RenderList[eRenID].push_back(*iter_begin);
			}
		}
	}	

	// y���� ����.
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
// 		MessageBox(g_hWnd, L"xml �ε� ����!!", L"����!", MB_OK);
// 		return ;
// 	}
// 
// 
// 	//ã�� ������ ������ �������̽�
// 	MSXML2::IXMLDOMNodeListPtr	pElementList = NULL;
// 
// 	//xml ������ �ӽ÷� ������ ����
// 	CString strTemp = L"";
// 
// 	
// 	//����üũ ��� �˻��� ���� ����.
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
// 	//������ �̹����� �ε带 �Ѵ�.
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
