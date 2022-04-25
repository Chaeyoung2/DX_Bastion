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
	// ȭ�� ��鸲 ȿ��
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

	// ��Ŀ�� ��
	m_bIsFocusOnGaspella = false;
	m_bIsFocusToEnd = true; // ������ ��ǥ�� ���� ������
	m_bIsFocusToOrigin = false; // ���� ��ǥ�� ���� ������

	m_fFocusToEndAfterTime = 0.f;

	// �ڸ�
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
	float fFlexibility = 100.f; // ȭ�鿡 ����� �߸��� ������Ʈ���� ���̳�.. 



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

		if(m_vScroll.x <= fEndScrollX) // ����ŭ �������� ���ش�
		{
			m_bIsFocusToEnd = false;

			// �� ������ �ڸ�11 ����.
			if(m_bIsMakeCompleteSub11 == false) // ���� wakeup�� �ڸ�2 ����.
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
	if(m_fFocusToEndAfterTime >= 5.f) // �� �����ߴٰ� �ٷ� ������ ����, �ð� ��
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

		if(m_vScroll.x >= fOriginScrollX) // ����ŭ �������� ���ش�
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
	// true�� ���� ������
	if(g_bIsShaking == false)
	{
		m_iShakingCount = 0;
		return;
	}

	// �ʱ� ��ũ�� �� ����
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


	// ��鸲_�� �϶� ��ũ�� �� ����.
	if(m_bIsShakingToRight == true)
	{
		m_vScroll.x -= m_fShakingSpeed;
	}
	// ��鸲_�� �϶� ��ũ�� �� ����.
	if(m_bIsShakingToLeft == true)
	{
		m_vScroll.x += m_fShakingSpeed;
	}
	// ��鸲_�� �϶� ��ũ�� �� ����.
	if(m_bIsShakingToBottom == true)
	{
		m_vScroll.y -= m_fShakingSpeed;
	}
	// ��鸲_�� �϶� ��ũ�� �� ����.
	if(m_bIsShakingToTop == true)
	{
		m_vScroll.y += m_fShakingSpeed;
	}

	// ��鸲 ����_�»� ����. // ���� ����� ��ũ�Ѱ��� (���� ��ũ�Ѱ� - ����)�� �����.
	if( m_vScroll.x >= m_fOriginScrollX + fX) // ��			`	-> ��
	{
		m_bIsShakingToLeft = false;
		m_bIsShakingToRight = true;

		m_iShakingCount++;
	}
	if( m_vScroll.x < m_fOriginScrollX - fX ) // �� -> ��
	{
		m_bIsShakingToLeft = true;
		m_bIsShakingToRight = false;

		m_iShakingCount++;
	}

	if( m_vScroll.y >= m_fOriginScrollY + fY) // �� -> ��
	{
		m_bIsShakingToBottom = true;
		m_bIsShakingToTop = false;
		m_iShakingCount++;
	}
	if( m_vScroll.y < m_fOriginScrollY - fY) //�� -> ��
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