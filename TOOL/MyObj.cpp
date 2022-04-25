#include "StdAfx.h"
#include "MyObj.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TOOLView.h"

CMyObj::CMyObj(void)
: m_iTempCount(0)
{
}

CMyObj::~CMyObj(void)
{
	Release();
}

HRESULT CMyObj::Initialize()
{
	return S_OK;
}

int CMyObj::Update()
{
	return 0;
}

void CMyObj::Render()
{
	// 그라운드 렌더
	list<PMYOBJ>::iterator iter = m_GroundList.begin();
	list<PMYOBJ>::iterator iter_end = m_GroundList.end();
	// 	iter = m_GroundList.end();
	// 	iter_end = m_GroundList.begin();

	for(; iter != iter_end; ++iter)
	{
		TCHAR* szKey = new TCHAR[MAX_STR];
		TCHAR* szKeyPasted = L"GROUND";

		//		(*iter)->wstrName = L"Ground";

		lstrcpy(szKeyPasted, szKey);

		const TEX_INFO* pTexture
			= CTextureMgr::GetInstance()->GetTexture(
			szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);

		float fGroundCX = float(pTexture->ImgInfo.Width);
		float fGroundCY = float(pTexture->ImgInfo.Height);

		(*iter)->vSize = D3DXVECTOR3(fGroundCX, fGroundCY, 0.f);

		float fCX = pTexture->ImgInfo.Width * 0.5f;
		float fCY = pTexture->ImgInfo.Height *0.5f;

		D3DXMATRIX matWorld, matTrans, matRotZ;

		D3DXMatrixTranslation(&matTrans,
			(*iter)->vPos.x - m_pMainView->GetScrollPos(0),
			(*iter)->vPos.y /*- fCY*/ - m_pMainView->GetScrollPos(1),
			0.f);

		D3DXMatrixRotationZ(&matRotZ, (*iter)->fTransformValue);

		matWorld = matRotZ * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		// 선택됐을 때 알파 값 조정 위한 변수
		int iAlpha = 255;

		if(((*iter)->bIsClicked) == true) 
			iAlpha = int(255 * 0.5);
		else 
			iAlpha = 255;

		// draw
		CDevice::GetInstance()->GetSprite()->Draw(
			pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
			NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));

	}

	iter = m_StructureList.begin();
	iter_end = m_StructureList.end();

// 	list<PMYOBJ>::iterator iter = m_StructureList.end();
// 	list<PMYOBJ>::iterator iter_end = m_StructureList.begin();

	// 렌더 옵션
	// 0 world, 1 backWard, 2forward


	for(; iter != iter_end; ++iter)
	{
// 		if( (*iter)->iRenderOption == 2) // 2forward 우선 렌더
// 		{
			TCHAR* szKey = new TCHAR[MAX_STR];
			TCHAR* szKeyPasted = L"STRUCT";

			lstrcpy(szKeyPasted, szKey);

			//		(*iter)->wstrName = L"Struct";

			const TEX_INFO* pTexture
				= CTextureMgr::GetInstance()->GetTexture(
				szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);

			float fStructCX = float(pTexture->ImgInfo.Width);
			float fStructCY = float(pTexture->ImgInfo.Height);

			(*iter)->vSize = D3DXVECTOR3(fStructCX, fStructCY, 0.f);

			float fCX = fStructCX * 0.5f;
			float fCY = fStructCY * 0.5f;

			D3DXMATRIX matWorld, matTrans, matRotZ;

			D3DXMatrixTranslation(&matTrans,
				(*iter)->vPos.x - m_pMainView->GetScrollPos(0),
				(*iter)->vPos.y /*- fCY*/ - m_pMainView->GetScrollPos(1),
				0.f);

			D3DXMatrixRotationZ(&matRotZ, (*iter)->fTransformValue);

			matWorld = matRotZ * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			// 선택됐을 때 알파 값 조정 위한 변수
			int iAlpha = 255;

			if(((*iter)->bIsClicked) == true) 
				iAlpha = int(255 * 0.5);
			else 
				iAlpha = 255;

			// draw
			CDevice::GetInstance()->GetSprite()->Draw(
				pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
				NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));
