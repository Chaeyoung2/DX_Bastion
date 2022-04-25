#include "StdAfx.h"
#include "Structure.h"
#include "TextureMgr.h"
//#include "Device.h"
#include "ObjMgr.h"
#include "EffectBreak.h"
#include "Player.h"

CStructure::CStructure(void)
: m_pPlayer(NULL), m_bIsSettedPlayer(false), m_bIsCompareWithPlayer(false)
{
}

CStructure::~CStructure(void)
{
}

HRESULT CStructure::Initialize(void)
{
	LoadData();

//	m_pSprite = CDevice::GetInstance()->GetSprite();

	m_wstrObjKey = L"BackGround";
	m_wstrStateKey = L"Structure";

	m_fTrans = 0.f; 
	m_fTransSpeed = 20.f;

	m_eRenderID = RENDER_END;

	

	return S_OK;
}

eOBJECT_RESULT CStructure::Update(void)
{
	if(m_bIsSettedPlayer == false)
	{
		m_pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>();
		m_bIsSettedPlayer = true;
	}

// 	if(m_bIsCompareWithPlayer == false)
// 	{
// 		IsHigherthanPlayer();
// 
// 		m_bIsCompareWithPlayer = true;
// 	}

//	IsHigherthanPlayer();

	MakingEffect();
	CalcDistanceToPlayer();
	
	return OR_OK;
}

void CStructure::Render(void)
{
	list<PMYOBJ>::iterator iter = m_StructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_StructureList.end();


	m_StructureList.sort( CompareY_st<PMYOBJ> );

	for(; iter != iter_end; ++iter)
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey, m_wstrStateKey, (*iter)->iCount);

		float fPosX = float(pTexture->ImgInfo.Width * 0.5f);
		float fPosY = float(pTexture->ImgInfo.Height * 0.5f);

		SettingCollRect( iter, static_cast<float>(pTexture->ImgInfo.Height), 
			static_cast<float>(pTexture->ImgInfo.Width) );

		MatrixCheck(iter);

		m_pSprite->SetTransform( &((*iter)->matWorld) );

		if( (*iter)->bIsDamaged == false)
		{
			m_pSprite->Draw( pTexture->pTexture,
				NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if( (*iter)->bIsDamaged == true)
		{
			m_pSprite->Draw( pTexture->pTexture,
				NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
				D3DCOLOR_ARGB(255, 255, 0, 0));
		}
	}
}
void CStructure::ForwardRender(void)
{
	list<PMYOBJ>::iterator iter = m_ForwardStructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_ForwardStructureList.end();

	m_ForwardStructureList.sort( CompareY_st<PMYOBJ> );

	for(; iter != iter_end; ++iter)
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey, m_wstrStateKey, (*iter)->iCount);

		float fPosX = float(pTexture->ImgInfo.Width * 0.5f);
		float fPosY = float(pTexture->ImgInfo.Height * 0.5f);

		SettingCollRect( iter, static_cast<float>(pTexture->ImgInfo.Height), 
			static_cast<float>(pTexture->ImgInfo.Width) );

		MatrixCheck(iter);

		m_pSprite->SetTransform( &((*iter)->matWorld) );

		if( (*iter)->bIsDamaged == false)
		{
			m_pSprite->Draw( pTexture->pTexture,
				NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if( (*iter)->bIsDamaged == true)
		{
			m_pSprite->Draw( pTexture->pTexture,
				NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
				D3DCOLOR_ARGB(255, 255, 0, 0));
		}
	}
}
void CStructure::BackwardRender(void)
{
	list<PMYOBJ>::iterator iter = m_BackwardStructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_BackwardStructureList.end();

	m_BackwardStructureList.sort( CompareY_st<PMYOBJ> );

	for(; iter != iter_end; ++iter)
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture(
			m_wstrObjKey, m_wstrStateKey, (*iter)->iCount);

		float fPosX = float(pTexture->ImgInfo.Width * 0.5f);
		float fPosY = float(pTexture->ImgInfo.Height * 0.5f);

		SettingCollRect( iter, static_cast<float>(pTexture->ImgInfo.Height), 
			static_cast<float>(pTexture->ImgInfo.Width) );

		MatrixCheck(iter);

		m_pSprite->SetTransform( &((*iter)->matWorld) );

		if( (*iter)->bIsDamaged == false)
		{
			m_pSprite->Draw( pTexture->pTexture,
				NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if( (*iter)->bIsDamaged == true)
		{
			m_pSprite->Draw( pTexture->pTexture,
				NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
				D3DCOLOR_ARGB(255, 255, 0, 0));
		}
	}
}

void CStructure::Release(void)
{
	list<PMYOBJ>::iterator iter = m_StructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_StructureList.end();

	for(; iter != iter_end;)
	{
		Safe_Delete((*iter));
		iter = m_StructureList.erase(iter);
	}
}

void CStructure::MatrixCheck(list<PMYOBJ>::iterator iter)
{
	D3DXMATRIX matTrans, matScale;
// 
// 	list<PMYOBJ>::iterator iter = m_StructureList.begin();
// 	list<PMYOBJ>::iterator iter_end = m_StructureList.end();

// 	for(; iter != iter_end; ++iter)
// 	{
	if((*iter)->bIsTransform == true)
	{
		if( (*iter)->vPos.y - (-m_vScroll.y)
		> ((*iter)->matWorld._42) )
		{
			(*iter)->fTransformValue -= m_fTransSpeed;
		}

		D3DXMatrixTranslation(&matTrans,
			(*iter)->vPos.x + m_vScroll.x,
			(*iter)->vPos.y + m_vScroll.y - WINCY - (*iter)->fTransformValue,
			(*iter)->vPos.z);

		
	}
	else
	{
		D3DXMatrixTranslation(&matTrans,
			(*iter)->vPos.x + m_vScroll.x,
			(*iter)->vPos.y + m_vScroll.y - WINCY - (*iter)->fTransformValue,
			(*iter)->vPos.z);
	}

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);


		(*iter)->matWorld = matScale * matTrans;
//	}
}

