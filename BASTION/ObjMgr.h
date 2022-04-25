#pragma once

#include "Include.h"

class CGameObject;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr);

public:
	CObjMgr(void);
	~CObjMgr(void);


public:
	HRESULT Init(void);
	void AddObject(CGameObject* pObj, eObjectType eID);
	void UpdateObj();
	void RenderObj();
	void ReleaseObj();
	void ReleaseObj(eObjectType eType);


public:
	OBJLIST*				GetList(void);
	const vector<PTILE>*	GetTile();

// public:
// 	void LoadItemDataFromXml(void);

private:
	OBJLIST	m_ObjListArr[OBJ_END];

	OBJLIST	m_RenderList[RENDER_END];
	OBJLIST m_ForwardStructureList;
	OBJLIST m_BackwardStructureList;
//	list<ITEM_DATA*>	m_ItemDataList;



public:
	template<typename T>
	//CGameObject* FindGameObject(int iIndex = 0)
	T* FindGameObject(int iIndex = 0)
	{
		for(size_t i = 0; i < OBJ_END; ++i)
		{
			OBJITER iter_begin = m_ObjListArr[i].begin();
			OBJITER iter_end = m_ObjListArr[i].end();

			for(; iter_begin != iter_end; ++iter_begin)
			{
				if( typeid(T) == typeid( *(*iter_begin) ) )
				{
					if(iIndex == 0)
						return dynamic_cast<T*>((*iter_begin));
					//return (*iter_begin);
					else
						--iIndex;
				}
			}
		}
		return NULL;
	}
};