//		}
	}
// 
// 	for(; iter != iter_end; ++iter)
// 	{
// 		if( (*iter)->iRenderOption == 0) // 다음 순으로 0 world 렌더
// 		{
// 			TCHAR* szKey = new TCHAR[MAX_STR];
// 			TCHAR* szKeyPasted = L"STRUCT";
// 
// 			lstrcpy(szKeyPasted, szKey);
// 
// 			//		(*iter)->wstrName = L"Struct";
// 
// 			const TEX_INFO* pTexture
// 				= CTextureMgr::GetInstance()->GetTexture(
// 				szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);
// 
// 			float fStructCX = float(pTexture->ImgInfo.Width);
// 			float fStructCY = float(pTexture->ImgInfo.Height);
// 
// 			(*iter)->vSize = D3DXVECTOR3(fStructCX, fStructCY, 0.f);
// 
// 			float fCX = fStructCX * 0.5f;
// 			float fCY = fStructCY * 0.5f;
// 
// 			D3DXMATRIX matWorld, matTrans, matRotZ;
// 
// 			D3DXMatrixTranslation(&matTrans,
// 				(*iter)->vPos.x - m_pMainView->GetScrollPos(0),
// 				(*iter)->vPos.y /*- fCY*/ - m_pMainView->GetScrollPos(1),
// 				0.f);
// 
// 			D3DXMatrixRotationZ(&matRotZ, (*iter)->fAngle);
// 
// 			matWorld = matRotZ * matTrans;
// 
// 			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
// 
// 			// 선택됐을 때 알파 값 조정 위한 변수
// 			int iAlpha = 255;
// 
// 			if(((*iter)->bIsClicked) == true) 
// 				iAlpha = int(255 * 0.5);
// 			else 
// 				iAlpha = 255;
// 
// 			// draw
// 			CDevice::GetInstance()->GetSprite()->Draw(
// 				pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
// 				NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));
// 		}
// 
// 	}
// 
// 	for(; iter != iter_end; ++iter)
// 	{
// 		if( (*iter)->iRenderOption == 1) // 마지막 순으로 1 forward 렌더
// 		{
// 			TCHAR* szKey = new TCHAR[MAX_STR];
// 			TCHAR* szKeyPasted = L"STRUCT";
// 
// 			lstrcpy(szKeyPasted, szKey);
// 
// 			//		(*iter)->wstrName = L"Struct";
// 
// 			const TEX_INFO* pTexture
// 				= CTextureMgr::GetInstance()->GetTexture(
// 				szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);
// 
// 			float fStructCX = float(pTexture->ImgInfo.Width);
// 			float fStructCY = float(pTexture->ImgInfo.Height);
// 
// 			(*iter)->vSize = D3DXVECTOR3(fStructCX, fStructCY, 0.f);
// 
// 			float fCX = fStructCX * 0.5f;
// 			float fCY = fStructCY * 0.5f;
// 
// 			D3DXMATRIX matWorld, matTrans, matRotZ;
// 
// 			D3DXMatrixTranslation(&matTrans,
// 				(*iter)->vPos.x - m_pMainView->GetScrollPos(0),
// 				(*iter)->vPos.y /*- fCY*/ - m_pMainView->GetScrollPos(1),
// 				0.f);
// 
// 			D3DXMatrixRotationZ(&matRotZ, (*iter)->fAngle);
// 
// 			matWorld = matRotZ * matTrans;
// 
// 			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
// 
// 			// 선택됐을 때 알파 값 조정 위한 변수
// 			int iAlpha = 255;
// 
// 			if(((*iter)->bIsClicked) == true) 
// 				iAlpha = int(255 * 0.5);
// 			else 
// 				iAlpha = 255;
// 
// 			// draw
// 			CDevice::GetInstance()->GetSprite()->Draw(
// 				pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
// 				NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));
// 
// 		}
// 
// 
// 	}






}

