#include "StdAfx.h"
#include "Player.h"
#include "Device.h"
#include "Mouse.h"
#include "Tile.h"
#include "Factory.h"
#include "Bullet.h"
#include "EffectPotion.h"
#include "EffectHammer.h"
#include "EffectBreak.h"
#include "DataSubject.h"
#include "SceneMgr.h"
// #include "EffectFactory.h"
// #include "EffectBuff.h"
// #include "DataSubject.h"

CPlayer::CPlayer(void)
:m_pKeyMgr(CKeyMgr::GetInstance()), m_pAstar(CAStar::GetInstance())
{
}

CPlayer::~CPlayer(void)
{
}

HRESULT CPlayer::Initialize(void)
{
	m_tFrame = FRAME(0.f, 0.f, 5.f, 5.f);

	m_tData.iAtt = 100;
	m_tData.iHp = 2000;
	m_tData.iXp = 0;
	m_tData.bIsHammer = true;
	m_tData.bIsGun = true;

	m_eRenderID = RENDER_OBJ;

	CDataSubject::GetInstance()->AddData(PLAYER_DATA, &m_tData);
	// 	CDataSubject::GetInstance()->Notify(MESSAGE_PLAYER_DATA, &m_tData);
	CDataSubject::GetInstance()->AddData(PLAYER_MATRIX, &m_tInfo.matWorld);
	// 	CDataSubject::GetInstance()->Notify(MESSAGE_PLAYER_MATRIX, &m_tInfo.matWorld);

	m_tInfo.vPos = D3DXVECTOR3(465, 2810.f, 0);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(500, 500, 0);

	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"Lie";


	m_bIsStateChanged = false;


	m_eDir = DIR_B;

	m_eCurState = PLAYER_LIE;
	m_ePreState = m_eCurState;

	m_fSpeed = 180.f;
	m_fScaleX = 1.f;
	m_fScaleY = 1.f;
	m_fScaleSpeed = 0.01f;


	m_bIsNoDirection = false; // 방향 없는 스프라이트 돌릴 때 구분 위함.


	m_fOffSetX = WINCX * 0.2f;
	m_fOffSetY = WINCY * 0.2f;


	// 자막.
	m_pSubTitle = (m_pObjMgr->FindGameObject<CSubTitle>());


	// 자막
	m_bIsMakeCompleteSub2 = false;
	m_bIsMakeCompleteSub3 = false;
	m_bIsMakeCompleteSub4 = false;
	m_bIsMakeCompleteSub5 = false;
	m_bIsMakeCompleteSub6 = false;
	m_bIsMakeCompleteSub7 = false;
	m_bIsMakeCompleteSub8 = false;
	m_bIsMakeCompleteSub9  = false;
	m_bIsMakeCompleteSub10 = false;
	m_bIsMakeCompleteSub11 = false;
	m_bIsMakeCompleteSub12 = false;
	m_bIsMakeCompleteSub13 = false;
	m_bIsMakeCompleteSub14 = false;
	m_bIsMakeCompleteSub15 = false;
	m_bIsMakeCompleteSub20 = false;
	m_bIsMakeCompleteSub21 = false;
	m_bIsMakeCompleteSub23 = false;
	m_bIsMakeCompleteSub24 = false;
	m_bIsMakeCompleteSub25 = false;
	m_bIsMakeCompleteSub26 = false;
	m_bIsMakeCompleteSub27 = false;
	m_bIsMakeCompleteSub28 = false;

	m_fSub12ToSub13Time = 0.f;
	m_bIsCountingSub12ToSub13Time = false;


	// 아이템
	//m_pHammer = m_pObjMgr->FindGameObject<CHammer>();

	// 흔들림 처리 관련
	m_fShakingRangeX = 0.f;
	m_fShakingRangeY = 0.f;

	m_bIsMeetHammerInit = true;

	// 포탈
	m_bIsGetReadyToPortal = true;
	m_bIsCompletedSettingPosInSceneChange = false;
	m_iWhereToGo = 1; // 스테이지1->디아

	// 소리
	m_bIsPlayedHammer = false;

	// 피격시 색깔

	m_iColor = 0;
	m_bIsDamaged = false;

	m_fDamageTime = 0.f;

	return S_OK;
}

eOBJECT_RESULT CPlayer::Update(void)
{
	// 스테이지 넘어갔으면 스크롤 원위치
	if(m_bIsGetReadyToPortal == false)
	{
		if(m_bIsCompletedSettingPosInSceneChange == false)
		{
			switch(g_eScene)
			{
			case SCENE_DIA: // 넘어간 스테이지가 디아펍이라면
				{
					if(m_bIsMakeCompleteSub14 == false)
					{
						m_pObjMgr->AddObject( 
							CFactory<CSubTitle>::CreateSubTitle(14.f), OBJ_SCREEN);
						m_pSoundMgr->PlaySound(L"Narrator14f.MP3", CHANNEL_SUBTITLE20);
						m_bIsMakeCompleteSub14 = true;
					}
					m_tInfo.vPos = D3DXVECTOR3(960.f, 1405.f, 0.f);
					m_bIsCompletedSettingPosInSceneChange = true;
				}
				
				break;
			case SCENE_STAGE2: // 넘어간 스테이지가 디아펍이라면
				m_tInfo.vPos = D3DXVECTOR3(947.f, 1179.f, 0.f);
				/*m_tInfo.vPos = D3DXVECTOR3(203.f, 2222.f, 0.f);*/
				m_bIsCompletedSettingPosInSceneChange = true;
				break;
			}
		}
	}

	// 피격당할시 빨간색으로 렌더
	if(m_bIsDamaged == true)
	{
		CDataSubject::GetInstance()->Notify(PLAYER_DATA, &m_tData);

		m_fDamageTime += m_pTimeMgr->GetTime();

		m_iColor = 0;

		if(m_fDamageTime >= 1.f)
		{
			m_iColor = 255;
			m_bIsDamaged = false;

			if(m_bIsInvincible == true)
			{
				m_bIsInvincible = false;
				m_fDamageTime = 0.f;

			}
		}
	}




	// 치트키
	if(GetAsyncKeyState(VK_F1))
	{
		CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE2);
		m_bIsGetReadyToPortal = false;
	}
	if(GetAsyncKeyState('2'))
	{
		m_tData.iHp -= 10;
		CDataSubject::GetInstance()->Notify(PLAYER_DATA, &m_tData);
	}
