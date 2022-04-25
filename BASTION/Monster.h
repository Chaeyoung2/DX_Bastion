#pragma once
#include "gameobject.h"

class CMonster :
	public CGameObject
{
public:
	CMonster(void);
	virtual ~CMonster(void);

protected:
	// �÷��̾� �ν� ��Ʈ
	RECT			m_tRangeRect;

	// �÷��̾� ����
	RECT*			m_pTargetRect;
	INFO*			m_pTargetInfo;
	D3DXVECTOR3		m_vTargetPos;

	// �������
	bool			m_bIsCombat;

	bool			m_bIsRush;

	// ����ŷ ���
	bool			m_bIsStalking;

	// ������
	DATA			m_tData;

	// ������
	FRAME			m_tFrame;
	wstring			m_wstrStateKey;

	// �÷��̾ �ǰݽ� ���� ������
	int				m_iColor;
	int				m_iAlpha;
	bool			m_bIsDamaged;

	float			m_fDamageTime;

	// �ߺ� �浹 ����
	bool			m_bIsInvincible;

	float			m_fInivinTime;

	// �ߺ� ����Ʈ ���� ����
	bool			m_bIsMakingBreak;
	bool			m_bIsMakingShot;


	// � �Ͱ� �浹�ǰ� �ִ���
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
