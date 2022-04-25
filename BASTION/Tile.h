#pragma once
#include "gameobject.h"
#include "KeyMgr.h"

class CTile :
	public CGameObject
{
public:
	CTile(void);
	virtual ~CTile(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);
	
	////////////////// 오브젝트 공통
	void					MatrixCheck(int iIndex);

	////////////////// FUNC
private:
	void					LoadMapData();
	void					MouseScrollCheck();
	void					CalcDistanceToPlayer();


	////////////////// GET SET
public:
	/*const */vector<PTILE>*	GetTile() { return &m_vecTile; }


	////////////////// STL
private:
	vector<PTILE>			m_vecTile;
	vector<float>				m_vecScaleX;
	vector<float>				m_vecScaleY;
	vector<bool>				m_vecShaking;
	vector<float>				m_vecTransX;
	vector<float>				m_vecTransY;


	////////////////// VARIABLE
	POINT					m_ptTileRender;

	float					m_fTransSpeed;
	float					m_fScaleSpeed;
	float					m_fTrans;
	float					m_fScaleX;
	float					m_fScaleY;

	bool					m_bIsTranslation;

	// class
	CKeyMgr*				m_pKeyMgr;

	bool					m_bIsSettingScrollInChangingScene;

};
