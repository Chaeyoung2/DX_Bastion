#pragma once

#include "Include.h"

class CTOOLView;
class CMyObj
{
private:
	list<PMYOBJ>	m_StructureList;
	list<PMYOBJ>	m_GroundList;
	CTOOLView*		m_pMainView;

public:
	void SetMainView(CTOOLView* pMainView) { m_pMainView = pMainView; }

	list<PMYOBJ>*	GetStructureList() { return &m_StructureList; }
	list<PMYOBJ>*	GetGroundList() { return &m_GroundList; }

	HRESULT			Initialize();
	int				Update();
	void			Render();
	void			Release();

	void			SetObject(D3DXVECTOR3 vPos, int iObjID, 
		int iRenderOption, int iCount, 
		int* pStIndex, int* pGrIndex
		, int iCollCount, bool bIsAgreeToDamage);

	void			MiniRender();


	// 
	void			TempRenderStruct();
	void			TempRenderGround();
	int				m_iTempCount;
	D3DXVECTOR3		m_vMousePos;
	list<PMYOBJ>	m_TempStructureList;
	list<PMYOBJ>	m_TempGroundList;


public:
	CMyObj(void);
	virtual ~CMyObj(void);
};
