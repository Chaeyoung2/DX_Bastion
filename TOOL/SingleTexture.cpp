#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture(void)
:m_pTexInfo(NULL)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pPath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEX_INFO;
	ZeroMemory(m_pTexInfo, sizeof(TEX_INFO));

	if(FAILED(D3DXGetImageInfoFromFile(pPath, &m_pTexInfo->ImgInfo)))
	{
		MessageBox(g_hWnd, pPath, L"Texture Info Error!", MB_OK);
		return E_FAIL;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(),
		pPath,		//경로
		m_pTexInfo->ImgInfo.Width,		//이미지 가로
		m_pTexInfo->ImgInfo.Height,		//이미지 세로
		m_pTexInfo->ImgInfo.MipLevels,	//3D에서 사용(이미지를 축소, 확대) -> MFC X
		0,	//이미지를 읽는 방식 설정
		m_pTexInfo->ImgInfo.Format,		//이미지 포멧
		D3DPOOL_MANAGED,
		//현재 읽는 이미지를 어떤메모리에 보관할지를 설정하는 옵션
		//D3DPOOL_MANAGED : 그래픽 카드(시스템 메모리를 추가적으로 사용하는 옵션)
		//D3DPOOL_DEFAULT : 그래픽 카드 꽉차면 시스템 출력 불가능.
		D3DX_DEFAULT,		//이미지의 테두리를 어떻게 표현할지
		D3DX_DEFAULT,		//확대/축소할때 어떻게 처리할것인지?
		NULL,			//제거할 색상
		&m_pTexInfo->ImgInfo,
		NULL,		//8비트 미만의 이미지에 관한 처리
		&m_pTexInfo->pTexture
		)) )
	{
		MessageBox(g_hWnd, pPath, L"Single Texture Load Error!", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

const TEX_INFO* CSingleTexture::GetTexture(const TCHAR* pKey /*= L""*/, const int& iCount /*= 0*/)
{
	return m_pTexInfo;
}

void CSingleTexture::Release()
{
	if(m_pTexInfo != NULL)
	{
		m_pTexInfo->pTexture->Release();
		Safe_Delete(m_pTexInfo);
	}
}
