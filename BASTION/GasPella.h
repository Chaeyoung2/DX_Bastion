#pragma once
#include "Monster.h"

class CGasPella :
	public CMonster
{
public:
	CGasPella(void);
	virtual ~CGasPella(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

public:
	void FrameCheck(void);
	void MatrixCheck(void);

	void DirSettingInTargetMoving();

	void MovingInPattern();

	void Scroll();


public:
	void JudgeStalkinginRange();
	void JudgeBeingCombatinRange();

	void SettingRangeRect();
	void SettingCollRect();

	void Stalking();
	void Combating();

	void MakingEffectBreak();

	void Spin();


public:

	void SetIsMakingBreak(bool bIsMakingBreak)
	{
		m_bIsMakingBreak = m_bIsMakingBreak;
	}
	void SetIsSpin(bool bIsSpin)
	{
		m_bIsSpin = bIsSpin;
	}

// 	template <typename T>
// 	CGameObject* CreateEffect(/*D3DXVECTOR3 vDir, */eObjectDir eDir)
// 	{
// 		CGameObject* pObj = new T;
// 		pObj->SetPos(m_tInfo.vPos.x, m_tInfo.vPos.y);
// 		dynamic_cast<CEffect*>(pObj)->SetObjecteDir(eDir);
// 		pObj->Initialize();
// 		/*		dynamic_cast<CEffect*>(pObj)->SetObjectDir(vDir);*/
// 
// 		return pObj;
// 	}


private:
	ePlayerState	m_ePreState;
	ePlayerState	m_eCurState;

	float			m_fScale;
	float			m_fScaleX;
	float			m_fScaleSpeed;

	bool			m_bIsMakingEffect;

	float			m_fFocusTime;
	
	bool			m_bIsSpin;

	int				m_iSpinCount;


};
