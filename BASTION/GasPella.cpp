#include "StdAfx.h"
#include "GasPella.h"
#include "Factory.h"
#include "Player.h"
#include "EffectAttack2.h"
#include "EffectBreak.h"
#include "EffectShot.h"
#include "SubTitle.h"
#include "DataSubject.h"

CGasPella::CGasPella(void)
{
}

CGasPella::~CGasPella(void)
{
}

HRESULT CGasPella::Initialize(void)
{
	m_tFrame = FRAME(0.f, 0.f, 1.f, 1.f);

	m_tData.iAtt = 50;
	m_tData.iHp = 400;
	m_tData.iXp = 100;

	m_tInfo.vDir  = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(70.f, 144.f, 0.f);

	m_wstrObjKey = L"Monster1";
// 	m_wstrStateKey = L"Dir_B"; // 나중에 유닛 배치할 땐 따로 해줄것
// 
// 	m_eDir = DIR_LB;

	m_eRenderID = RENDER_OBJ;

	m_fSpeed  = 90.f;
	m_fScale = 0.f;
	m_fScaleX = 1.f;
	m_fScaleSpeed = 0.1f;

	m_bIsMakingEffect = false;

	m_pTargetInfo = (m_pObjMgr->FindGameObject<CPlayer>()->GetInfo());
	//m_vTargetPos = (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo())->vPos;


	m_fFocusTime = 0.f;

	m_iSpinCount = 0;

	if(m_bIsSpin != true) // 따로 설정해 준게 아니면
	{
		m_bIsSpin = false;
	m_wstrStateKey = L"Dir_L";

	m_eDir = DIR_L;

	}
	if(m_bIsSpin)
	{
		m_bIsStalking = true;
	}

	return S_OK; 

}

eOBJECT_RESULT CGasPella::Update(void)
{
// 	system("cls");
// 	cout << "GasPella" << endl;
// 	cout << "m_tCollRect.top : " << m_tCollRect.top << endl;
// 	cout <<	"m_tCollRect.bottom : " << m_tCollRect.bottom << endl;
// 	cout <<	"m_tCollRect.left : " << m_tCollRect.left  << endl;
// 	cout << "m_tCollRect.right : " << m_tCollRect.right << endl << endl;

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

	// 잠시 얘한테 스크롤 맞춰주는 순간이 있음
	Scroll();

	// 스핀
	if(m_bIsSpin)
		Spin();


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
		CGameObject* pPlayer = m_pObjMgr->FindGameObject<CPlayer>();
		dynamic_cast<CPlayer*>(pPlayer)->SetDamage(m_tData.iAtt);
		//CDataSubject::GetInstance()->Notify(PLAYER_DATA, &m_tData);
		return OR_DELETE;
	}

	// 체력 닳아도 사망
	if(m_tData.iHp <= 0)
	{
		if(m_eMonColID == MONCOL_HAMMER)
		{
			m_pObjMgr->AddObject( 
				CreateEffectBreak<CEffectBreak>(m_eDir, m_tInfo.vPos), 
				OBJ_EFFECT);
		}
		if(m_eMonColID == MONCOL_BULLET)
		{
			m_pObjMgr->AddObject( 
				CreateEffectBreak<CEffectShot>(m_eDir,  m_tInfo.vPos), 
				OBJ_EFFECT);
		}
		CGameObject* pPlayer = m_pObjMgr->FindGameObject<CPlayer>();

		if( dynamic_cast<CPlayer*>(pPlayer)->m_bIsMakeCompleteSub12 == false)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(12.f), OBJ_SCREEN);
			m_pSoundMgr->PlaySound(L"Narrator13.MP3", CHANNEL_SUBTITLE13);
			dynamic_cast<CPlayer*>(pPlayer)->m_bIsMakeCompleteSub12 = true;
			dynamic_cast<CPlayer*>(pPlayer)->m_bIsCountingSub12ToSub13Time = true;
		}

		m_bIsDead = true;
	}

	// 플레이어에 피격당할시 검정색으로 렌더
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
		if(m_eMonColID == MONCOL_BULLET) // 해머와 충돌중이고
		{
			if(m_bIsMakingShot == false) // 아직 break 이펙트를 만들지 않았으면
			{
				m_pObjMgr->AddObject( 
					CreateEffectBreak<CEffectShot>(m_eDir, m_tInfo.vPos), 
					OBJ_EFFECT);
				m_bIsMakingShot = true;
			}
		}

		if(m_fDamageTime >= 1.f)
		{
			m_iColor = 255;
/*			m_iAlpha = 255;*/
			m_bIsDamaged = false;
			m_fDamageTime = 0.f;
		}
	}



	return OR_OK;
}

