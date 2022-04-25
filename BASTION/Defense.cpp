#include "StdAfx.h"
#include "Defense.h"
#include "Factory.h"
#include "Player.h"
#include "MonBullet.h"
#include "EffectBreak.h"
#include "EffectShot.h"
#include "SubTitle.h"

CDefense::CDefense(void)
{
}

CDefense::~CDefense(void)
{
}

HRESULT CDefense::Initialize(void)
{
	// ������
	m_tFrame = FRAME(0.f, 0.f, 0.f, 0.f);

	// Ű��
	m_wstrObjKey = L"Monster2";
	m_wstrStateKey = L"Dir_LB"; // ���߿� ���� ��ġ�� �� ���� ���ٰ�

	// ���� ������
	m_tData.iAtt = 70;
	m_tData.iHp = 200;
	m_tData.iXp = 150;

	// INFO
	m_tInfo.vDir  = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(130.f, 180.f, 0.f);

	// ����
	m_eDir = DIR_LB;

	// ���� �ð����� �ҷ� �߻�
	m_fBulletTime = 0.f;
	m_bIsSetBulletTime = false;
	

	// ���ǵ�
	m_fSpeed  = 70.f;

	// ������
	m_fScaleX = 1.f;


	// Ÿ�� ����
	m_pTargetInfo = (m_pObjMgr->FindGameObject<CPlayer>()->GetInfo());


	m_eRenderID = RENDER_OBJ;

	// �ڸ�
	m_bISMakeCompleteSub16 = false;

	m_iShotCount = 0; // 3������ �߻����� �� 16 �ڸ� ����

	m_bIsMakingGunSound = false;

	return S_OK; 
}

eOBJECT_RESULT CDefense::Update(void)
{

	// ������Ʈ ����
	FrameCheck();
	MatrixCheck();

	// ���潺
	JudgeBeingCombatinRange();


	// ���� ����
	SettingCollRect();
	SettingRangeRect();
	DirSettingInTargetMoving();
	Combating();

	if(m_bIsInvincible == true)
	{
		m_fInivinTime += m_pTimeMgr->GetTime();

		if(m_fInivinTime >= 1.f)
		{
			m_bIsInvincible = false;
			m_fInivinTime = 0.f;
		}
	}


	// �÷��̾ �ǰݴ��ҽ� ���������� ����
	if(m_bIsDamaged == true)
	{
		m_fDamageTime += m_pTimeMgr->GetTime();

		m_iColor = 0;

		if(m_fDamageTime >= 1.f)
		{
			m_iColor = 255;
			m_bIsDamaged = false;
			m_fDamageTime = 0.f;
		}

		if(m_eMonColID == MONCOL_HAMMER) // �ظӿ� �浹���̰�
		{
			if(m_bIsMakingBreak == false) // ���� break ����Ʈ�� ������ �ʾ�����
			{
				m_pObjMgr->AddObject( 
					CreateEffectBreak<CEffectBreak>(m_eDir, m_tInfo.vPos), 
					OBJ_EFFECT);
				m_bIsMakingBreak = true;
			}
		}
		if(m_eMonColID == MONCOL_BULLET) // �ظӿ� �浹���̰�
		{
			if(m_bIsMakingShot == false) // ���� break ����Ʈ�� ������ �ʾ�����
			{
				m_pObjMgr->AddObject( 
					CreateEffectBreak<CEffectShot>(m_eDir, m_tInfo.vPos), 
					OBJ_EFFECT);
				m_bIsMakingShot = true;
			}
		}
	}

	// ��� ����
	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	// ü�� ��Ƶ� ���
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

		m_bIsDead = true;
	}

// 	if(m_bIsInvincible == true) // ���� ���°� �ƴٴ� �� �ᱹ �ǰ� �ѹ� ���ߴٴ°�! ���� �ѹ� ����Ʈ �����ϵ���
// 	{
// 		
// 	}

	

	return OR_OK;
}

void CDefense::Render(void)
{
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
		D3DCOLOR_ARGB(255, m_iColor, m_iColor, m_iColor)
		);
}

void CDefense::Release(void)
{

}

