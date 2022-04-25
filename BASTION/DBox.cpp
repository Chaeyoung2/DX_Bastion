#include "StdAfx.h"
#include "DBox.h"
#include "EffectBreak.h"
#include "GasPella.h"
#include "Factory.h"

CDBox::CDBox(void)
:m_bIsTransform(true), m_fTransSpeed(900.f), m_fMoveSpeed(0.f), m_bIsTransformUp(false),
m_bIsMakeCompleteSub19(false)
{					  
}

CDBox::~CDBox(void)
{
}

HRESULT CDBox::Initialize(void)
{
	m_eRenderID = RENDER_OBJ;

	m_wstrObjKey = L"BackGround";

	SettingCollRect(m_tFrame.fFrame); // 이미지 장수에 따라 다른 충돌박스



	return S_OK;

}

eOBJECT_RESULT CDBox::Update(void)
{
	if(m_bIsDead == true)
	{
		m_pObjMgr->AddObject( 
			CreateEffectBreak<CEffectBreak>(DIR_RB, m_tInfo.vPos), 
			OBJ_EFFECT);

		if(m_tFrame.fFrame == 62.f)
		{
			if(m_bIsMakeCompleteSub19 == false)
			{
				

				m_pObjMgr->AddObject( 
					CFactory<CGasPella>::CreateSpinGapella(m_tInfo.vPos.x - 2.f, m_tInfo.vPos.y, DIR_B),
					OBJ_MONSTER);
				m_pObjMgr->AddObject( 
					CFactory<CGasPella>::CreateSpinGapella(m_tInfo.vPos.x + 2.f, m_tInfo.vPos.y, DIR_B),
					OBJ_MONSTER);
				m_pObjMgr->AddObject( 
					CFactory<CGasPella>::CreateSpinGapella(m_tInfo.vPos.x - 4.f, m_tInfo.vPos.y + 5.f, DIR_B),
					OBJ_MONSTER);
				m_pObjMgr->AddObject( 
					CFactory<CGasPella>::CreateSpinGapella(m_tInfo.vPos.x + 4.f, m_tInfo.vPos.y + 5.f, DIR_B),
					OBJ_MONSTER);

// 				m_pObjMgr->AddObject( 
// 					CFactory<CSubTitle>::CreateSubTitle(19.f), OBJ_SCREEN);
				// 별로임

				m_bIsMakeCompleteSub19 = true;
			}

		}
		else
		{
			m_pObjMgr->AddObject( 
				CFactory<CGasPella>::CreateSpinGapella(m_tInfo.vPos.x - 5.f, m_tInfo.vPos.y, DIR_B),
				OBJ_MONSTER);
			m_pObjMgr->AddObject( 
				CFactory<CGasPella>::CreateSpinGapella(m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y, DIR_B),
				OBJ_MONSTER);

		}

			

		return OR_DELETE;
	}

	return OR_OK;
}

void CDBox::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		L"Structure", int(m_tFrame.fFrame) );

	if(pTex == NULL)		return;


	float fX = pTex->ImgInfo.Width * 0.5f;
	float fY = pTex->ImgInfo.Height * 0.5f;

	MatrixCheck();

// 	if(m_tFrame.fFrame == 59)
// 		int iA = 0;

	m_pSprite->SetTransform(&m_tInfo.matWorld);

	m_pSprite->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);

}  

void CDBox::Release(void)
{

}

void CDBox::FrameCheck(void)
{

}

void CDBox::MatrixCheck(void)
{
	D3DXMATRIX matTrans;

	// 하강
	float fY = WINCY + 100.f;

	if(m_bIsTransform == true)
	{
		m_fMoveSpeed += m_fTransSpeed * m_pTimeMgr->GetTime();  // 
	}

	if(m_tInfo.vPos.y - (-m_vScroll.y) <=
		m_tInfo.matWorld._42 )
	{
		m_bIsTransform = false;
		m_bIsTransformUp = true;
	}

	

	// 상승

	if(m_bIsTransformUp == true)
	{
		m_fMoveSpeed -= m_fTransSpeed * m_pTimeMgr->GetTime() * 0.01f;

		if((m_tInfo.vPos.y - 0.5f) - (-m_vScroll.y) >=
			m_tInfo.matWorld._42 ) // 100.f는 위로 띄울 거리
		{
			m_bIsTransformUp = false;
			m_bIsDead = true; // 다 띄웠으면 죽을 준비 하셈
		}
	}
	


	//
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y - fY + m_fMoveSpeed,
		m_tInfo.vPos.z);

	m_tInfo.matWorld = matTrans;

}

void CDBox::SettingCollRect(float fFrame)
{
	int iFrame = (int)fFrame;
	switch(iFrame)
	{
	case 58:
		m_tInfo.vSize = D3DXVECTOR3(137.f, 133.f, 0.f);
		break;
	case 59:
		m_tInfo.vSize = D3DXVECTOR3(125.f, 121.f, 0.f);
		break;
	case 60:
		m_tInfo.vSize = D3DXVECTOR3(116.f, 97.f, 0.f);
		break;
	case 61:
		m_tInfo.vSize = D3DXVECTOR3(95.f, 91.f, 0.f);
		break;
	}

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f));
	m_tCollRect.bottom = static_cast<LONG>(m_tInfo.vPos.y - (m_tInfo.vSize.y * 0.5f));
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + (m_tInfo.vSize.x * 0.5f));
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - (m_tInfo.vSize.x * 0.5f));

}
