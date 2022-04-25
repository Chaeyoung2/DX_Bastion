#include "StdAfx.h"
#include "Boss.h"
#include "Factory.h"
#include "Player.h"
#include "EffectBreak.h"
#include "EffectShot.h"
#include "EffectAttack1.h"

CBoss::CBoss(void)
{
}

CBoss::~CBoss(void)
{
}

HRESULT CBoss::Initialize(void)
{
	m_tFrame = FRAME(0.f, 0.f, 10.f, 10.f);

	m_tData.iAtt = 50;
	m_tData.iHp = 2000;
	m_tData.iXp = 1000;

	m_tInfo.vDir  = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(50.f, 400.f, 0.f);

	m_wstrObjKey = L"Boss";
	m_wstrStateKey = L"Dir_B";
	m_eDir = DIR_B;

	m_eRenderID = RENDER_OBJ;

	m_fSpeed  = 90.f;
	m_fScaleX = 1.f;


	m_bIsMakingEffect = false;

	m_pTargetInfo = (m_pObjMgr->FindGameObject<CPlayer>()->GetInfo());


	m_bIsStalking = false;


	return S_OK; 

}

eOBJECT_RESULT CBoss::Update(void)
{

	// 무빙
	MovingInPattern();

	// 몬스터 공통
	SettingCollRect();
	SettingRangeRect();
	JudgeStalkinginRange(); // RangeRect와 플레이어 collrect로 스토킹 모드 판별
	JudgeBeingCombatinRange(); // CollRect와 플레이어 CollRect로 어택 모드 판별

	// 오브젝트 공통
	FrameCheck();
	MatrixCheck();
	DirSettingInTargetMoving();

	// 무적
	if(m_bIsInvincible == true)
	{
		m_fDamageTime += m_pTimeMgr->GetTime();

		if(m_fDamageTime >= 1.f)
		{
			m_bIsInvincible = false;
			m_fDamageTime = 0.f;
		}
	}

	// 사망 판정
	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	// 체력 닳으면 사망
	if(m_tData.iHp <= 0)
	{
		// 해머에 부딪혀서 뒤지면
		if(m_eMonColID == MONCOL_HAMMER)
		{
			m_pObjMgr->AddObject( 
				CreateEffectBreak<CEffectBreak>(m_eDir, m_tInfo.vPos), 
				OBJ_EFFECT);
		}
		// 불렛에 부딪혀서 뒤지면
		if(m_eMonColID == MONCOL_BULLET)
		{
			m_pObjMgr->AddObject( 
				CreateEffectBreak<CEffectShot>(m_eDir,  m_tInfo.vPos), 
				OBJ_EFFECT);
		}

		// 사망 판정
		m_bIsDead = true;
	}


	// 플레이어에 피격당할시 데미지 on, 검정색으로 렌더
	if(m_bIsDamaged == true)
	{
		m_fDamageTime += m_pTimeMgr->GetTime();

		m_iColor = 0;

		if(m_eMonColID == MONCOL_HAMMER) // 해머와 충돌중이고
		{
			if(m_bIsMakingBreak == false) // 아직 break 이펙트를 만들지 않았으면
			{
				m_pObjMgr->AddObject( 
					CreateEffectBreak<CEffectBreak>(m_eDir, m_tInfo.vPos), 
					OBJ_EFFECT);
				m_bIsMakingBreak = true;
			}
		}
		if(m_eMonColID == MONCOL_BULLET) // 불렛과 충돌중이고
		{
			if(m_bIsMakingShot == false) // 아직 shot 이펙트를 만들지 않았으면
			{
				m_pObjMgr->AddObject( 
					CreateEffectBreak<CEffectShot>(m_eDir, m_tInfo.vPos), 
					OBJ_EFFECT);
				m_bIsMakingShot = true;
			}
		}

		// 데미지 타임 1.f보다 커지면 다시 원상태
		if(m_fDamageTime >= 1.f)
		{
			m_iColor = 255;
			m_bIsDamaged = false;
			m_fDamageTime = 0.f;
		}
	}


	//

	return OR_OK;

}