void CGasPella::Render(void)
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

void CGasPella::Release(void)
{

}

void CGasPella::FrameCheck(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime();

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fFrame = m_tFrame.fOriFrame;
	}
}

void CGasPella::MovingInPattern()
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


void CGasPella::Scroll()
{
	if(CGameObject::m_bIsFocusOnGaspella == false)
		return;

	if(CGameObject::m_bIsFocusOnGaspella == true)
	{
		m_fFocusTime += m_pTimeMgr->GetTime();

		if(m_fFocusTime >= 3.f)
		{
			CGameObject::m_bIsFocusOnGaspella = false;
		}
	}

	float fNumberX = WINCX * 0.5f - 50;
	float fNumberY = WINCY * 0.5f - 50;

	if( ( m_tInfo.vPos.x + m_vScroll.x ) > WINCX - fNumberX)
	{
		m_vScroll.x -= 300.f * m_pTimeMgr->GetTime() ;
	}
	else if(m_tInfo.vPos.x + m_vScroll.x <= fNumberX)
	{
		m_vScroll.x += 300.f * m_pTimeMgr->GetTime() ;
	}

	if(m_tInfo.vPos.y + m_vScroll.y > WINCY - fNumberY)
	{
		m_vScroll.y -= 300.f * m_pTimeMgr->GetTime() ;
	}
	else if(m_tInfo.vPos.y + m_vScroll.y <= fNumberY)
	{
		m_vScroll.y += 300.f * m_pTimeMgr->GetTime() ;
	}


}

void CGasPella::MatrixCheck(void)
{
	D3DXMATRIX matTrans, matScale;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z
		);

	if(m_bIsSpin)
	{
		m_fScale += m_fScaleSpeed;

		if(m_fScale > 1.f)
		{
			m_bIsSpin = false;
		}

		D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 1.f);
	}
	else
	{
		D3DXMatrixScaling(&matScale, m_fScaleX, 1.f, 1.f);
	}

	m_tInfo.matWorld = matScale * matTrans;

// 	system("cls");
// 	cout << m_tInfo.matWorld._11 << endl;
// 	cout << m_tInfo.matWorld._22 << endl;
// 	cout << m_bIsInWindow << endl;

}

void CGasPella::SettingRangeRect()
{
	float fRangeRectY = 250.f;
	float fRangeRectX = 150.f;

	m_tRangeRect.top = static_cast<LONG>(m_tInfo.vPos.y - fRangeRectY);
	m_tRangeRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fRangeRectY);
	m_tRangeRect.left = static_cast<LONG>(m_tInfo.vPos.x - fRangeRectX);
	m_tRangeRect.right = static_cast<LONG>(m_tInfo.vPos.x + fRangeRectX);
}

void CGasPella::JudgeStalkinginRange()
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

void CGasPella::JudgeBeingCombatinRange()
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
		&& m_bIsRush == false
		&& IntersectRect(&tTempRect, &m_tCollRect, &tTargetRect) )
	{
		m_bIsCombat = true;
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

void CGasPella::SettingCollRect()
{
	float fCollRectY = 50.f;
	float fCollRectX = 25.f;

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - fCollRectY);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fCollRectY);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - fCollRectX);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + fCollRectX);
}

void CGasPella::DirSettingInTargetMoving()
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
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = 1.f;

	}
	else if(fAngle >= -75 && fAngle < -15)
	{
		m_eDir = DIR_LB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = 1.f;

	}
	else if(fAngle >= -105 && fAngle < -75)
	{
		m_eDir = DIR_B;
		m_wstrStateKey = L"Dir_B";

	}
	else if(fAngle >= -165 && fAngle < -105)
	{
		m_eDir = DIR_RB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = -1.f;



	}
	else if(fAngle < -165 || fAngle >= 165)
	{
		m_eDir = DIR_R;
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = -1.f;

	}
	else if(fAngle >= 105 && fAngle < 165)
	{
		m_eDir = DIR_RT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = -1.f;

	}
	else if(fAngle >= 75 && fAngle < 105)
	{
		m_eDir = DIR_T;
		m_wstrStateKey = L"Dir_T";

	}
	else if(fAngle >= 15 && fAngle < 75)
	{

		m_eDir = DIR_LT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = 1.f;

	}

}

