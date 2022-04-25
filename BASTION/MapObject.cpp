#include "StdAfx.h"
#include "MapObject.h"
#include "Player.h"
#include "EffectBreak.h"
#include "Tile.h"


CMapObject::CMapObject(void)
:m_pPlayer(NULL), m_bIsSettingPlayer(false), m_bIsSettingPosY(false), m_fDamagedTime(0.f),
m_bIsSettingTile(false)
{
}

CMapObject::~CMapObject(void)
{
}

HRESULT CMapObject::Initialize( void )
{
	m_wstrObjKey = L"BackGround";
	m_wstrStateKey = L"Structure";

	m_fTrans = 0.f; 
	m_fTransSpeed = 20.f;

	if(m_tObjInfo.iRenderOption == 0) // world
		m_eRenderID = RENDER_OBJ;
	else if(m_tObjInfo.iRenderOption == 1) // forward
		m_eRenderID = RENDER_STRUCTURE_FORWARD;
	else if(m_tObjInfo.iRenderOption == 2)
		m_eRenderID = RENDER_STRUCTURE_BACKWARD;



	return S_OK;

}

eOBJECT_RESULT CMapObject::Update( void )
{
	if(m_bIsSettingPlayer == false)
	{
		m_pPlayer = m_pObjMgr->FindGameObject<CPlayer>();
		m_tInfo.vPos.x = m_tObjInfo.vPos.x;
		m_tInfo.vPos.y = m_tObjInfo.vPos.y;
		m_bIsSettingPlayer = true;
	}

	if(m_bIsSettingTile == false)
	{
		m_pVecTile = m_pObjMgr->FindGameObject<CTile>()->GetTile();
		m_bIsSettingTile = true;
	}



	// 화면 내에 존재하는 오브젝트만 연산한다.
	if(m_bIsInWindow == true)
	{
		CalcDistanceToPlayer();
	}

	MakingEffect();
	MatrixCheck();

	if(m_bIsDead == true)
	{
		m_pObjMgr->AddObject( 
			CreateEffectBreak<CEffectBreak>(DIR_RB, m_tInfo.vPos), 
			OBJ_EFFECT);

		CalcDistanceToTileForOption();

		return OR_DELETE;
	}

	return OR_OK;
}