void CBoss::Render(void)
{
	//
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), static_cast<int>(m_tFrame.fFrame) );

	if(pTex == NULL)		return;

	float fX = pTex->ImgInfo.Width * 0.5f;
	float fY = pTex->ImgInfo.Height * 0.5f;

	m_pSprite->SetTransform(&m_tInfo.matWorld);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(m_iAlpha, m_iColor, m_iColor, m_iColor)
		);

}

void CBoss::Release(void)
{

}

void CBoss::FrameCheck(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime();

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fFrame = m_tFrame.fOriFrame;
	}

}

void CBoss::MatrixCheck(void)
{

	D3DXMATRIX matTrans, matScale;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z
		);


	D3DXMatrixScaling(&matScale, m_fScaleX, 1.f, 1.f);


	m_tInfo.matWorld = matScale * matTrans;

}

void CBoss::DirSettingInTargetMoving()
{
	// 스토킹 모드 아닐땐 방향 세팅 해줄 필요 없음
	if(m_bIsStalking == false) return;

	D3DXVECTOR3 vMonsterToPlayer = 
		m_tInfo.vPos - m_pTargetInfo->vPos;

	D3DXVec3Normalize(&vMonsterToPlayer, &vMonsterToPlayer);

	float fRadian = atan2f(vMonsterToPlayer.y, vMonsterToPlayer.x);
	float fAngle  = D3DXToDegree(fRadian);

	// 방향

	if(fAngle >= -15 && fAngle < 15)
	{
		m_eDir = DIR_L;
		m_wstrStateKey = L"Dir_R";
		m_fScaleX = -1.f;

	}
	else if(fAngle >= -75 && fAngle < -15)
	{
		m_eDir = DIR_LB;
		m_wstrStateKey = L"Dir_RB";
		m_fScaleX = -1.f;

	}
	else if(fAngle >= -105 && fAngle < -75)
	{
		m_eDir = DIR_B;
		m_wstrStateKey = L"Dir_B";

	}
	else if(fAngle >= -165 && fAngle < -105)
	{
		m_eDir = DIR_RB;
		m_wstrStateKey = L"Dir_RB";
		m_fScaleX = 1.f;

	}
	else if(fAngle < -165 || fAngle >= 165)
	{
		m_eDir = DIR_R;
		m_wstrStateKey = L"Dir_R";
		m_fScaleX = 1.f;

	}
	else if(fAngle >= 105 && fAngle < 165)
	{
		m_eDir = DIR_RT;
		m_wstrStateKey = L"Dir_RT";
		m_fScaleX = 1.f;

	}
	else if(fAngle >= 75 && fAngle < 105)
	{
		m_eDir = DIR_T;
		m_wstrStateKey = L"Dir_T";

	}
	else if(fAngle >= 15 && fAngle < 75)
	{

		m_eDir = DIR_LT;
		m_wstrStateKey = L"Dir_RT";
		m_fScaleX = -1.f;

	}

}

void CBoss::MovingInPattern()
{
	//////////////////////////////////////////////////////////////////////////
	// 플레이어 - 몬스터 방향 벡터에 따른 스토킹
	if(m_bIsStalking == true && m_bIsCombat == false)
		Stalking();

	//////////////////////////////////////////////////////////////////////////
	// 플레이어 향한 공격

	if(m_bIsCombat == true) // 컴뱃 모드 아니면 ㄴㄴ
		Combating();

}

void CBoss::JudgeStalkinginRange()
{
	RECT tTempRect = {};
	RECT tTargetRect = {};

	m_pTargetRect = m_pObjMgr->
		FindGameObject<CPlayer>()->GetCollRect();

	tTargetRect.right = m_pTargetRect->right;
	tTargetRect.left = m_pTargetRect->left;
	tTargetRect.bottom = m_pTargetRect->bottom;
	tTargetRect.top = m_pTargetRect->top;

	if( IntersectRect(&tTempRect, &m_tRangeRect, &tTargetRect) )
	{
		m_bIsStalking = true;
	}

}

