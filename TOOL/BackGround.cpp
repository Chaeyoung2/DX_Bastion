#include "StdAfx.h"
#include "BackGround.h"

#include "TextureMgr.h"
#include "Device.h"

#include "TOOLView.h"




CBackGround::CBackGround(void)
:m_bRenderBor(true), m_bRenderIndex(true), m_bRenderOp(true), m_iStage(0)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}

HRESULT CBackGround::Initialize()
{
	// 디아 술집
// 	float fTileX = TILEXDIA;
// 	float fTileY = TILEYDIA;
	
	// 스테이지2
	float fTileX = TILEX2;
	float fTileY = TILEY2;

	for(int i = 0; i < fTileY; ++i)
	{
		for(int j = 0; j < fTileX; ++j)
		{
			PTILE pTile = new TILE;

			float fStartY = 10.f;
			float fStartX = 10.f;
			float fX = fStartX + j * TILECX + (i % 2) * TILECX / 2.f;
			float fY = fStartY + i * TILECY * 0.5f;

			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3(float(TILECX), float(TILECY), 0.f);
			pTile->bIsTransform = true;
			D3DXMatrixIdentity(&(pTile->matWorld));
			pTile->vOriPos = pTile->vPos;
			pTile->byOption = 0;
			pTile->byDrawId = 123;
			pTile->fMoveSpeed = 5;

			m_vecTile.push_back(pTile);
		}
	}



	return S_OK;
}

int CBackGround::Update()
{
	return 0;
}

void CBackGround::Render()
{
	 //맵 렌더
//  	const TEX_INFO* pMapTexture
//  		= CTextureMgr::GetInstance()->GetTexture(L"BACKGROUND", L"BackGround", 0);
//  	float fMapX = pMapTexture->ImgInfo.Height * 0.5f;
//  	float fMapY = pMapTexture->ImgInfo.Width * 0.5f;
//  
//  
//  	D3DXMATRIX matMapTrans;
//  
//  	D3DXMatrixTranslation(&matMapTrans, fMapX /* - m_pMainView->GetScrollPos(0)*/, 
//  		fMapY /*- m_pMainView->GetScrollPos(1)*/, 0.f);
//  
//  	CDevice::GetInstance()->GetSprite()->SetTransform(&matMapTrans);
//  
//  
//  	if(m_iStage != 2)
//  	{
//  		CDevice::GetInstance()->GetSprite()->Draw(
//  			pMapTexture->pTexture,
//  			NULL,
//  			&D3DXVECTOR3(fMapX, fMapY, 0.f),
//  			NULL,
//  			D3DCOLOR_ARGB(255, 255, 255, 255));
//  
//  	}
	

	// 디아 술집
// 	float fTileX = TILEXDIA;
// 	float fTileY = TILEYDIA;

	// 스테이지2
	float fTileX = TILEX2;
	float fTileY = TILEY2;


	// 타일
	D3DXMATRIX matTrans;

	TCHAR szText[MIDDLE_STR] = L"";

	for(int i = 0; i < fTileY; ++i)
	{
		for(int j = 0; j < fTileX; ++j)
		{
			int iIndex = int(i * fTileX + j);

			if(iIndex < 0 || iIndex >= fTileX * fTileY)
				break;

			// 타일 텍스쳐
			const TEX_INFO* pTileTexture 
				= CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", m_vecTile[iIndex]->byDrawId);

			// 옵션 텍스쳐
			const TEX_INFO* pOptionTexture
				= CTextureMgr::GetInstance()->GetTexture(L"TILEOPTION", L"TileOption", m_vecTile[iIndex]->byOption);
			
			// 윤곽선 텍스쳐
			const TEX_INFO* pBorderTexture
				= CTextureMgr::GetInstance()->GetTexture(L"TILEBORDER", L"TileBorder", 0);

			D3DXMatrixTranslation(&matTrans, 
				m_vecTile[iIndex]->vPos.x - m_pMainView->GetScrollPos(0),	//0 : 스크롤 가로
				m_vecTile[iIndex]->vPos.y - m_pMainView->GetScrollPos(1),	//1 : 스크롤 세로
				m_vecTile[iIndex]->vPos.z);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			// 윤곽선 텍스쳐의 중점
			float fBorPosX = pBorderTexture->ImgInfo.Width * 0.5f;
			float fBorPosY = pBorderTexture->ImgInfo.Height * 0.5f;
			// 옵션 텍스쳐의 중점
			float fOpPosX = pOptionTexture->ImgInfo.Width * 0.5f;
			float fOpPosY = pOptionTexture->ImgInfo.Height * 0.5f;
			// 타일 텍스쳐의 중점
			float fPosX = pTileTexture->ImgInfo.Width * 0.5f;
			float fPosY = pTileTexture->ImgInfo.Height * 0.5f;


			// 타일 draw
			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(fPosX, fPosY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			// 윤곽선 draw
			if(m_bRenderBor == true)
			{
				CDevice::GetInstance()->GetSprite()->Draw(
					pBorderTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fBorPosX, fBorPosY, 0.f),
					NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// 옵션 draw
			if(m_bRenderOp == true)
			{
				CDevice::GetInstance()->GetSprite()->Draw(
				pOptionTexture->pTexture,
				NULL,
				&D3DXVECTOR3(fOpPosX, fOpPosY, 0.f),
				NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}


			//폰트출력
			RECT rcText;
			float fGap = -10.f;
			rcText.left = (LONG)fGap;
			rcText.top = (LONG)fGap;
			rcText.right = LONG(fPosX * 2.f + fGap);
			rcText.bottom =  LONG(fPosY * 2.f + fGap);

			wsprintf(szText, L"%d", iIndex);

			if(m_bRenderIndex == true)
			{
				CDevice::GetInstance()->GetFont()->DrawTextW(
					CDevice::GetInstance()->GetSprite(),
					szText,
					lstrlen(szText),
					&rcText,
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255)
					);		
			}
		}
	}

}


void CBackGround::Release()
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		Safe_Delete(m_vecTile[i]);
	}
	m_vecTile.clear();
}

