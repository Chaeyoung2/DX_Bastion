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
		pPath,		//���
		m_pTexInfo->ImgInfo.Width,		//�̹��� ����
		m_pTexInfo->ImgInfo.Height,		//�̹��� ����
		m_pTexInfo->ImgInfo.MipLevels,	//3D���� ���(�̹����� ���, Ȯ��) -> MFC X
		0,	//�̹����� �д� ��� ����
		m_pTexInfo->ImgInfo.Format,		//�̹��� ����
		D3DPOOL_MANAGED,
		//���� �д� �̹����� ��޸𸮿� ���������� �����ϴ� �ɼ�
		//D3DPOOL_MANAGED : �׷��� ī��(�ý��� �޸𸮸� �߰������� ����ϴ� �ɼ�)
		//D3DPOOL_DEFAULT : �׷��� ī�� ������ �ý��� ��� �Ұ���.
		D3DX_DEFAULT,		//�̹����� �׵θ��� ��� ǥ������
		D3DX_DEFAULT,		//Ȯ��/����Ҷ� ��� ó���Ұ�����?
		NULL,			//������ ����
		&m_pTexInfo->ImgInfo,
		NULL,		//8��Ʈ �̸��� �̹����� ���� ó��
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