void CDefense::FrameCheck(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime();

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		m_tFrame.fFrame = m_tFrame.fOriFrame;
	}
}

void CDefense::MatrixCheck(void)
{
	D3DXMATRIX matTrans, matScaleX;
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z
		);


	D3DXMatrixScaling(&matScaleX, m_fScaleX, 1.f, 1.f);

	m_tInfo.matWorld = matScaleX * matTrans;
}

void CDefense::JudgeBeingCombatinRange()
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
		m_bIsCombat = true;
		m_bIsSetBulletTime = true;
	}


// 	system("cls");
// 	cout<< "m_fBulletTime		: "	 << m_fBulletTime			<< endl;
// 	cout<< "m_bIsCombat			: "	 << m_bIsCombat				<< endl;
// 	cout<< "m_tRangeRect.bottom	: "	 << m_tRangeRect.bottom		<< endl;
// 	cout<< "m_tRangeRect.left	: "	 << m_tRangeRect.left		<< endl;
// 	cout<< "m_pTargetRect->top	: "	 << m_pTargetRect->top		<< endl;
// 	cout<< "m_pTargetRect->right: "	 << m_pTargetRect->right	<< endl;

}

void CDefense::SettingRangeRect()
{
	float fRangeRectY = 400.f;
	float fRangeRectX = 500.f;

	m_tRangeRect.top = static_cast<LONG>(m_tInfo.vPos.y - fRangeRectY);
	m_tRangeRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fRangeRectY);
	m_tRangeRect.left = static_cast<LONG>(m_tInfo.vPos.x - fRangeRectX);
	m_tRangeRect.right = static_cast<LONG>(m_tInfo.vPos.x + fRangeRectX);
}

void CDefense::SettingCollRect()
{
	float fCollRectX = 29.f;
	float fCollRectY = 56.f;

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - fCollRectY);
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + fCollRectY);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - fCollRectX);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + fCollRectX);
}

void CDefense::DirSettingInTargetMoving()
{

	D3DXVECTOR3 vMonsterToPlayer = 
		m_tInfo.vPos - m_pTargetInfo->vPos;

	D3DXVec3Normalize(&vMonsterToPlayer, &vMonsterToPlayer);

	float fRadian = atan2f(vMonsterToPlayer.y, vMonsterToPlayer.x);
	float fAngle  = D3DXToDegree(fRadian);

	// ����

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

void CDefense::Combating()
{
	// Ÿ�� ��ġ ��������
	m_vTargetPos = m_pTargetInfo->vPos;

	D3DXVECTOR3 vDir = m_vTargetPos - m_tInfo.vPos;

	m_tInfo.vDir = vDir;


	if(m_bIsCombat == true)
	{
		m_fBulletTime += m_pTimeMgr->GetTime();

		if(m_bIsMakingGunSound == false)
		{
			m_pSoundMgr->PlaySound(L"SecurityGun.mp3", CHANNEL_DEFENSEWING);
			m_bIsMakingGunSound = true;
		}

		if(m_fBulletTime >= 6.f)
		{
			// �ҷ� ����
			m_pObjMgr->AddObject(
				CreateBullet<CMonBullet>(m_tInfo.vDir, m_tInfo.vPos),
				OBJ_MONSTER_BULLET);

			// �ҷ� ����Ʈ
			m_pObjMgr->AddObject( 
				CreateEffectBreak<CEffectShot>(m_eDir,  m_tInfo.vPos), 
				OBJ_EFFECT);

			m_pSoundMgr->PlaySound(L"Gun.wav", CHANNEL_DEFENSEGUN);

			m_iShotCount++;

			// �ð� �ʱ�ȭ
			m_fBulletTime = 0.f;

			m_bIsMakingGunSound = false;
		}

	}

	if(m_iShotCount == 3)
	{
		if(m_bISMakeCompleteSub16 == false && g_eScene == SCENE_DIA)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(16.f), OBJ_SCREEN);
			m_pSoundMgr->PlaySound(L"Narrator21.MP3", CHANNEL_SUBTITLE17);

			m_bISMakeCompleteSub16 = true;
		}
	}

	

}