int CBackGround::GetTileIndex(const D3DXVECTOR3& vPos)
{
	//실제 피킹
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if(Picking(vPos, i))		//피킹이 되었다면.!
		{
			return i;
		}
	}

	return -1;
}

void CBackGround::TileChange(const D3DXVECTOR3& vPos, const int& iDrawID, const int& iOption)
{
	int iIndex = GetTileIndex(vPos);		//피킹이 완료된 타일의 Index

	if(iIndex == -1)
		return;

	m_vecTile[iIndex]->byDrawId = iDrawID;
	m_vecTile[iIndex]->byOption = iOption;			//포탈, 갈수있는, 갈수없는.....

	// 뭐지 이상한데ㅠㅠ
// #ifdef _DEBUG
// 	system("cls");
// 	cout << "마우스 vPos.x : " << vPos.x << endl;
// 	cout << "마우스 vPos.y : " << vPos.y << endl;
// 	cout << "피킹이 된 iIndex : " << iIndex << endl;
// 	cout << "m_vecTile[iIndex]->vPos.x : " << m_vecTile[iIndex]->vPos.x << endl;
// 	cout << "m_vecTile[iIndex]->vPos.y : " << m_vecTile[iIndex]->vPos.y << endl;
// #endif
}

void CBackGround::TileSetting(const D3DXVECTOR3& vPos, const float& fHeight, const float& fMoveSpeed)
{
	int iIndex = GetTileIndex(vPos);		//피킹이 완료된 타일의 Index

	if(iIndex == -1)
		return;

	D3DXVECTOR3 vTempPos = m_vecTile[iIndex]->vPos;
	vTempPos.y -= fHeight;
	m_vecTile[iIndex]->vPos = vTempPos;
	m_vecTile[iIndex]->vOriPos = vTempPos;


	m_vecTile[iIndex]->fMoveSpeed = fMoveSpeed;
}


