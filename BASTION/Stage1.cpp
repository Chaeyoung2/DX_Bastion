#include "StdAfx.h"
#include "Stage1.h"
#include "Factory.h"
#include "BlackScreen.h"
#include "Player.h"
#include "GasPella.h"
#include "Tile.h"
#include "Structure.h"
#include "Ground.h"
#include "Defense.h"
#include "HPBar.h"
#include "XPBar.h"
#include "IconGun.h"
#include "IconHammer.h"
#include "MyMouse.h"
#include "Bandibul.h"
#include "SceneMgr.h"
#include "Device.h"
#include "SubTitle.h"
#include "Hammer.h"
#include "Portal.h"



CStage1::CStage1(void)
:m_fTime(0.f)
{
}

CStage1::~CStage1(void)
{
	Release();
}

HRESULT CStage1::Initialize(void)
{


	g_eScene =  SCENE_STAGE1;

// 	// PathFind
// 	if(FAILED(CTextureMgr::GetInstance()->LoadImagePath(L"../Data/MultiImagePath.txt")))
// 	{
// 		MessageBox(g_hWnd, L"이미지 패스 로드 실패!!", L"에러!", MB_OK);
// 		return E_FAIL;
// 	}

	// Insert Texture
// 	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/Tile/Tile%d.png",
// 		L"TILE",
// 		TEX_MULTI,
// 		L"Tile",
// 		124)))
// 	{
// 		MessageBox(g_hWnd, L"멀티 텍스쳐 로드 실패", L"에러!", MB_OK);
// 		return E_FAIL;
// 	}
// 	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/BackGround/BackGround%d.png",
// 		L"BACKGROUND",
// 		TEX_MULTI,
// 		L"BackGround",
// 		4)))
// 	{
// 		MessageBox(g_hWnd, L"멀티 텍스쳐 로드 실패", L"에러!", MB_OK);
// 		return E_FAIL;
// 	}


	
	// SubTitle
// 	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(
// 		L"../Texture/UI/SubTitle/%d.png",
// 		L"SUBTITLE",
// 		TEX_MULTI,
// 		L"SubTitle",
// 		2)))
// 	{
// 		MessageBox(g_hWnd, L"멀티 텍스쳐 로드 실패", L"에러!", MB_OK);
// 		return E_FAIL;
// 	}

	


	// UI
	m_pObjMgr->AddObject(
		CFactory<CHPBar>::CreateGameObject(), OBJ_UI);
	m_pObjMgr->AddObject(
		CFactory<CXPBar>::CreateGameObject(), OBJ_UI);
	m_pObjMgr->AddObject(
		CFactory<CIconGun>::CreateGameObject(), OBJ_UI);
	m_pObjMgr->AddObject(
		CFactory<CIconHammer>::CreateGameObject(), OBJ_UI);

// 	m_pObjMgr->AddObject(
// 		CFactory<CMyMouse>::CreateGameObject(), OBJ_MOUSE);

	// GameObject
	m_pObjMgr->AddObject( 
		CFactory<CTile>::CreateGameObject(), OBJ_TILE);

//	m_pObjMgr->AddObject( 
//		CFactory<CStructure>::CreateGameObject(), OBJ_STRUCTURE);

	AddStructure();

	m_pObjMgr->AddObject( 
		CFactory<CGround>::CreateGameObject(), OBJ_GROUND);

	m_pObjMgr->AddObject( 
		CFactory<CPlayer>::CreateGameObject(), OBJ_PLAYER);

	m_pObjMgr->AddObject( 
		CFactory<CGasPella>::CreateGameObject(4866.f, 695.f), OBJ_MONSTER);

	// 반딧불
	//AddBandibul();

	// 아이템 해머
	m_pObjMgr->AddObject( 
		CFactory<CHammer>::CreateGameObject(), OBJ_ITEM);

	//k
	m_pObjMgr->AddObject( 
		CFactory<CPortal>::CreateGameObject(5242.f, 243.f), OBJ_PORTAL);

	// 처음에 작은 렉트로 시작해얗 ㅏㅁ
	g_bIsMenuToStage1 = true;


	// 페이드인
	g_bIsFadeOut = false;
	g_bIsFadeIn = true;
	


	return S_OK;
}