void CStructure::LoadData()
{

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

		m_StructureList.push_back(pObj);
	}

	CloseHandle(hFile);


}

void CStructure::SettingCollRect(list<PMYOBJ>::iterator iter, float fHeight, float fWidth)
{
	(*iter)->tCollRect.top = static_cast<LONG>( (*iter)->vPos.y - fHeight * 0.5f );
	(*iter)->tCollRect.bottom = static_cast<LONG>( (*iter)->vPos.y + fHeight * 0.5f );
	(*iter)->tCollRect.left = static_cast<LONG>( (*iter)->vPos.x - fWidth * 0.5f );
	(*iter)->tCollRect.right = static_cast<LONG>( (*iter)->vPos.x + fWidth * 0.5f );
}

void CStructure::MakingEffect()
{

	list<PMYOBJ>::iterator iter = m_StructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_StructureList.end();

	for(; iter != iter_end;)
	{
		bool bIsMakingEffect = (*iter)->bIsMakingEffect;
		bool bIsDamaged = (*iter)->bIsDamaged;

		if(bIsDamaged == true)
		{
// 			if(bIsMakingEffect == false)
// 			{
				CObjMgr::GetInstance()->AddObject( 
					CreateEffectBreak<CEffectBreak>(DIR_RB, (*iter)->vPos), 
					OBJ_EFFECT);
				//bIsMakingEffect = true;
				Safe_Delete(*iter);
				iter = m_StructureList.erase(iter);
/*			}*/
		}
		else
			++iter;
	}
}

void CStructure::CalcDistanceToPlayer()
{

	float fDist = 450.f;

	list<PMYOBJ>::iterator iter = m_StructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_StructureList.end();

	for(; iter != iter_end; ++iter)
	{
		D3DXVECTOR3 vDist = (*iter)->vPos - m_pPlayer->GetInfo()->vPos;

		float fVecLength = D3DXVec3Length(&vDist);

		if(dynamic_cast<CPlayer*>(m_pPlayer)->GetState() != PLAYER_LIE)
		{
			if ( fVecLength <= fDist )
			{
				(*iter)->bIsTransform = true;
			}
		}

	}

}

// ** 0913
void CStructure::IsHigherthanPlayer()
{
	// Release() 해주고
	// Forward
	list<PMYOBJ>::iterator iter = m_ForwardStructureList.begin();
	list<PMYOBJ>::iterator iter_end = m_ForwardStructureList.end();

	for(; iter != iter_end;)
	{
		Safe_Delete(*iter);
		iter = m_ForwardStructureList.erase(iter);
	}

	// Backward
	iter = m_BackwardStructureList.begin();
	iter_end = m_BackwardStructureList.end();

	for(; iter != iter_end;)
	{
		Safe_Delete(*iter);
		iter = m_BackwardStructureList.erase(iter);
	}

	///////////////////////////////////////////////////////////////////////////////////


	// 다시 설정
	float fPlayerY = m_pPlayer->GetInfo()->vPos.y;


	iter = m_StructureList.begin();
	iter_end = m_StructureList.end();

	for(; iter != iter_end; ++iter)
	{
		float fStructureY = (*iter)->vPos.y;

		if( fPlayerY < fStructureY ) // 플레이어가 위에 있으면 스트럭쳐는 앞에
		{
			m_ForwardStructureList.push_back( (*iter) );
		}
		else // 플레이어가 아래에 있으면 스트럭쳐는 뒤에
		{
			m_BackwardStructureList.push_back( (*iter) );
		}
	}
}