bool CBackGround::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{

	//꼭지점 만들자.
	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, 
		m_vecTile[iIndex]->vPos.y - TILECY / 2.f, 
		0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX / 2.f, 
		m_vecTile[iIndex]->vPos.y, 
		0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, 
		m_vecTile[iIndex]->vPos.y + TILECY / 2.f, 
		0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX / 2.f, 
		m_vecTile[iIndex]->vPos.y, 
		0.f)
	};

	//방향벡터를 만들어 보자.
	D3DXVECTOR3 vDir[4] = 
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들자
	//어느한벡터에 직각이 되는 벡터를 의미한다.
	D3DXVECTOR3 vNormal[4] = 
	{
		D3DXVECTOR3(vDir[0].y, -vDir[0].x, 0.f),
		D3DXVECTOR3(vDir[1].y, -vDir[1].x, 0.f),
		D3DXVECTOR3(vDir[2].y, -vDir[2].x, 0.f),
		D3DXVECTOR3(vDir[3].y, -vDir[3].x, 0.f)
	};


	//법선벡터를 단위벡터로 만들어 주자.
	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);		//단위벡터로 만들어주는 함수.


	//내적을 하자.
	//내적의 결과값이 양수면 타일외부에 있다.
	//				  음수면 타일내부에 있다.

	for(int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vDirTemp = vPos - vPoint[i];
		D3DXVec3Normalize(&vDirTemp, &vDirTemp);

		float fDotResult = D3DXVec3Dot(&vDirTemp, &vNormal[i]);

		if(fDotResult > 0.f)
			return false;
	}

	return true;
}

void CBackGround::MiniMapRender()
{
	// 디아술집
// 	float fTileX = TILEXDIA;
// 	float fTileY = TILEYDIA;

	// 스테이지2
	float fTileX = TILEX2;
	float fTileY = TILEY2;

	// 맵 렌더
	const TEX_INFO* pMapTexture
		= CTextureMgr::GetInstance()->GetTexture(L"BACKGROUND", L"BackGround", 0);

	float fMapX = pMapTexture->ImgInfo.Height * 0.5f;
	float fMapY = pMapTexture->ImgInfo.Width * 0.5f;


	D3DXMATRIX matMapTrans, matMapScale, matMapWorld;

	D3DXMatrixTranslation(&matMapTrans, fMapX * 0.43f, 
		fMapY * 0.43f, 0.f);

	D3DXMatrixScaling(&matMapScale, 0.43f, 0.43f, 1.f);

	matMapWorld = matMapScale * matMapTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matMapWorld);


	CDevice::GetInstance()->GetSprite()->Draw(
		pMapTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fMapX, fMapY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	// 타일

	D3DXMATRIX matTrans, matScale, matWorld;

	TCHAR szText[MIDDLE_STR] = L"";

	for(int i = 0; i < fTileY; ++i)
	{
		for(int j = 0; j < fTileX; ++j)
		{
			int iIndex = int(i * fTileX + j);

			if(iIndex < 0 || iIndex >= fTileX * fTileY)
				break;

			const TEX_INFO* pTileTexture 
				= CTextureMgr::GetInstance()->GetTexture(
				L"Back", L"Tile", m_vecTile[iIndex]->byDrawId);

			D3DXMatrixScaling(&matScale, 0.25f, 0.25f, 1.f);

			D3DXMatrixTranslation(&matTrans, 
				m_vecTile[iIndex]->vPos.x * 0.25f,
				m_vecTile[iIndex]->vPos.y * 0.25f,
				m_vecTile[iIndex]->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTexture->pTexture,
				NULL,
				&D3DXVECTOR3(TILECX/2.f, TILECY/2.f, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