eOBJECT_RESULT CStage1::Update(void)
{
	m_pObjMgr->UpdateObj();

	AddBandibul();

	return OR_OK;
}

void CStage1::Render(void)
{
	if(g_eScene != SCENE_DIA)
	{
		// 맵 렌더
		const TEX_INFO* pMapTexture
			= m_pTextureMgr->GetTexture(L"BackGround", L"BackGround", 0);
		float fMapX = pMapTexture->ImgInfo.Height * 0.5f;
		float fMapY = pMapTexture->ImgInfo.Width * 0.5f;

		D3DXMATRIX matMapTrans;

		D3DXMatrixTranslation(&matMapTrans, 
			fMapX /* + m_vScroll.x*/, 
			fMapY /*+ m_vScroll.y*/, 0.f);

		m_pDevice->GetSprite()->SetTransform(&matMapTrans);


		m_pDevice->GetSprite()->Draw(
			pMapTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fMapX, fMapY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}


	m_pObjMgr->RenderObj();

	

	//// 자막 테스트용 렌더
	//const TEX_INFO*	pTexture = 
	//	CTextureMgr::GetInstance()->GetTexture(L"SUBTITLE", L"SubTitle", 0);

	//D3DXMATRIX matTrans;



	//if(NULL == pTexture)
	//	return;

	//float fX = pTexture->ImgInfo.Width * 0.5f;
	//float fY = pTexture->ImgInfo.Height * 0.5f;

	//D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY - fY * 2.f, 0.f);


	//CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	//CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, 
	//	NULL, // 출력할 이미지의 크기를 결정, 렉트로 지정
	//	&D3DXVECTOR3(fX, fY, 0.f), // 출력할 이미지의 센터 지점을 결정
	//	NULL, // 출력할 이미지의 위치를 지정
	//	D3DCOLOR_ARGB(255, 255, 255, 255));


}

void CStage1::Release(void)
{
	m_pObjMgr->ReleaseObj();
}

void CStage1::AddStructure()
{

	// LoadData

	HANDLE hFile = CreateFile(
		L"../Data/StructureData.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwByte = 0;

	while(true)
	{
		MYOBJ* pObj = new MYOBJ;

		ReadFile(hFile, pObj, sizeof(MYOBJ), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pObj);
			break;
		}

// 		CGameObject* pMapObject = new CGameObject;
// 		dynamic_cast<CMapObject*>(pMapObject)->SetObjInfo(pObj);

	m_pObjMgr->AddObject(
		CFactory<CMapObject>::CreateStructure( pObj ), OBJ_STRUCTURE);


		//m_StructureList.push_back(pMapObject);
	}

	CloseHandle(hFile);


	//
// 	list<PMYOBJ>::iterator iter = m_StructureList.begin();
// 	list<PMYOBJ>::iterator iter_end = m_StructureList.end();
// 
// 	for(; iter != iter_end; ++iter)
// 	{
// // 		m_pObjMgr->AddObject(
// // 			CFactory<CMapObject>::CreateGameObject(), OBJ_STRUCTURE);
// 		m_pObjMgr->AddObject( (*iter), OBJ_STRUCTURE);
// 	}

}

void CStage1::AddBandibul()
{
	// 0914 패턴 다르게!
// 	float fAngle = 30.f;
// 
// 	for(int i = 0; i < 20; ++i)
// 	{
// 		m_pObjMgr->AddObject( 
// 			CFactory<CBandibul>::CreateBandibul(
// 			WINCX * (i * 0.1f), WINCY + (i * 0.3f), fAngle + 10 * i), OBJ_EFFECT);
// 	}
	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime >= 5.f)
	{
		float fSpeed = 2.f;
		float fAngle = 0.f;
		float fDistance = 500.f;


		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);

		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.1f, (float)WINCY, fAngle, fSpeed * 0.5f, fDistance * 5), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.3f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance), OBJ_BANDIBUL);
		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.8f, (float)WINCY, fAngle, fSpeed * 3.f, fDistance * 3), OBJ_BANDIBUL);

		m_pObjMgr->AddObject(
			CFactory<CBandibul>::CreateBandibul(
			WINCX * 0.5f, (float)WINCY, fAngle, fSpeed, fDistance * 0.1f), OBJ_BANDIBUL);

		 m_fTime = 0.f;
	}

	


}
