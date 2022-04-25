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

	////////////////// ������Ʈ ����
	void					MatrixCheck();

	// �浹�ڽ�
	void					SettingCollRect(float fHeight, float fWidth);

	// �÷��̾�� �Ÿ� ���
	void					CalcDistanceToPlayer();

	// �������� ����Ʈ
	void					MakingEffect();

	// ���� �� ���� ����� Ÿ������ �ɼ� �� �ѱ��
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
