#pragma once
#include "gameobject.h"

class CStructure :
	public CGameObject
{
public:
	CStructure(void);
	virtual ~CStructure(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	////////////////// 오브젝트 공통
	void					MatrixCheck( list<PMYOBJ>::iterator iter);
	void					ForwardRender();
	void					BackwardRender();

	////////////////// FUNC
public:
	void					LoadData();
	void					MakingEffect();
	void					SettingCollRect( list<PMYOBJ>::iterator iter, float fHeight, float fWidth );
	void					CalcDistanceToPlayer();

	// ** 0913
	void					IsHigherthanPlayer();

	////////////////// Get Set
public:
	list<PMYOBJ>*	GetStructureList()
	{
		return &m_StructureList;
	}
	list<PMYOBJ>*	GetForwardStructureList()
	{
		return &m_ForwardStructureList;
	}
	list<PMYOBJ>*	GetBackwardStructureList()
	{
		return &m_BackwardStructureList;
	}
private:
	list<PMYOBJ>	m_StructureList;
	list<PMYOBJ>	m_ForwardStructureList;
	list<PMYOBJ>	m_BackwardStructureList;
	wstring			m_wstrStateKey;

	float			m_fTrans;
	float			m_fTransSpeed;

	CGameObject*	m_pPlayer;

	bool			m_bIsSettedPlayer;
	bool			m_bIsCompareWithPlayer;


};
