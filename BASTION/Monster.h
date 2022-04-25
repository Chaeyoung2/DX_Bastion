#pragma once
#include "gameobject.h"

class CMonster :
	public CGameObject
{
public:
	CMonster(void);
	virtual ~CMonster(void);

protected:
	// 플레이어 인식 렉트
	RECT			m_tRangeRect;

	// 플레이어 정보
	RECT*			m_pTargetRect;
	INFO*			m_pTargetInfo;
	D3DXVECTOR3		m_vTargetPos;

	// 전투모드
	bool			m_bIsCombat;

	bool			m_bIsRush;

	// 스토킹 모드
	bool			m_bIsStalking;

	// 데이터
	DATA			m_tData;

	// 프레임
	FRAME			m_tFrame;
	wstring			m_wstrStateKey;

	// 플레이어에 피격시 색깔 입히기
	int				m_iColor;
	int				m_iAlpha;
	bool			m_bIsDamaged;

	float			m_fDamageTime;

	// 중복 충돌 방지
	bool			m_bIsInvincible;

	float			m_fInivinTime;

	// 중복 이펙트 생성 방지
	bool			m_bIsMakingBreak;
	bool			m_bIsMakingShot;


	// 어떤 것과 충돌되고 있는지
	eMonCollisionID	m_eMonColID;

public:
	bool			GetIsInvincible() { return m_bIsInvincible; }
	void			SetIsInvincible(bool bIsInvincible) { m_bIsInvincible = bIsInvincible; }
	void			SetMonColID(eMonCollisionID eID) { m_eMonColID = eID; }

public:
	const DATA& GetData()
	{
		return m_tData;
	}
	void SetDamage(int iAtt)
	{
		m_tData.iHp -= iAtt;
	}
	void SetIsDamaged(bool bIsDamaged)
	{
		m_bIsDamaged = bIsDamaged;
	}
	void SetDirection(eObjectDir eDir)
	{
		m_eDir = eDir;

		switch(m_eDir)
		{
		case DIR_B:
			m_wstrStateKey = L"DIR_B";
			break;
		case DIR_LB:
			m_wstrStateKey = L"DIR_LB";
			break;
		case DIR_RB:
			m_wstrStateKey = L"DIR_LB";
			break;
		case DIR_LT:
			m_wstrStateKey = L"DIR_LT";
			break;
		case DIR_RT:
			m_wstrStateKey = L"DIR_LT";
			break;
		case DIR_T:
			m_wstrStateKey = L"DIR_T";
			break;
		case DIR_R:
			m_wstrStateKey = L"DIR_L";
			break;
		case DIR_L:
			m_wstrStateKey = L"DIR_L";
			break;
		}
	}
};
