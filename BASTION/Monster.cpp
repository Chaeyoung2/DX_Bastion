#include "StdAfx.h"
#include "Monster.h"

CMonster::CMonster(void)
:m_bIsStalking(false), m_bIsCombat(false), m_bIsRush(false),
m_iColor(255), m_iAlpha(255), m_bIsDamaged(false), m_bIsInvincible(false),
 m_bIsMakingBreak(false), m_bIsMakingShot(false), m_eMonColID(MONCOL_END),
 m_fInivinTime(0.f), m_fDamageTime(0.f)
{
}

CMonster::~CMonster(void)
{
}
