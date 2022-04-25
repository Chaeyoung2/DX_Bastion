#pragma once
#include "Monster.h"

class CDefense :
	public CMonster
{
public:
	CDefense(void);
	virtual ~CDefense(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

public:
	void JudgeBeingCombatinRange();


	void DirSettingInTargetMoving();

	void SettingRangeRect();
	void SettingCollRect();

	void Combating();

private:
	float			m_fScaleX;

	float			m_fBulletTime;
	bool			m_bIsSetBulletTime;

	bool			m_bISMakeCompleteSub16;
	int				m_iShotCount;

	bool			m_bIsMakingGunSound;


	// template

private:
	template <typename T>
	CGameObject* CreateBullet(D3DXVECTOR3 vDir, 
		D3DXVECTOR3 vPos)
	{
		float fDefenseCY = DEFENSECY * 0.5f;
		float fMagicCX = 10.f;

		CGameObject* pObj = CFactory<T>::CreateGameObject();

		pObj->SetPos(vPos.x, vPos.y - fDefenseCY);
		pObj->SetDir(vDir);

		return pObj;
	}


};