/*		g_bIsUIRender = true;*/

	// 씬 전환
// 
//  		system("cls");
//  		cout << "m_vScroll.x :"  << m_vScroll.x <<endl; 
//  		cout << "m_vScroll.y :" 	<< m_vScroll.y <<endl;
//  		cout << "m_tInfo.vPos.x :"	<< m_tInfo.vPos.x<<endl;
//  		cout << "m_tInfo.vPos.y :" << m_tInfo.vPos.y<<endl;
// 
// 	
//   	cout << "m_bIsCompletedSettingPosInSceneChange  :" << m_bIsCompletedSettingPosInSceneChange <<endl;
//  	cout << "m_bIsGetReadyToPortal :" <<  m_bIsGetReadyToPortal <<endl;


	// 유닛 좌표 찍기
// 	D3DXVECTOR3 vMousePos = CMouse::GetMousePos() - m_vScroll;
// 
// 	system("cls");
// 	cout << vMousePos.x << endl;
// 	cout << vMousePos.y << endl;

	//게스펠라 쪽으로 스크롤 움직이기
// 	system("cls");
// 	cout << "m_vScroll.x : " << m_vScroll.x << endl;
// 	cout << "m_vScroll.y : " << m_vScroll.y << endl;
// 	cout << "m_bIsFocusOnGaspella : " << CGameObject::m_bIsFocusOnGaspella << endl;
// 	cout << "m_bIsFocusToEnd : " << CGameObject::m_bIsFocusToEnd << endl;
// 	cout << "m_bIsFocusToOrigin : " << CGameObject::m_bIsFocusToOrigin << endl;

	// 자막12->13로 넘어갈 때 카운트
	if(m_bIsCountingSub12ToSub13Time == true)
	{
		m_fSub12ToSub13Time += m_pTimeMgr->GetTime();

		if(m_fSub12ToSub13Time >= 4.f)
		{
			if(m_bIsMakeCompleteSub13 == false)
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(13.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator14.MP3", CHANNEL_SUBTITLE14);
				m_bIsCountingSub12ToSub13Time = false;
				m_bIsMakeCompleteSub13 = true;
			}
		}
	}
	// 화면 흔들림
	if(m_bIsMeetHammerInit == false) // 해머 처음 만났을 때 화면 흔들림 처리.
	{
		m_fShakingRangeX = 5.f;
		m_fShakingRangeY = 5.f;

		g_bIsShaking = true;

		g_bIsZoomIn = true;

		if(m_bIsMakeCompleteSub6 == false)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(6), OBJ_SCREEN);
			m_pSoundMgr->PlaySound(L"Narrator07.MP3", CHANNEL_SUBTITLE07);

			m_bIsMakeCompleteSub6 = true;
		}

		m_bIsMeetHammerInit = true;		
	}

	if(m_bIsMeetGunInit == false) // 건 처음 만났을 때 화면 흔들림 처리.
	{
		m_fShakingRangeX = 5.f;
		m_fShakingRangeY = 5.f;

		g_bIsShaking = true;

		g_bIsZoomIn = true;

		if(m_bIsMakeCompleteSub15 == false)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(15), OBJ_SCREEN);
			m_pSoundMgr->PlaySound(L"Narrator16.MP3", CHANNEL_SUBTITLE16);

			m_bIsMakeCompleteSub15 = true;
		}

		m_bIsMeetGunInit = true;		
	}
	
	CGameObject::IsShaking(m_fShakingRangeX, m_fShakingRangeY);
	CGameObject::FocusOnGaspella();

	FrameSettingInState();
	FrameCheck();

	SettingCollRect();
	SettingHammerRect();

	MatrixCheck();
	KeyCheck();
	AStarMove();

	MakeEffectInFrame(); // 프레임 장수에 따라 이펙트 주기.

	Scroll(); // 스크롤
	// 
	// 	system("cls");
	// 
	// 	cout << "Player" << endl;
	// 	cout << "m_tInfo.vPos.y: " << m_tInfo.vPos.y << endl << endl;


	return OR_OK;
}

