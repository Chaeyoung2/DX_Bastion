#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)


HRESULT CDevice::Init3D(void)
{
	m_pD3D = NULL;
	m_pD3dDevice = NULL;

	//D3D��ü�� ����. -> D3dDevice�� ����ϱ� ���ؼ�.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//��ġ�� ���� ����. -> �����ϴ� ���� ����.
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		&DeviceCaps) ) )
	{
		//�ʱ�ȭ�� �����Ѵٸ� �����!
		AfxMessageBox(L"��ġ �ʱ�ȭ ����!");
		//ERROR_TEXT(L"��ġ �ʱ�ȭ ����!");
		return E_FAIL;
	}

	DWORD vp;

	//�ҷ����� ������ ����
	//���� ���� ���� ������ �ϵ��� ���� ����Ʈ��� ������ �Ǵ��ϰ� �ȴ�.
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		//�׷��� ī�带 ������ �Ѵٸ�
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//Cpu
		//�׷��� ī�带 �������� �ʴ´ٸ�
	}

	//��ġ�� �����Ҷ� �����ڰ� ����������ϴ� ��ġ ����
	D3DPRESENT_PARAMETERS d3dp;
	ZeroMemory(&d3dp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dp);		//��ġ ������ ����!

	if(FAILED(m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		vp,
		&d3dp,
		&m_pD3dDevice)) )
	{
		AfxMessageBox(L"����̽� �ʱ�ȭ ����!");
		//ERROR_TEXT(L"����̽� �ʱ�ȭ ����!");
		return E_FAIL;
	}


	if(FAILED( D3DXCreateSprite(m_pD3dDevice, &m_pSprite) ))
	{
		AfxMessageBox(L"��������Ʈ��ü �ʱ�ȭ ����!");
		return E_FAIL;
	}

	//��Ʈ��ü �ʱ�ȭ
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESC));

	FontInfo.Height = 20;
	FontInfo.Width = 10;

	FontInfo.Weight = FW_BOLD;	//��Ʈ �β�
	FontInfo.CharSet = HANGUL_CHARSET;		//�ѱۻ��

	lstrcpy(FontInfo.FaceName, L"����");		//�۲��� ����.

	if(FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		AfxMessageBox(L"��Ʈ��ü �ʱ�ȭ ����!");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dp)
{
	d3dp.BackBufferWidth  = WINCX;
	d3dp.BackBufferHeight = WINCY;
	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dp.BackBufferCount = 1;	//����� ����

	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;		//�̹����� Ű���� �ٿ��� �Ҷ�.. �������������� �����Ұ��ΰ�?
	//������� ����.

	d3dp.MultiSampleQuality = 0;


	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//������� ���� ���۸� ����� ����.

	//���� : ���� �׸��� �׸��� �ִ� ������ �ǹ��Ѵ�.
	//		������ �⺻ ���� �Ѱ��� ������ �ְ�, ����۸� �߰������� 1���� ����� 
	//		�ְ� �Ǵµ�
	//      ȭ�麸�̴� ���۸� �������
	//		�����(������)�� ���� �׸��� ���� ���۴� �ĸ�����̴�.

	//���������� ������ �����͸� ������ �ִ�.
	//�����͵��� ���ؼ� �ȼ��鿡 �������ؼ� ���������� �����ϰ� �ȴ�.

	//�����Ͱ� ��ȯ�Ǹ鼭 �̹����� ��ü�� ȿ���� ��� �ȴ�.

	//->���� ü���� ���ؼ� ���ۿ� �׸��� �׸���.

	d3dp.hDeviceWindow = g_hWnd;		//������ �ڵ�

	d3dp.Windowed = true;		//��üȭ��(flase) / â���(TRUE)

	d3dp.EnableAutoDepthStencil = TRUE;
	//���ٽ� ����
	//���� ���۶�� ������� ������ �ִ�.

	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//��ü ���� �����Ͽ����� ������� �ֻ����� ���󰡵��� �����ϴ� �ɼ�.

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Render_Begin(void)
{
	m_pD3dDevice->Clear(0,	//������ ����
		NULL, //��ġ�� �ּҰ�
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : �ĸ����, 
		D3DCOLOR_ARGB(255, 0, 0, 0),		//�������?
		1.f,	//�������Ŀ� ���̹����� �ʱⰪ
		0);	//���ٽ� ������ �ʱⰪ

	m_pD3dDevice->BeginScene();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
void CDevice::Render_End(void)
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);
}

void CDevice::Render_End(HWND& _hWnd)
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, _hWnd, NULL);
}

void CDevice::Release(void)
{
	m_pFont->Release();
	m_pSprite->Release();
	m_pD3dDevice->Release();
	m_pD3D->Release();

}
CDevice::CDevice(void)
{
}

CDevice::~CDevice(void)
{
	Release();
}
