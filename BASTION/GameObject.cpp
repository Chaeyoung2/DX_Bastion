#include "StdAfx.h"
#include "GameObject.h"
#include "Device.h"
#include "SubTitle.h"
#include "Factory.h"



D3DXVECTOR3 CGameObject::m_vScroll = D3DXVECTOR3(0.f, /*0.f*/-(3135 - WINCY), 0.f);

CGameObject::CGameObject(void)
:m_wstrObjKey(L"")
,m_fSpeed(0.f), m_bIsDead(false), 
m_pSprite(CDevice::GetInstance()->GetSprite()),
m_eRenderID(RENDER_END),
m_pTimeMgr(CTimeMgr::GetInstance()),
m_pTextureMgr(CTextureMgr::GetInstance()),
m_pObjMgr(CObjMgr::GetInstance()),
m_pSoundMgr(CSoundMgr::GetInstance())

{
	// 화면 흔들림 효과
	//m_bIsShaking = false;
	m_fShakingSpeed = 10.f;
	m_fShakingTime = 0.f;

	m_iShakingCount = 0;
	m_iShakingMaxCount = 200;
 
	m_bIsShakingToRight = false;
	m_bIsShakingToLeft = true;
	m_bIsShakingToBottom = false;
	m_bIsShakingToTop = true;

	m_bIsSetCompletedOriginScrollValue = false;

	// 포커스 온
	m_bIsFocusOnGaspella = false;
	m_bIsFocusToEnd = true; // 변경할 좌표로 가줄 것인지
	m_bIsFocusToOrigin = false; // 이전 좌표로 가줄 것인지

	m_fFocusToEndAfterTime = 0.f;

	// 자막
	m_bIsMakeCompleteSub11 = false;

	m_bIsCountingEndToOrigin = false;


	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
}

CGameObject::~CGameObject(void)
{
}

bool CGameObject::IsInWindow()
{
	float fFlexibility = 100.f; // 화면에 대놓고 잘리는 오브젝트들이 보이넹.. 



	if( m_tInfo.vPos.x >= (-m_vScroll.x) - fFlexibility 
		&& m_tInfo.vPos.x <= (WINCX + (-m_vScroll.x)) + fFlexibility
		&& m_tInfo.vPos.y >= (-m_vScroll.y) - fFlexibility
		&& m_tInfo.vPos.y <= (WINCY + (-m_vScroll.y)) + fFlexibility )
	{
		m_bIsInWindow = true;
		return true;
	}
	else
	{
		m_bIsInWindow = false;
		return false;
	}
}