void CPlayer::Render(void)
{
	const TEX_INFO* pTex = m_pTextureMgr->GetTexture(
		m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(), static_cast<int>(m_tFrame.fFrame) );

	if(pTex == NULL)		return;

	float fX = pTex->ImgInfo.Width * 0.5f;
	float fY = pTex->ImgInfo.Height * 0.5f + PLAYERCY_ORI * 0.3f;

	m_pSprite->SetTransform(&m_tInfo.matWorld);

	if(m_bIsDamaged == false)
	{
		m_pSprite->Draw(
			pTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
			);
	}
	if(m_bIsDamaged == true)
	{
		m_pSprite->Draw(
			pTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 0, 0)
			);
	}
	



	// 	// 충돌 박스 렌더
	// 
	// 	D3DXVECTOR2		vPoint[4] = 
	// 	{
	// 		D3DXVECTOR2(m_tHammerRect.left, m_tHammerRect.top),
	// 		D3DXVECTOR2(m_tHammerRect.right, m_tHammerRect.top),
	// 		D3DXVECTOR2(m_tHammerRect.right, m_tHammerRect.bottom),
	// 		D3DXVECTOR2(m_tHammerRect.left, m_tHammerRect.bottom)
	// 	};
	// 
	// 	CDevice::GetInstance()->Render_End();
	// 	CDevice::GetInstance()->Render_Begin();
	// 
	// 	CDevice::GetInstance()->GetLine()->Draw(vPoint, 4, D3DCOLOR_ARGB(255, 0, 0, 0));
	// 
	// 	CDevice::GetInstance()->Render_End();
	// 	CDevice::GetInstance()->Render_Begin();
}

void CPlayer::Release(void)
{

}

void CPlayer::FrameCheck(void)
{
	if( m_eCurState == PLAYER_WAKEUP )
	{
		
		/*cout << "m_tFrame.fFrame: " << m_tFrame.fFrame << endl;*/
		m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime() * 0.001f;
	}
	if( m_eCurState == PLAYER_ROLL )
	{
		m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime() * 2.f;
	}
	else
	{
		m_tFrame.fFrame += m_tFrame.fCount * m_pTimeMgr->GetTime();
	}

	

	if(m_tFrame.fFrame >= m_tFrame.fMaxFrame)
	{
		if(m_eCurState == PLAYER_GUN || m_eCurState == PLAYER_HAMMER
			 || m_eCurState == PLAYER_WAKEUP
			|| m_eCurState == PLAYER_STANDUP || m_eCurState == PLAYER_DRINKING)
		{
			if(m_eCurState == PLAYER_HAMMER)
			{
				m_bIsPlayedHammer = false;
			}
			if(m_eCurState == PLAYER_GUN)
			{
				m_bIsPlayedGun = false;
			}
			m_eCurState = PLAYER_IDLE;
			m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
		}
		else if( m_eCurState == PLAYER_ROLL)
		{
			m_eCurState = PLAYER_RUN;
			m_fSpeed = 180.f;
			m_tFrame = FRAME(32.f, 32.f, (51.f - 32.f), 51.f);
		}

		m_tFrame.fFrame = m_tFrame.fOriFrame;
	}

	// 	system("cls");
	// 	cout << "m_eCurState : " << m_eCurState << endl;
	// 	cout << "m_tFrame.fCount : " << m_tFrame.fCount << endl;
	// 	cout << "m_tFrame.fFrame : " << m_tFrame.fFrame << endl;
	// 	cout << "m_tFrame.fMaxFrame : " << m_tFrame.fMaxFrame << endl;
	// 	cout << "m_tFrame.fOriFrame : " << m_tFrame.fOriFrame << endl;
}



void CPlayer::MatrixCheck(void)
{
	D3DXMATRIX matTrans, matScale;

	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y,
		m_tInfo.vPos.z
		);

	//system("cls");
	//cout << "m_fScaleX : " << m_fScaleX << endl;
	//cout << "m_fScaleY : " << m_fScaleY << endl;
	//cout << "m_eDir : " << m_eDir << endl;
	//cout << "m_eCurState : " << m_eCurState << endl;

	if(m_eCurState == PLAYER_DROP )
	{
		if(m_eDir == DIR_R || m_eDir == DIR_RB || m_eDir == DIR_RT)
		{
			//float m_fReverseScaleSpeed = -m_fScaleSpeed;

			if(m_fScaleX <= 0)
			{
				m_fScaleX += m_fScaleSpeed;
			}
			if(m_fScaleY >= 0)
			{
				m_fScaleY -= m_fScaleSpeed;
			}
			if(m_fScaleX > 0 || m_fScaleY < 0)
			{
				m_eCurState = PLAYER_STANDUP;
				m_bIsNoDirection = false;
				m_fScaleX = -1.f;
				m_fScaleY = 1.f;
			}
		}
		else
		{
			if(m_fScaleX >= 0 || m_fScaleY >= 0)
			{

				m_fScaleX -= m_fScaleSpeed;
				m_fScaleY -= m_fScaleSpeed;
			}
			else
			{
				m_eCurState = PLAYER_STANDUP;
				m_fScaleX = 1.f;
				m_fScaleY = 1.f;
			}
		}


	}


	D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, 1.f);

	m_tInfo.matWorld = matScale * matTrans;

	//	CDataSubject::GetInstance()->Notify(MESSAGE_PLAYER_MATRIX, &m_tInfo.matWorld);

}