void CMyObj::Release()
{
	list<PMYOBJ>::iterator iter = m_GroundList.begin();
	list<PMYOBJ>::iterator iter_end = m_GroundList.end();

	for(; iter != iter_end; )
	{
		Safe_Delete((*iter));
		iter = m_GroundList.erase(iter);
	}

	iter = m_StructureList.begin();
	iter_end = m_StructureList.end();

	for(; iter != iter_end; )
	{
		Safe_Delete((*iter));
		iter = m_StructureList.erase(iter);
	}
}

void CMyObj::SetObject(D3DXVECTOR3 vPos, int iObjID, 
					   int iRenderOption, int iCount, 
					   int* pStIndex, int* pGrIndex
					   , int iCollCount, bool bIsAgreeToDamage)
{
	PMYOBJ pMyObj = new MYOBJ;

	pMyObj->bIsClicked = false;
	pMyObj->fTransformValue = 0;
	pMyObj->iRenderOption = iRenderOption;
	pMyObj->iCount = iCount;
	pMyObj->iObjID = iObjID;
	pMyObj->bIsAdded = false;
	pMyObj->iGrIndex = 0;
	pMyObj->iStIndex = 0;
	pMyObj->iCollCount = 0;
	pMyObj->bIsInvincible = false;
	pMyObj->bIsDamaged = false;
	pMyObj->bIsMakingEffect = false;
	pMyObj->bIsTransform = false;

	pMyObj->bIsAgreeToDamage = bIsAgreeToDamage;
	

	// 포스값 발 밑으로 --> 얘는 사이즈가 다 달라서 나중에 드로우할때 설정해야될듯..
//	float fPosY = 0.f;
	pMyObj->vPos = D3DXVECTOR3(vPos.x, vPos.y, 0.f);


	switch(iObjID)
	{
	case 0:
		pMyObj->wstrName = L"Struct";
		pMyObj->iStIndex = (*pStIndex);
		m_StructureList.push_front(pMyObj);
		(*pStIndex) += 1;
		break;
	case 1:
		pMyObj->wstrName = L"Ground";
		pMyObj->iGrIndex = (*pGrIndex);
		m_GroundList.push_front(pMyObj);
		(*pGrIndex) += 1;
		break;
	}

}

void CMyObj::MiniRender()
{

	list<PMYOBJ>::iterator iter = m_StructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_StructureList.end();

	for(; iter != iter_end; ++iter)
	{
		TCHAR* szKey = new TCHAR[MAX_STR];
		TCHAR* szKeyPasted = L"STRUCT";

		lstrcpy(szKeyPasted, szKey);

		//		(*iter)->wstrName = L"Struct";

		const TEX_INFO* pTexture
			= CTextureMgr::GetInstance()->GetTexture(
			szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);

		float fStructCX = float(pTexture->ImgInfo.Width);
		float fStructCY = float(pTexture->ImgInfo.Height);

		(*iter)->vSize = D3DXVECTOR3(fStructCX, fStructCY, 0.f);

		float fCX = pTexture->ImgInfo.Width * 0.5f;
		float fCY = pTexture->ImgInfo.Height *0.5f;

		D3DXMATRIX matWorld, matTrans, matScale;

		D3DXMatrixTranslation(&matTrans,
			(*iter)->vPos.x * 0.25f,
			(*iter)->vPos.y * 0.25f,
			0.f);

		D3DXMatrixScaling(&matScale, 0.25f, 0.25f, 1.f);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		// 선택됐을 때 알파 값 조정 위한 변수
		int iAlpha = 255;

		if(((*iter)->bIsClicked) == true) 
			iAlpha = int(255 * 0.5);
		else 
			iAlpha = 255;

		// draw
		CDevice::GetInstance()->GetSprite()->Draw(
			pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
			NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));

	}

	// 그라운드 렌더
	iter = m_GroundList.begin();
	iter_end = m_GroundList.end();

	for(; iter != iter_end; ++iter)
	{
		TCHAR* szKey = new TCHAR[MAX_STR];
		TCHAR* szKeyPasted = L"GROUND";

		//		(*iter)->wstrName = L"Ground";

		lstrcpy(szKeyPasted, szKey);

		const TEX_INFO* pTexture
			= CTextureMgr::GetInstance()->GetTexture(
			szKeyPasted, (*iter)->wstrName.c_str(), (*iter)->iCount);

		float fGroundCX = float(pTexture->ImgInfo.Width);
		float fGroundCY = float(pTexture->ImgInfo.Height);

		(*iter)->vSize = D3DXVECTOR3(fGroundCX, fGroundCY, 0.f);

		float fCX = pTexture->ImgInfo.Width * 0.5f;
		float fCY = pTexture->ImgInfo.Height *0.5f;

		D3DXMATRIX matWorld, matTrans, matScale;

		D3DXMatrixTranslation(&matTrans,
			(*iter)->vPos.x * 0.25f,
			(*iter)->vPos.y * 0.25f,
			0.f);

		D3DXMatrixScaling(&matScale, 0.25f, 0.25f, 1.f);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		// 선택됐을 때 알파 값 조정 위한 변수
		int iAlpha = 255;

		if(((*iter)->bIsClicked) == true) 
			iAlpha = int(255 * 0.5);
		else 
			iAlpha = 255;

		// draw
		CDevice::GetInstance()->GetSprite()->Draw(
			pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
			NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));

	}
}

