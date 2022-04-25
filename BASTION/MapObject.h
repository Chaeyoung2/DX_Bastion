#pragma once
#include "gameobject.h"

class CMapObject :
	public CGameObject
{
public:
	CMapObject(void);
	virtual ~CMapObject(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	////////////////// 오브젝트 공통
	void					MatrixCheck();

	// 충돌박스
	void					SettingCollRect(float fHeight, float fWidth);

	// 플레이어와 거리 계산
	void					CalcDistanceToPlayer();

	// 줘터지는 이펙트
	void					MakingEffect();

	// 죽을 때 가장 가까운 타일한테 옵션 값 넘기기
	void					CalcDistanceToTileForOption();



public:
	wstring			m_wstrStateKey;
	float			m_fTrans;
	float			m_fTransSpeed;

	MYOBJ			m_tObjInfo;

	CGameObject*	m_pPlayer;
	vector<PTILE>*	m_pVecTile;

	bool			m_bIsSettingTile;
	bool			m_bIsSettingPlayer;
	bool			m_bIsSettingPosY;

	float			m_fDamagedTime;

	float			m_fHeight;



	///////////////////////////
	void SetObjInfo(PMYOBJ pObjInfo)
	{
		m_tObjInfo = (*pObjInfo);
	}
	void SetIsDamaged(bool bIsDamaged)
	{
		m_tObjInfo.bIsDamaged = bIsDamaged;
	}
	void SetIsInvincible(bool bIsInvincible)
	{
		m_tObjInfo.bIsInvincible = bIsInvincible;
	}
	void SetCollCount(int iCollCount)
	{
		m_tObjInfo.iCollCount += iCollCount;
	}

	PMYOBJ GetObjInfo()
	{
		return &m_tObjInfo;
	}

	/////////////////////////

};