void CGasPella::Stalking()
{
	// 타겟 포스 가져오기
	m_vTargetPos = m_pTargetInfo->vPos;


	D3DXVECTOR3 vDir = m_tInfo.vPos - m_vTargetPos;

	D3DXVec3Normalize(&vDir, &vDir);

	m_tInfo.vPos -= vDir * m_pTimeMgr->GetTime() * m_fSpeed;

}

void CGasPella::Combating()
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
				CreateEffect<CEffectAttack2>(m_eDir), 
				OBJ_EFFECT);
			m_pSoundMgr->PlaySound(L"Monster1Att.MP3", CHANNEL_GASPELLAHIT);
			CGameObject* pPlayer = m_pObjMgr->FindGameObject<CPlayer>();
			dynamic_cast<CPlayer*>(pPlayer)->SetDamage(m_tData.iAtt);
			//m_pSoundMgr->PlaySound(L"MonsterHit.wav", CHANNEL_GASPELLAHIT);
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

void CGasPella::Spin()
{
	//cout << m_iSpinCount << endl;

	m_iSpinCount++;

	int i = 0;
	int iGob = 10;

	if(m_iSpinCount == i ) 
	{
		m_eDir = DIR_B;
		m_wstrStateKey = L"Dir_B";
	}
	else if(m_iSpinCount == i + 1)
	{
		m_eDir = DIR_LB;
		m_wstrStateKey = L"Dir_LB";
	}
	else if(m_iSpinCount == i + 2)
	{
		m_eDir = DIR_LT;
		m_wstrStateKey = L"Dir_LT";
	}
	else if(m_iSpinCount == i + 3)
	{
		m_eDir = DIR_T;
		m_wstrStateKey = L"Dir_T";
	}
	else if(m_iSpinCount == i + 4)
	{
		m_eDir = DIR_RT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = -1.f;
	}
	else if(m_iSpinCount == i + 5)
	{
		m_eDir = DIR_R;
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = -1.f;
	}
	else if(m_iSpinCount == i + 6)
	{
		m_eDir = DIR_RB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = -1.f;
	}
	else if(m_iSpinCount == i + 7)
	{
		m_eDir = DIR_B;
		m_wstrStateKey = L"Dir_B";
	}
	else if(m_iSpinCount == i + 8)
	{
		m_eDir = DIR_LB;
		m_wstrStateKey = L"Dir_LB";
	}


// 	if(i <= m_iSpinCount && iGob > m_iSpinCount) // 1 ~ 9
// 	{
// 		m_eDir = DIR_B;
// 		m_wstrStateKey = L"DIR_B";
// 	}
// 	else if(i * iGob <= m_iSpinCount && (i + 1) * iGob > m_iSpinCount) // 10 ~ 19
// 	{
// 		m_eDir = DIR_LB;
// 		m_wstrStateKey = L"DIR_LB";
// 	}
// 	if((i + 1) * iGob <= m_iSpinCount && (i + 2) * iGob > m_iSpinCount)
// 	{
// 		m_eDir = DIR_LT;
// 		m_wstrStateKey = L"DIR_LT";
// 	}
// 	if((i + 2) * iGob <= m_iSpinCount && (i + 3) * iGob > m_iSpinCount)
// 	{
// 		m_eDir = DIR_T;
// 		m_wstrStateKey = L"DIR_T";
// 	}
// 	if((i + 3) * iGob <= m_iSpinCount && (i + 4) * iGob > m_iSpinCount)
// 	{
// 		m_eDir = DIR_RT;
// 		m_wstrStateKey = L"DIR_LT";
// 		m_fScaleX = -1.f;
// 	}
// 	if((i + 4) * iGob <= m_iSpinCount && (i + 5) * iGob > m_iSpinCount)
// 	{
// 		m_eDir = DIR_R;
// 		m_wstrStateKey = L"DIR_L";
// 		m_fScaleX = -1.f;
// 	}
// 	if((i + 5) * iGob <= m_iSpinCount && (i + 6) * iGob > m_iSpinCount)
// 	{
// 		m_eDir = DIR_RB;
// 		m_wstrStateKey = L"DIR_LB";
// 		m_fScaleX = -1.f;
// 	}
}