void CPlayer::KeyCheck(void)
{
	// RUN
	if(m_pKeyMgr->OnceKeyUp(VK_LBUTTON))
	{
		m_fSpeed = 180.f;
		// 		POINT ptScroll;
		// 		ptScroll.x = m_vScroll.x;
		// 		ptScroll.y = m_vScroll.y;
		// 
		// 		ScreenToClient(g_hWnd, &ptScroll);
		// 
		// 		D3DXVECTOR3 vScroll = D3DXVECTOR3(ptScroll.x, ptScroll.y, 0.f);


		D3DXVECTOR3 vMousePos = CMouse::GetMousePos() - m_vScroll;

		int iOption = m_pAstar->AStarStartPos(m_tInfo.vPos, vMousePos);

		switch(iOption)
		{
		case 0:
			break;
		case 1:
			m_eCurState = PLAYER_RUN;
			break;
		case 2:
			{
				//m_eCurState = PLAYER_DROP;
			}
			break;
		case 3:
			m_eCurState = PLAYER_RUN;
			break;
		case 4:
			m_eCurState = PLAYER_RUN;
			break;
		case 5:
			m_eCurState = PLAYER_RUN;
			break;
		default:
			break;
		}


		m_bIsStateChanged = true;
	}
	// ATTACK_HAMMER
	if(m_pKeyMgr->OnceKeyUp('Q'))
	{
		D3DXVECTOR3 vMousePos = GetMousePos() - m_vScroll;
		D3DXVECTOR3 vMouseToPlayer = vMousePos - m_tInfo.vPos;
		DirSettingInAttack(vMouseToPlayer);


		m_pSoundMgr->PlaySound(L"Hammer01.MP3", CHANNEL_HAMMER01);

		m_eCurState = PLAYER_HAMMER;
 

// 		if(m_bIsGetReadyToPortal == false)
// 			m_bIsGetReadyToPortal = true;
// 		if(m_bIsCompletedSettingPosInSceneChange == false)
// 			m_bIsCompletedSettingPosInSceneChange = true;
	}

	// ATTACK_GUN
	if(m_pKeyMgr->OnceKeyUp('W'))
	{
		D3DXVECTOR3 vMousePos = GetMousePos() - m_vScroll;
		D3DXVECTOR3 vMouseToPlayer = vMousePos - m_tInfo.vPos;

		if(m_bIsPlayedGun == false)
		{
		m_pSoundMgr->PlaySound(L"Gun.wav", CHANNEL_PLAYERGUN);
		m_bIsPlayedGun = true;
		}
		CObjMgr::GetInstance()->AddObject( 
			CreateBullet<CBullet>(vMouseToPlayer, m_tInfo.vPos), 
			OBJ_PLAYER_BULLET);
		DirSettingInAttack(vMouseToPlayer);
		m_eCurState = PLAYER_GUN;
	}

	// DRINKING
	if(m_pKeyMgr->OnceKeyUp('E'))
	{
		if(m_bIsMakeCompleteSub21 == false)
		{
			m_pObjMgr->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(21.f), OBJ_SCREEN);
			m_pSoundMgr->PlaySound(L"Gulp.MP3", CHANNEL_GULP);
			m_pSoundMgr->PlaySound(L"Narrator18.MP3", CHANNEL_SUBTITLE22);

			m_bIsMakeCompleteSub21 = true;


			m_bIsGetReadyToPortal = true;
			m_bIsCompletedSettingPosInSceneChange = false;
		}
		m_eCurState = PLAYER_DRINKING;

		CObjMgr::GetInstance()->AddObject( 
			CreateEffect<CEffectPotion>(/*m_tInfo.vDir, */m_eDir), 
			OBJ_EFFECT);
		m_tData.iHp += 100;
		CDataSubject::GetInstance()->Notify(PLAYER_DATA, &m_tData);
	}

	// WAKEUP & ROLL
	if(m_pKeyMgr->OnceKeyUp(VK_SPACE))
	{
		if(m_eCurState == PLAYER_LIE)
		{
			m_eCurState = PLAYER_WAKEUP;
			
			if(m_bIsMakeCompleteSub2 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pSoundMgr->StopSoundAll();
				m_pSoundMgr->PlayBGM(L"TwistedStreets.mp3", CHANNEL_BGM, 0.7f);
				m_pSoundMgr->PlaySound(L"Narrator03.MP3", CHANNEL_SUBTITLE03);
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(2.f), OBJ_SCREEN);
				m_bIsMakeCompleteSub2 = true;
			}
		}
		else
		{
			m_eCurState = PLAYER_ROLL;
			m_pSoundMgr->PlaySound(L"Roll.MP3", CHANNEL_ROLL);
			m_fSpeed = 180.f * 2.f;
		}
	}

// 	if(m_pKeyMgr->OnceKeyDown(VK_UP))
// 	{
// 		g_bIsZoomIn = true;
// 	}
// 
// 	if(m_pKeyMgr->OnceKeyDown(VK_DOWN))
// 	{
// 		g_bIsZoomOut = true;
// 	}
}

