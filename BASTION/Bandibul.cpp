#include "StdAfx.h"
#include "Bandibul.h"

CBandibul::CBandibul(void)
:m_fTime(0.f)
{
}

CBandibul::~CBandibul(void)
{
}

HRESULT CBandibul::Initialize(void)
{

	m_eRenderID = RENDER_SCREEN;

	m_tFrame = FRAME(0.f, 0.f, 2.f, 2.f);


	m_tInfo.vDir  = D3DXVECTOR3(0.f, -1.f, 0.f);
	m_tInfo.vLook = D3DXVECTOR3(0.f, -1.f, 0.f);
	m_tInfo.vSize = D3DXVECTOR3(50.f, 50.f, 0.f);

	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"Bandibul";

/*	m_fRotSpeed = 3.f;*/

//	m_fAngle = 45.f;
/*	m_fRotAngle = -90.f;*/


/*	m_fDistance = 10.f;*/


	return S_OK;
}

eOBJECT_RESULT CBandibul::Update(void)
{
	// 오브젝트 공통
	FrameCheck();
	MatrixCheck();


	if(m_tInfo.vPos.y < 0)
		m_bIsDead = true;

	if(m_bIsDead)
	{
		return OR_DELETE;
	}

	return OR_OK;
}

void CBandibul::Render(void)
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
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);

	
}

void CBandibul::Release(void)
{

}

void CBandibul::FrameCheck(void)
{

}

void CBandibul::MatrixCheck(void)
{
// 	if(m_bIsSettingCenter == false)
// 	{
// 		m_fCenterX = m_tInfo.vPos.x;
// 		m_fCenterY = m_tInfo.vPos.y;
// 		m_bIsSettingCenter = true;
// 	}
// 	
// 	system("cls");
// 	cout << "m_tInfo.vPos.x: " << m_tInfo.vPos.x << endl;
// 	cout << "m_tInfo.vPos.y: " << m_tInfo.vPos.y << endl;
	//

	m_fTime += m_pTimeMgr->GetTime();
// 	float fTime = GET_TIME;
// 
	if( m_fTime >= 0.03f)
	{
		m_tInfo.vPos.x = cosf( m_fAngle) * m_fDistance;

		m_tInfo.vPos.y -= m_fSpeed;

		D3DXMATRIX matTrans;

		D3DXMatrixTranslation(&matTrans, 
			m_tInfo.vPos.x /*+ m_vScroll.x*/,
			m_tInfo.vPos.y /*+ m_vScroll.y*/,
			m_tInfo.vPos.z
			);

		m_tInfo.matWorld = matTrans;

		m_fAngle += 0.01f;

		m_fTime = 0.f;
	}



}