void CMapObject::Render( void )
{
	const TEX_INFO* pTexture = m_pTextureMgr->GetTexture(
		m_wstrObjKey, m_wstrStateKey, m_tObjInfo.iCount);

	float fPosX = float(pTexture->ImgInfo.Width * 0.5f);
	float fPosY = float(pTexture->ImgInfo.Height * 0.5f);


	m_fHeight = fPosX;


	SettingCollRect( static_cast<float>(pTexture->ImgInfo.Height), 
		static_cast<float>(pTexture->ImgInfo.Width) );

	if(m_bIsSettingPosY == false)
	{	
		m_tInfo.vPos.y = m_tObjInfo.vPos.y + (pTexture->ImgInfo.Height * 0.2f);
		m_bIsSettingPosY = true;
	}

	m_pSprite->SetTransform( &(m_tObjInfo.matWorld) );

	if( m_tObjInfo.bIsDamaged == false)
	{
		m_pSprite->Draw( pTexture->pTexture,
			NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if( m_tObjInfo.bIsDamaged == true)
	{
		m_pSprite->Draw( pTexture->pTexture,
			NULL, &D3DXVECTOR3(fPosX, fPosY, 0.f), NULL, 
			D3DCOLOR_ARGB(255, 255, 0, 0));
	}

}

void CMapObject::Release( void )
{

}

void CMapObject::MatrixCheck( )
{
	D3DXMATRIX matTrans, matScale;


	if(m_tObjInfo.bIsTransform == true)
	{
		if( m_tObjInfo.vPos.y - (-m_vScroll.y)
		> (m_tObjInfo.matWorld._42) )
		{
			m_tObjInfo.fTransformValue -= m_fTransSpeed;
		}

		D3DXMatrixTranslation(&matTrans,
			m_tObjInfo.vPos.x + m_vScroll.x,
			m_tObjInfo.vPos.y + m_vScroll.y - (WINCY * 2.f) - m_tObjInfo.fTransformValue,
			m_tObjInfo.vPos.z);


	}
	else
	{
		D3DXMatrixTranslation(&matTrans,
			m_tObjInfo.vPos.x + m_vScroll.x,
			m_tObjInfo.vPos.y + m_vScroll.y - (WINCY * 2.f) - m_tObjInfo.fTransformValue,
			m_tObjInfo.vPos.z);
	}

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);


	m_tObjInfo.matWorld = matScale * matTrans;

}

void CMapObject::SettingCollRect(float fHeight, float fWidth)
{

	m_tObjInfo.tCollRect.top = static_cast<LONG>( m_tObjInfo.vPos.y - fHeight * 0.5f );
	m_tObjInfo.tCollRect.bottom = static_cast<LONG>( m_tObjInfo.vPos.y + fHeight * 0.5f );
	m_tObjInfo.tCollRect.left = static_cast<LONG>( m_tObjInfo.vPos.x - fWidth * 0.5f );
	m_tObjInfo.tCollRect.right = static_cast<LONG>( m_tObjInfo.vPos.x + fWidth * 0.5f );

}

void CMapObject::CalcDistanceToPlayer()
{
	float fDist = 380.f;

	D3DXVECTOR3 vDist = m_tObjInfo.vPos - m_pPlayer->GetInfo()->vPos;

	float fVecLength = D3DXVec3Length(&vDist);

// 	if( dynamic_cast<CPlayer*>(m_pPlayer)->GetState() != PLAYER_LIE )
// 	{
		if ( fVecLength <= fDist )
		{
			m_tObjInfo.bIsTransform = true;
		}
/*	}*/

}

void CMapObject::MakingEffect()
{

	if(m_tObjInfo.bIsDamaged == true) // 데미지를 입었다면
	{
		m_fDamagedTime += m_pTimeMgr->GetTime(); // 데미지 타임 카운트 시작

		if(m_tObjInfo.bIsMakingEffect == false) // 최초에 한번만 생성되도록
		{
			m_pObjMgr->AddObject( 
				CreateEffectBreak<CEffectBreak>(DIR_RB, m_tInfo.vPos), 
				OBJ_EFFECT);
			m_tObjInfo.bIsMakingEffect = true;
		}

		if(m_fDamagedTime >= 1.f) // 일정시간 지난 후
		{
			m_tObjInfo.bIsInvincible = false;
			m_tObjInfo.bIsDamaged = false;
			m_tObjInfo.bIsMakingEffect = false;
		}
	}
}

void CMapObject::CalcDistanceToTileForOption()
{
	float fTileX2 = 0.f;
	float fTileY2 = 0.f;

	if(g_eScene == SCENE_STAGE1)
	{
		fTileX2 = TILEX;
		fTileY2 = TILEY;
	}
	else if(g_eScene == SCENE_DIA)
	{
		fTileX2 = TILEXDIA;
		fTileY2 = TILEYDIA;
	}
	else if(g_eScene == SCENE_STAGE2)
	{
		fTileX2 = TILEX2;
		fTileY2 = TILEY2;
	}

	// 화면 내에 존재하는 타일들과 거리를 계산한다.

	float fShorterDist = 1000000.f;
	int	  iNearIndex = 0;

	float fTileX = WINCX / TILECX + 2;
	float fTileY = static_cast<long>(WINCY / (TILECY * 0.5f)) + 3;

	for(int i = 0; i < fTileX; ++i)
	{
		for(int j = 0; j < fTileY; ++j)
		{
			int iCullX = int(-m_vScroll.x / TILECX);
			int iCullY = int(-m_vScroll.y / (TILECY * 0.5f));

			int iIndex = static_cast<int>( (i + iCullY) * fTileX2 + (j + iCullX) );

			if(iIndex < 0 || iIndex >= fTileX2 * fTileY2) 
				continue;

			if(((*m_pVecTile)[iIndex])->byOption == 1)
				 continue;


			D3DXVECTOR3 vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + m_fHeight * 0.5f, 0.f);
			D3DXVECTOR3 vDist = vPos - ((*m_pVecTile)[iIndex])->vPos;

			float fVecLength = D3DXVec3Length(&vDist);

			if(fVecLength <= fShorterDist)
			{
				fShorterDist = fVecLength;
				iNearIndex = iIndex;
			}
		}
	}

	((*m_pVecTile)[iNearIndex])->byOption = 1; // 이동 가능 옵션으로 바꿈

}