void CPlayer::AStarMove(void)
{
	list<int>* pBestList = m_pAstar->GetBestList();
         
	if(pBestList->empty())
		return;

	const vector<TILE*>* pVecTile = m_pObjMgr->FindGameObject<CTile>()->GetTile();

	int iMoveIndex = pBestList->front();
	int iEndIndex  = pBestList->back();

	// 타일 인덱스에 따른 자막
	CheckIndexForSubTitle(iMoveIndex);
	m_iMoveIndex = iMoveIndex;
	

	m_tInfo.vDir = ((*pVecTile)[iMoveIndex])->vPos - m_tInfo.vPos;

	float fDist = D3DXVec3Length(&m_tInfo.vDir);

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetTime() * m_fSpeed;



	// 마지막 인덱스 도달시 플레이어 상태 구분
	D3DXVECTOR3 vPlayerToEnd = ((*pVecTile)[iEndIndex])->vPos - m_tInfo.vPos;
	float fPlayerToEndDist = D3DXVec3Length(&vPlayerToEnd);

	if(fPlayerToEndDist < 3.f)
	{
		if(m_eCurState != PLAYER_DROP)
			m_eCurState = PLAYER_IDLE;
	}

	// 현재 타일이 추락 타일이라면
	if( ((*pVecTile)[iEndIndex])->byOption == 2 )
	{
		// 클릭하자마자 drop 모션 취하는 거 방지..
		if(fPlayerToEndDist < 5.f)
		{
			m_eCurState = PLAYER_DROP;
			
			if(m_bIsMakeCompleteSub7 == false)
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(7.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator08.MP3", CHANNEL_SUBTITLE08);

				m_bIsMakeCompleteSub7 = true;
			}
			
		}
		else
			m_eCurState = PLAYER_RUN;
	}


	// #ifdef _DEBUG
	// 	cout << "fDist: " << fDist << endl;
	// 	cout << "fPlayerToEndDist: " << fPlayerToEndDist << endl;
	// #endif

	if(fDist < 3.f)
	{
		pBestList->pop_front();
		return;
	}

	// 플레이어 방향 세팅
	// DirSettingInMoving( ((*pVecTile)[iMoveIndex])->vPos );

	DirSettingInMoving( m_tInfo.vDir );
}

void CPlayer::FrameSettingInState()
{
	if( m_eCurState != m_ePreState )
	{
		// 방향 O
		if(m_eCurState == PLAYER_IDLE || m_eCurState == PLAYER_RUN ||
			m_eCurState == PLAYER_HAMMER || m_eCurState == PLAYER_ROLL ||
			m_eCurState == PLAYER_GUN)
		{
			m_bIsNoDirection = false;

			switch(m_eCurState)
			{
			case PLAYER_IDLE:
				m_tFrame = FRAME(0.f, 0.f, 31.f, 31.f);
				break;
			case PLAYER_RUN:
				m_tFrame = FRAME(32.f, 32.f, (51.f - 32.f), 51.f);
				break;
			case PLAYER_HAMMER:
				m_tFrame = FRAME(52.f, 52.f, (75.f - 52.f), 75.f);
				break;
			case PLAYER_ROLL:
				m_tFrame = FRAME(76.f, 76.f, (88.f - 76.f), 88.f);
				break;
			case PLAYER_GUN:
				m_tFrame = FRAME(89.f, 89.f, (100.f - 89.f), 100.f);
				break;
			}

			switch(m_eDir)
			{
			case DIR_B:
				m_wstrStateKey = L"Dir_B";
				break;
			case DIR_L:
				m_wstrStateKey = L"Dir_L";
				break;
			case DIR_LB:
				m_wstrStateKey = L"Dir_LB";
				break;
			case DIR_LT:
				m_wstrStateKey = L"Dir_LT";
				break;
			case DIR_T:
				m_wstrStateKey = L"Dir_T";
				break;
			}
		}

		// 방향 X
		if(m_eCurState == PLAYER_DEATH || m_eCurState == PLAYER_DRINKING ||
			m_eCurState == PLAYER_DROP || m_eCurState == PLAYER_LIE ||
			m_eCurState == PLAYER_STANDUP || m_eCurState == PLAYER_WAKEUP)
		{
			m_bIsNoDirection = true;

			switch(m_eCurState)
			{
			case PLAYER_DEATH:
				m_wstrStateKey = L"Death";
				m_tFrame = FRAME(0.f, 0.f, 37.f, 37.f);
				break;

			case PLAYER_DRINKING:
				m_wstrStateKey = L"Drinking";
				m_tFrame = FRAME(0.f, 0.f, 22.f, 22.f);
				break;

			case PLAYER_DROP:
				m_wstrStateKey = L"Drop";
				m_tFrame = FRAME(0.f, 0.f, 30.f, 30.f);
				break;

			case PLAYER_LIE:
				m_wstrStateKey = L"Lie";
				m_tFrame = FRAME(0.f, 0.f, 5.f, 5.f);
				break;

			case PLAYER_STANDUP:
				m_wstrStateKey = L"StandUp";
				m_tFrame = FRAME(0.f, 0.f, 65.f, 65.f);
				break;

			case PLAYER_WAKEUP:
				m_wstrStateKey = L"WakeUp";
				m_tFrame = FRAME(0.f, 0.f, 64.f, 64.f);
				break;
			}

			m_tFrame.fCount = m_tFrame.fMaxFrame;
		}

		m_ePreState = m_eCurState;
	}
}