void CBoss::JudgeBeingCombatinRange()
{
	RECT tTempRect = {};
	RECT tTargetRect = {};

	m_pTargetRect = m_pObjMgr->
		FindGameObject<CPlayer>()->GetCollRect();

	tTargetRect.right = m_pTargetRect->right;
	tTargetRect.left = m_pTargetRect->left;
	tTargetRect.bottom = m_pTargetRect->bottom;
	tTargetRect.top = m_pTargetRect->top;




	// 비전투모드, 스토킹모드, 후진모드일 때 렉트가 충돌했다면
	if( m_bIsCombat == false
		&& m_bIsStalking == true
		&& m_bIsRush == false )
	{
		if(IntersectRect(&tTempRect, &m_tCollRect, &tTargetRect))
			m_bIsCombat = true;
		else
			m_bIsCombat = false;
	}

	// 비전투모드, 비스토킹모드,
	if( m_bIsCombat == false
		&& m_bIsStalking == true
		&& m_bIsRush == true
		&& IntersectRect(&tTempRect, &m_tCollRect, &tTargetRect) )
	{
		m_bIsRush = false;
		m_bIsCombat = false;
		m_bIsStalking = true;
	}
}

void CBoss::SettingRangeRect()
{
	float fRangeRectY = 300.f;
	float fRangeRectX = 300.f;

	m_tRangeRect.top = static_cast<LONG>(m_tInfo.vPos.y - fRangeRectY);
	m_tRangeRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fRangeRectY);
	m_tRangeRect.left = static_cast<LONG>(m_tInfo.vPos.x - fRangeRectX);
	m_tRangeRect.right = static_cast<LONG>(m_tInfo.vPos.x + fRangeRectX);

}

void CBoss::SettingCollRect()
{

	float fCollRectY = 50.f;
	float fCollRectX = 25.f;

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - fCollRectY);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fCollRectY);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - fCollRectX);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + fCollRectX);
}

void CBoss::Stalking()
{
	// 타겟 포스 가져오기
	m_vTargetPos = m_pTargetInfo->vPos;


	D3DXVECTOR3 vDir = m_tInfo.vPos - m_vTargetPos;

	D3DXVec3Normalize(&vDir, &vDir);

	m_tInfo.vPos -= vDir * m_pTimeMgr->GetTime() * m_fSpeed;

}

void CBoss::Combating()
{
	// 타겟 포스 가져오기
	m_vTargetPos = m_pTargetInfo->vPos;

	D3DXVECTOR3 vDir = m_tInfo.vPos - m_vTargetPos;

	// 플레이어로의 방향 벡터 길이
	float fDirLength = D3DXVec3Length(&vDir);


	if(fDirLength > 190.f)
	{
		m_bIsRush = true;
		m_bIsMakingEffect = false; // 앵간치 왔으면 다시 이펙트 만들 준비
	}
	if(fDirLength < 5.f)
	{
		m_bIsRush = false;
		m_bIsStalking = true;

		if(m_bIsMakingEffect == false)
		{
			m_pObjMgr->AddObject( 
				CreateEffect<CEffectAttack1>(m_eDir), 
				OBJ_EFFECT);
			m_bIsMakingEffect = true;

			m_bIsMakingBreak = false; // 까불었으니까 다시 줘터지는 이펙트 생길 준비
		}
	}


	m_tInfo.vDir = vDir;

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	// 후진 모드일땐
	if(m_bIsRush == false)
	{
		m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetTime() * m_fSpeed * 1.5f;
	}
	// 돌진 모드일땐
	else
	{
		m_tInfo.vPos -= m_tInfo.vDir * m_pTimeMgr->GetTime() * m_fSpeed * 5.f;
	}

	// 	 #ifdef _DEBUG
	// 	system("cls");
	// 	 	cout << "fDirLength: " << fDirLength << endl;
	// 	 	cout << "m_bIsRush: " << m_bIsRush << endl;
	// 		cout << "m_bIsCombat: " << m_bIsCombat << endl;
	// 		cout << "m_bIsStalking: " << m_bIsStalking << endl;
	// 	 #endif
}

void CBoss::MakingEffectBreak()
{

}