void CMyObj::TempRenderStruct()
{
// 	list<PMYOBJ>::iterator iter = m_StructureList.begin();
// 	list<PMYOBJ>::iterator iter_end = m_StructureList.end();
// 
// 	for(; iter != iter_end; ++iter)
// 	{
// 		TCHAR* szKey = new TCHAR[MAX_STR];
// 		TCHAR* szKeyPasted = L"STRUCT";
// 
// 		lstrcpy(szKeyPasted, szKey);


		const TEX_INFO* pTexture
			= CTextureMgr::GetInstance()->GetTexture(
			L"STRUCT", L"Struct", m_iTempCount);

		float fStructCX = float(pTexture->ImgInfo.Width);
		float fStructCY = float(pTexture->ImgInfo.Height);


		float fCX = fStructCX * 0.5f;
		float fCY = fStructCY * 0.5f;

		D3DXMATRIX matWorld, matTrans;

		m_vMousePos.x = GetMousePos().x + m_pMainView->GetScrollPos(0);
		m_vMousePos.y = GetMousePos().y + m_pMainView->GetScrollPos(1);

		D3DXMatrixTranslation(&matTrans,
			m_vMousePos.x - m_pMainView->GetScrollPos(0),
			m_vMousePos.y /*- fCY*/ - m_pMainView->GetScrollPos(1),
			0.f);


		matWorld = matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);



		// draw
		CDevice::GetInstance()->GetSprite()->Draw(
			pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
			NULL, D3DCOLOR_ARGB(123, 255, 255, 255));

}

void CMyObj::TempRenderGround()
{
	const TEX_INFO* pTexture
		= CTextureMgr::GetInstance()->GetTexture(
		L"GROUND", L"Ground", m_iTempCount);

	float fStructCX = float(pTexture->ImgInfo.Width);
	float fStructCY = float(pTexture->ImgInfo.Height);


	float fCX = fStructCX * 0.5f;
	float fCY = fStructCY * 0.5f;

	D3DXMATRIX matWorld, matTrans;

	m_vMousePos.x = GetMousePos().x + m_pMainView->GetScrollPos(0);
	m_vMousePos.y = GetMousePos().y + m_pMainView->GetScrollPos(1);

	D3DXMatrixTranslation(&matTrans,
		m_vMousePos.x - m_pMainView->GetScrollPos(0),
		m_vMousePos.y /*- fCY*/ - m_pMainView->GetScrollPos(1),
		0.f);


	matWorld = matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);



	// draw
	CDevice::GetInstance()->GetSprite()->Draw(
		pTexture->pTexture, NULL, &D3DXVECTOR3(fCX, fCY, 0.f),
		NULL, D3DCOLOR_ARGB(123, 255, 255, 255));

}