// 방1. atan2
void CPlayer::DirSettingInMoving(D3DXVECTOR3 vDir)
{
	// 방향이 필요 없을 땐 설정해줄 필요 업뜸
	if(m_bIsNoDirection == true)
		return;

	// 각도 구하기
	float fRadian = atan2f(vDir.y, vDir.x);
	float fAngle = D3DXToDegree(fRadian);



	// 방향

	if(fAngle >= -15 && fAngle < 15)
	{
		m_eDir = DIR_R;
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = -1.f;
	}
	else if(fAngle >= -75 && fAngle < -15)
	{
		m_eDir = DIR_RT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = -1.f;
	}
	else if(fAngle >= -105 && fAngle < -75)
	{
		m_eDir = DIR_T;
		m_wstrStateKey = L"Dir_T";

	}
	else if(fAngle >= -165 && fAngle < -105)
	{
		m_eDir = DIR_LT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = 1.f;
	}
	else if(fAngle < -165 || fAngle >= 165)
	{
		m_eDir = DIR_L;
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = 1.f;
	}
	else if(fAngle >= 105 && fAngle < 165)
	{
		m_eDir = DIR_LB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = 1.f;
	}
	else if(fAngle >= 75 && fAngle < 105)
	{
		m_eDir = DIR_B;
		m_wstrStateKey = L"Dir_B";
	}
	else if(fAngle >= 15 && fAngle < 75)
	{
		m_eDir = DIR_RB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = -1.f;
	}


}

void CPlayer::SettingCollRect()
{
	float fCollRectY = 60.f;
	float fCollRectX = 45.f;

	m_tCollRect.top = static_cast<LONG>(m_tInfo.vPos.y - fCollRectY);
	m_tCollRect.bottom =static_cast<LONG>(m_tInfo.vPos.y + fCollRectY);
	m_tCollRect.left = static_cast<LONG>(m_tInfo.vPos.x - fCollRectX);
	m_tCollRect.right = static_cast<LONG>(m_tInfo.vPos.x + fCollRectX);
}

void CPlayer::DirSettingInAttack(D3DXVECTOR3 vDir)
{

	// 각도 구하기
	float fRadian = atan2f(vDir.y, vDir.x);
	float fAngle = D3DXToDegree(fRadian);



	// 방향

	if(fAngle >= -15 && fAngle < 15)
	{
		m_eDir = DIR_R;
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = -1.f;
	}
	else if(fAngle >= -75 && fAngle < -15)
	{
		m_eDir = DIR_RT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = -1.f;
	}
	else if(fAngle >= -105 && fAngle < -75)
	{
		m_eDir = DIR_T;
		m_wstrStateKey = L"Dir_T";

	}
	else if(fAngle >= -165 && fAngle < -105)
	{
		m_eDir = DIR_LT;
		m_wstrStateKey = L"Dir_LT";
		m_fScaleX = 1.f;
	}
	else if(fAngle < -165 || fAngle >= 165)
	{
		m_eDir = DIR_L;
		m_wstrStateKey = L"Dir_L";
		m_fScaleX = 1.f;
	}
	else if(fAngle >= 105 && fAngle < 165)
	{
		m_eDir = DIR_LB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = 1.f;
	}
	else if(fAngle >= 75 && fAngle < 105)
	{
		m_eDir = DIR_B;
		m_wstrStateKey = L"Dir_B";
	}
	else if(fAngle >= 15 && fAngle < 75)
	{
		m_eDir = DIR_RB;
		m_wstrStateKey = L"Dir_LB";
		m_fScaleX = -1.f;
	}

}

void CPlayer::MakeEffectInFrame()
{
	if(m_eCurState == PLAYER_HAMMER)
	{
		if(m_tFrame.fFrame >= 69.5f && m_tFrame.fFrame < 70)
		{
			m_pObjMgr->AddObject( 
				CreateEffect<EffectHammer>(m_eDir), 
				OBJ_EFFECT);
			
			if( m_bIsPlayedHammer == false)
			{

			m_pSoundMgr->PlaySound(L"Hammer03.MP3", CHANNEL_HAMMER02);
			m_bIsPlayedHammer = true;
			}
		}
	}
}