void CGameObject::FocusOnGaspella()
{
	if(m_bIsFocusOnGaspella == false)
		return;

	float fOriginScrollX = -3643.29f;
	float fOriginScrollY = -161.188f;
	float fEndScrollX = -4120.f;
	float fEndScrollY = -294.13f;

	float fScrollSpeed = 150.f;

	if(m_bIsFocusToEnd == true)
	{
		m_vScroll.x -= fScrollSpeed * m_pTimeMgr->GetTime();
		m_vScroll.y -= fScrollSpeed * m_pTimeMgr->GetTime() * 0.2f;

		if(m_vScroll.x <= fEndScrollX) // 갈만큼 가줬으면 꺼준다
		{
			m_bIsFocusToEnd = false;

			// 가 줬으면 자막11 생성.
			if(m_bIsMakeCompleteSub11 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(11.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator12.MP3", CHANNEL_SUBTITLE12);
				m_bIsMakeCompleteSub11 = true;
				m_bIsCountingEndToOrigin = true;
			}

		}
	}

	if(m_bIsCountingEndToOrigin == true)
	{
		m_fFocusToEndAfterTime += m_pTimeMgr->GetTime();
	}
	if(m_fFocusToEndAfterTime >= 5.f) // 다 도달했다고 바로 꺼주지 말고, 시간 셈
	{
		m_bIsFocusToOrigin = true;
		m_fFocusToEndAfterTime = 0.f;
		m_bIsCountingEndToOrigin = false;
	}

	
/*	cout << m_fFocusToEndAfterTime << endl;*/

	if(m_bIsFocusToOrigin == true)
	{
		m_vScroll.x += fScrollSpeed * m_pTimeMgr->GetTime();
		m_vScroll.y += fScrollSpeed * m_pTimeMgr->GetTime() * 0.2f;

		if(m_vScroll.x >= fOriginScrollX) // 갈만큼 가줬으면 꺼준다
		{
			m_bIsFocusToOrigin = false;
			m_bIsFocusOnGaspella = false;
		}


	}

	
	

}

void CGameObject::IsShaking(float fX, float fY)
{
// 	cout << "m_bIsShaking : " << m_bIsShaking << endl;
// 	cout << "m_iShakingCount : " << m_iShakingCount << endl;
	// true일 때만 켜지게
	if(g_bIsShaking == false)
	{
		m_iShakingCount = 0;
		return;
	}

	// 초기 스크롤 값 세팅
	if(m_bIsSetCompletedOriginScrollValue == false)
	{
		m_fOriginScrollX = m_vScroll.x;
		m_fOriginScrollY = m_vScroll.y;
		m_bIsSetCompletedOriginScrollValue = true;
	}


// 	system("cls");
// 	cout << "m_bIsShakingToLeft: "	 << m_bIsShakingToLeft		<< endl;
// 	cout << "m_bIsShakingToRight: "	 << m_bIsShakingToRight		<< endl;
// 	cout << "m_bIsShakingToTop: "	 << m_bIsShakingToTop		<< endl;
// 	cout << "m_bIsShakingToBottom: " << m_bIsShakingToBottom	<< endl;
// 	cout << "m_fOriginScrollX: "	 << m_fOriginScrollX		<< endl;
// 	cout << "m_fOriginScrollY: "	 << m_fOriginScrollY		<< endl;
// 	cout << "m_vScroll.x: "			 << m_vScroll.x				<< endl;
// 	cout << "m_vScroll.y: "			 << m_vScroll.y				<< endl;


	// 흔들림_우 일때 스크롤 값 변경.
	if(m_bIsShakingToRight == true)
	{
		m_vScroll.x -= m_fShakingSpeed;
	}
	// 흔들림_좌 일때 스크롤 값 변경.
	if(m_bIsShakingToLeft == true)
	{
		m_vScroll.x += m_fShakingSpeed;
	}
	// 흔들림_우 일때 스크롤 값 변경.
	if(m_bIsShakingToBottom == true)
	{
		m_vScroll.y -= m_fShakingSpeed;
	}
	// 흔들림_좌 일때 스크롤 값 변경.
	if(m_bIsShakingToTop == true)
	{
		m_vScroll.y += m_fShakingSpeed;
	}

	// 흔들림 우하_좌상 구분. // 현재 변경된 스크롤값이 (기존 스크롤값 - 범위)를 벗어나면.
	if( m_vScroll.x >= m_fOriginScrollX + fX) // 좌			`	-> 우
	{
		m_bIsShakingToLeft = false;
		m_bIsShakingToRight = true;

		m_iShakingCount++;
	}
	if( m_vScroll.x < m_fOriginScrollX - fX ) // 우 -> 좌
	{
		m_bIsShakingToLeft = true;
		m_bIsShakingToRight = false;

		m_iShakingCount++;
	}

	if( m_vScroll.y >= m_fOriginScrollY + fY) // 상 -> 하
	{
		m_bIsShakingToBottom = true;
		m_bIsShakingToTop = false;
		m_iShakingCount++;
	}
	if( m_vScroll.y < m_fOriginScrollY - fY) //하 -> 상
	{
		m_bIsShakingToBottom = false;
		m_bIsShakingToTop = true;
		m_iShakingCount++;
	}

	if(m_iShakingCount >= m_iShakingMaxCount)
	{
		m_iShakingCount = 0;
		m_vScroll.x = m_fOriginScrollX;
		m_vScroll.y = m_fOriginScrollY;
		g_bIsShaking = false;
		m_bIsSetCompletedOriginScrollValue = false;
	}
	
}