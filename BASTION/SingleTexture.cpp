#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture(void)
:m_pTEX_INFO(NULL)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}
HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	m_pTEX_INFO = new TEX_INFO;
	ZeroMemory(m_pTEX_INFO, sizeof(TEX_INFO));

	if(FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTEX_INFO->ImgInfo)))
	{
		MSG_BOX(wstrFilePath.c_str());
		return E_FAIL;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice->GetDevice(), 
		wstrFilePath.c_str(), 
		m_pTEX_INFO->ImgInfo.Width, 
		m_pTEX_INFO->ImgInfo.Height, 
		m_pTEX_INFO->ImgInfo.MipLevels,
		0, 
		m_pTEX_INFO->ImgInfo.Format,
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT,		// 테두리 표현을 어떻게 할 것인가
		D3DX_DEFAULT,		// 확대, 축소 했을 경우
		NULL,				// 제거할 이미지 색상 값
		&m_pTEX_INFO->ImgInfo,
		NULL,				// 팔레트(8비트 미만의 이미지) 처리
		&m_pTEX_INFO->pTexture)))
	{
		MSG_BOX(L"Single Texture Image Load Failed");
		return E_FAIL;
	}		


	return S_OK;
}


const TEX_INFO* CSingleTexture::GetTexture(const wstring& wstrStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	return m_pTEX_INFO;
}

void CSingleTexture::Release()
{
	m_pTEX_INFO->pTexture->Release();

	::Safe_Delete(m_pTEX_INFO);
}