void CPlayer::CheckIndexForSubTitle(int iIndex)
{
	if(g_eScene == SCENE_STAGE1)
	{
		if(iIndex == 3785  || iIndex == 3830) // 자막3
		{
			if(m_bIsMakeCompleteSub3 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(3.f), OBJ_SCREEN);
				//m_pSoundMgr->StopSoundAll();
				m_pSoundMgr->StopSound(CHANNEL_SUBTITLE03);
				m_pSoundMgr->PlaySound(L"Narrator04.MP3", CHANNEL_SUBTITLE04);
				m_bIsMakeCompleteSub3 = true;
			}
		}

		if(iIndex == 3117  || iIndex == 3163) // 자막4
		{
			if(m_bIsMakeCompleteSub4 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(4.f), OBJ_SCREEN);

				m_pSoundMgr->StopSound(CHANNEL_SUBTITLE04);
				m_pSoundMgr->PlaySound(L"Narrator05.MP3",CHANNEL_SUBTITLE05);
				m_bIsMakeCompleteSub4 = true;
			}                         
		}

		if(iIndex == 2628  || iIndex == 2673) // 자막5
		{
			if(m_bIsMakeCompleteSub5 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(5.f), OBJ_SCREEN);

				m_pSoundMgr->StopSound(CHANNEL_SUBTITLE05);
				m_pSoundMgr->PlaySound(L"Narrator06.MP3", CHANNEL_SUBTITLE06);
				m_bIsMakeCompleteSub5 = true;
			}
		}

		if(iIndex == 1871  || iIndex == 1917 || iIndex == 1826) // 자막8
		{
			if(m_bIsMakeCompleteSub8 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(8.f), OBJ_SCREEN);

				//m_pSoundMgr->StopSound(CHANNEL_SUBTITLE07);
				m_pSoundMgr->PlaySound(L"Narrator09.MP3", CHANNEL_SUBTITLE09);
				m_bIsMakeCompleteSub8 = true;
			}
		}

		if(iIndex == 1336  || iIndex == 1382 || iIndex == 1427
			|| iIndex == 1473) // 자막9
		{
			if(m_bIsMakeCompleteSub9 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(9.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator10.MP3", CHANNEL_SUBTITLE10);
				m_bIsMakeCompleteSub9 = true;
			}
		}

		if(iIndex == 756  || iIndex == 846 || iIndex == 936
			|| iIndex == 1026 || iIndex == 1116) // 자막10
		{
			if(m_bIsMakeCompleteSub10 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(10.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator11.MP3", CHANNEL_SUBTITLE11);
				m_bIsMakeCompleteSub10 = true;

				CGameObject::m_bIsFocusOnGaspella = true;
			}
		}
	}

	else if(g_eScene == SCENE_DIA)
	{
		if(iIndex == 840  || iIndex == 940 || iIndex == 990
			|| iIndex == 1040 || iIndex == 916 || iIndex == 966
			|| iIndex == 1016 || iIndex == 1066) // 자막10
		{
			if(m_bIsMakeCompleteSub20 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(20.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator20.MP3", CHANNEL_SUBTITLE21);
				m_bIsMakeCompleteSub20 = true;
			}
		}
	}
	
	else if(g_eScene == SCENE_STAGE2)
	{
		if(iIndex == 3745  || iIndex == 3701 || iIndex == 3656) // 자막10
		{
			if(m_bIsMakeCompleteSub23 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(23.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator23f.MP3", CHANNEL_SUBTITLE23);
				m_bIsMakeCompleteSub23 = true;
			}
		}
		if(iIndex == 3711  || iIndex == 3756 || iIndex == 3802) // 자막10
		{
			if(m_bIsMakeCompleteSub24 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(24.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator24.mp3", CHANNEL_SUBTITLE24);
				m_bIsMakeCompleteSub24 = true;
			}
		}
		if(iIndex == 2263  || iIndex == 2218) // 자막10
		{
			if(m_bIsMakeCompleteSub25 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(25.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator25.mp3", CHANNEL_SUBTITLE25);
				m_bIsMakeCompleteSub25 = true;
			}
		}
		if(iIndex ==826  || iIndex == 871 || iIndex == 917) // 자막10
		{
			if(m_bIsMakeCompleteSub26 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(26.f), OBJ_SCREEN);

				m_pSoundMgr->StopSoundAll();
				CSoundMgr::GetInstance()->StopSoundAll();
				m_pSoundMgr->PlayBGM(L"Escape.mp3", CHANNEL_BGM, 1.f);
				m_pSoundMgr->PlaySound(L"Narrator26.mp3", CHANNEL_SUBTITLE26);

				m_bIsMakeCompleteSub26 = true;
			}
		}
		if(iIndex == 1917  || iIndex == 1872 || iIndex == 1828) // 자막10
		{
			if(m_bIsMakeCompleteSub27 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(27.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator27.mp3", CHANNEL_SUBTITLE27);
				m_bIsMakeCompleteSub27 = true;
			}
		}
		if(iIndex == 266  || iIndex == 312 || iIndex == 357 || iIndex == 403) // 자막10
		{
			if(m_bIsMakeCompleteSub28 == false) // 최초 wakeup시 자막2 생성.
			{
				m_pObjMgr->AddObject( 
					CFactory<CSubTitle>::CreateSubTitle(28.f), OBJ_SCREEN);
				m_pSoundMgr->PlaySound(L"Narrator28.mp3", CHANNEL_SUBTITLE28);
				m_bIsMakeCompleteSub28 = true;
			}
		}


	}


// 	if(iIndex == 552  || iIndex == 513 || iIndex == 473 || iIndex == 434) // 자막11
// 	{
// 		if(m_bIsMakeCompleteSub11 == false) // 최초 wakeup시 자막2 생성.
// 		{
// 			m_pObjMgr->AddObject( 
// 				CFactory<CSubTitle>::CreateSubTitle(11.f), OBJ_SCREEN);
// 			m_bIsMakeCompleteSub11 = true;
// 		}
// 	}


}

void CPlayer::SettingHammerRect()
{
	// 	system("cls");
	// 	cout << "m_eDir : " << m_eDir << endl;
	// 	cout << "m_tInfo.vPos.x : " << m_tInfo.vPos.x << endl;
	// 	cout << "m_tInfo.vPos.y : " << m_tInfo.vPos.y << endl;
	// 	cout << "m_tHammerRect.top : " << m_tHammerRect.top << endl;
	// 		cout <<	"m_tHammerRect.bottom : " << m_tHammerRect.bottom << endl;
	// 	cout <<	"m_tHammerRect.left : " << m_tHammerRect.left  << endl;
	// 	cout << "m_tHammerRect.right : " << m_tHammerRect.right << endl;
	if(m_eCurState != PLAYER_HAMMER)
	{
		m_tHammerRect.top = 0;
		m_tHammerRect.bottom = 0;
		m_tHammerRect.left = 0;
		m_tHammerRect.right = 0;
		return;
	}

	if( (m_tFrame.fFrame >= 56/* && m_tFrame.fFrame <= 60*/) ||
		(m_tFrame.fFrame >= 70 /*&& m_tFrame.fFrame <= 73*/) )
	{
		float fHammerSize = 65.f;
		float fIntervalX = 50.f;
		switch(m_eDir)
		{
		case DIR_B:
			{
				m_tHammerRect.top = static_cast<LONG>(m_tInfo.vPos.y + 30);
				m_tHammerRect.bottom = static_cast<LONG>(m_tInfo.vPos.y + 30 + fHammerSize);
				m_tHammerRect.left = static_cast<LONG>(m_tInfo.vPos.x - fHammerSize * 0.5f);
				m_tHammerRect.right = static_cast<LONG>(m_tInfo.vPos.x + fHammerSize * 0.5f);
			}
			break;
		case DIR_L:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y);
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y + fHammerSize);
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x - 30 - fHammerSize);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		case DIR_LB:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y);
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y + fHammerSize);
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x - fHammerSize - 30);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		case DIR_LT:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y - fHammerSize);
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y);
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x - fHammerSize);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		case DIR_R:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y);
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y + fHammerSize);
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x + 30);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		case DIR_RB:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y);
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y + fHammerSize);
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		case DIR_RT:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y - fHammerSize);
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y);
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		case DIR_T:
			{
				m_tHammerRect.top =  static_cast<LONG>(m_tInfo.vPos.y - 30 - fHammerSize );
				m_tHammerRect.bottom =  static_cast<LONG>(m_tInfo.vPos.y - 30 );
				m_tHammerRect.left =  static_cast<LONG>(m_tInfo.vPos.x - 30 - fHammerSize);
				m_tHammerRect.right =  static_cast<LONG>(m_tHammerRect.left + fHammerSize);
			}
			break;
		}

		return;
	}

	else
	{
		m_tHammerRect.top = 0;
		m_tHammerRect.bottom = 0;
		m_tHammerRect.left = 0;
		m_tHammerRect.right = 0;
		return;
	}
}

