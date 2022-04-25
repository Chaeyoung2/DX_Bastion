#pragma once
#include "Monster.h"

class CBoss:
	public CMonster
{
public:
	CBoss(void);
	virtual ~CBoss(void);

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

	void JudgeStalkinginRange();
	void JudgeBeingCombatinRange();

	void SettingRangeRect();
	void SettingCollRect();

	void Stalking();
	void Combating();

	void MakingEffectBreak();

public:
	void SetIsMakingBreak(bool bIsMakingBreak)
	{
		m_bIsMakingBreak = m_bIsMakingBreak;
	}

private:
	float			m_fScaleX;

	bool			m_bIsMakingEffect;

};