void CPlayer::Scroll()
{
	if(CGameObject::m_bIsFocusOnGaspella == true)
		return;

	if(g_bIsShaking == true)
		return;


	// 스테이지에 따라 오프셋을 다르게 설정해야 함.
	switch(g_eScene)
	{
	case SCENE_STAGE1:
		break;
	default:
		break;
	}


	float fScrollSpeed = 1.f;
	float fNumberX = WINCX * 0.5f - 50;
	float fNumberY = WINCY * 0.5f - 50;

	// 	system("cls");
	// 	cout << "m_tInfo.vPos.x : " << m_tInfo.vPos.x << endl;
	// 	cout << "m_tInfo.vPos.y : " << m_tInfo.vPos.y << endl;
	// 	cout << "m_vScroll.x : " << m_vScroll.x << endl;
	// 	cout << "m_vScroll.y : " << m_vScroll.y << endl;

	//float fTime = m_pTimeMgr->GetTime();

	if( ( m_tInfo.vPos.x + m_vScroll.x ) > WINCX - fNumberX)
	{
		m_vScroll.x -= m_fSpeed * m_pTimeMgr->GetTime() * fScrollSpeed;
	}
	else if(m_tInfo.vPos.x + m_vScroll.x <= fNumberX)
	{
		m_vScroll.x += m_fSpeed * m_pTimeMgr->GetTime()  * fScrollSpeed;
	}

	if(m_tInfo.vPos.y + m_vScroll.y > WINCY - fNumberY)
	{
		m_vScroll.y -= m_fSpeed * m_pTimeMgr->GetTime() * fScrollSpeed;
	}
	else if(m_tInfo.vPos.y + m_vScroll.y <= fNumberY)
	{
		m_vScroll.y += m_fSpeed * m_pTimeMgr->GetTime() * fScrollSpeed;
	}


	// float fNumber = 300.f;
	//if(m_tInfo.vPos.x > m_fOffSetX/* + fNumber*/)
	//{
	//	m_vScroll.x -= m_fSpeed * GET_TIME;
	//	m_fOffSetX  += m_fSpeed * GET_TIME;
	//}
	//else if(m_tInfo.vPos.x <= m_fOffSetX /*- fNumber*/)
	//{
	//	m_vScroll.x += m_fSpeed * GET_TIME;
	//	m_fOffSetX  -= m_fSpeed * GET_TIME;
	//}

	//if(m_tInfo.vPos.y > m_fOffSetY /*+ fNumber*/)
	//{
	//	m_vScroll.y -= m_fSpeed * GET_TIME;
	//	m_fOffSetY  += m_fSpeed * GET_TIME;
	//}
	//else if(m_tInfo.vPos.y <= m_fOffSetY/* - fNumber*/)
	//{
	//	m_vScroll.y += m_fSpeed * GET_TIME;
	//	m_fOffSetY  -= m_fSpeed * GET_TIME;
	//}


	// 스크롤 막기
	if(m_vScroll.x > 0)
		m_vScroll.x = 0;
// 	if(m_vScroll.y > 0)
// 		m_vScroll.y = 0;
// 
// 	// 씬마다 스크롤 막기
	switch(g_eScene)
	{
	case SCENE_STAGE1:
		{
			if(m_vScroll.x < WINCX - STAGE1CX)
				m_vScroll.x = WINCX - STAGE1CX;
			if(m_vScroll.y < WINCY - STAGE1CY)
				m_vScroll.y = WINCY - STAGE1CY;
		}
		break;
	case SCENE_DIA:
		{
			if(m_vScroll.x < WINCX - 3000)
				m_vScroll.x = WINCX - 3000;
			if(m_vScroll.y < WINCY - 1980)
				m_vScroll.y = WINCY - 1980;
		}
		break;
	default:
		break;
	}

}
