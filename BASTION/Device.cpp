#include "StdAfx.h"
#include "Device.h"
#include "TimeMgr.h"
#include "SubTitle.h"
#include "Factory.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice(void)
: m_pD3D(NULL)
, m_pD3dDevice(NULL)
, m_pSprite(NULL)
, m_pFont(NULL)
, m_fX(160.f), m_fY(90.f), 
 m_fZoomSpeedX(2.f), m_fZoomSpeedY(0.0001f)
 , m_fTime(0.f), m_fZoomTime(0.1f)
 , m_bIsMakeCompleteSub1(false), m_fZoomInTime(0.f)
{									 
	m_tSourRect.left = 0;
	m_tSourRect.right = WINCX;
	m_tSourRect.bottom = WINCY;
	m_tSourRect.top = 0;

	m_tDestRect.left = 0;
	m_tDestRect.right = WINCX;
	m_tDestRect.bottom = WINCY;
	m_tDestRect.top = 0;

}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::Init3D()
{
	m_hDC = GetDC(g_hWnd);

	m_pD3D = NULL;
	m_pD3dDevice = NULL;

	// D3dDevice�� ����ϱ� ���� D3D ��ü ����
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION); 

	// ��ġ ���� ����, ���� ���� ����
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		&DeviceCaps) ) )
	{
		//�ʱ�ȭ ���н�
		//AfxMessageBox(L"��ġ �ʱ�ȭ ����!");
		//ERROR_TEXT(L"��ġ �ʱ�ȭ ����!");
		return E_FAIL;
	}

	DWORD vp;
	//�ҷ����� ������ ����
	//���� ���� ���� ������ �ϵ��� ���� ����Ʈ��� ������ �Ǵ��ϰ� �ȴ�.
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
		//�׷��� ī�带 ������ �Ѵٸ�
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		//Cpu
		//�׷��� ī�带 �������� �ʴ´ٸ�
	}

	vp |= D3DCREATE_MULTITHREADED;		//##���������ϰ� �ʹٸ� �ݵ�� �߰��ؾ��Ѵ�.



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
		//AfxMessageBox(L"����̽� �ʱ�ȭ ����!");
		//ERROR_TEXT(L"����̽� �ʱ�ȭ ����!");
		return E_FAIL;
	}


	if(FAILED( D3DXCreateSprite(m_pD3dDevice, &m_pSprite) ))
	{
		//AfxMessageBox(L"��������Ʈ��ü �ʱ�ȭ ����!");
		return E_FAIL;
	}



	//��Ʈ��ü �ʱ�ȭ
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESC));

	FontInfo.Height = 10;
	FontInfo.Width = 10;

	FontInfo.Weight = FW_NORMAL;	//��Ʈ �β�
	FontInfo.CharSet = HANGUL_CHARSET;		//�ѱۻ��

	//lstrcpy(FontInfo.FaceName, L"�ü�");		//�۲��� ����.

	if(FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		//AfxMessageBox(L"��Ʈ��ü �ʱ�ȭ ����!");
		return E_FAIL;
	}

// 	if(FAILED(D3DXCreateLine(m_pD3dDevice, &m_pLine)))
// 	{
// 		MSG_BOX(L"���� ��ü ���� ����");
// 		return E_FAIL;
// 	}


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

void CDevice::Render_Begin()
{
// 	m_pD3dDevice->Clear(0,	//������ ����
// 						NULL, //��ġ�� �ּҰ�
// 						D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : �ĸ����, 
// 						D3DCOLOR_ARGB(255, 0, 0, 255),		//�������?
// 						1.f,	//�������Ŀ� ���̹����� �ʱⰪ
// 						0);	//���ٽ� ������ �ʱⰪ

	m_pD3dDevice->BeginScene();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CDevice::Render_End()
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();

// 
   	if(g_bIsZoomIn == true)
   		ZoomIn();
  
   	if(g_bIsZoomOut == true)
   		ZoomOut();

// 	system("cls");
// 	cout << "m_fTime: "			<< m_fTime			<< endl;
// 	cout << "m_fX: "			<< m_fX			<< endl;
// 	cout << "m_fY: "			<< m_fY			<< endl;
// 	cout << "g_bIsZoomIn: "		<< g_bIsZoomIn		<< endl;
// 	cout << "g_bIsZoomOut: "	<< g_bIsZoomOut	<< endl;

	if(g_bIsMenuToStage1)
	{
// 		int iX = 180;
// 		int iY = 90;
		m_tSourRect.left = m_fX;
		m_tSourRect.top = m_fY;
		m_tSourRect.bottom = (WINCY - m_fY);
		m_tSourRect.right = (WINCX - m_fX);

		g_bIsZoomOut = true;

		g_bIsMenuToStage1 = false;
	}

	///////////////////// 0911 ���� �ܾƿ�

	m_pD3dDevice->Present(&m_tSourRect, &m_tDestRect, NULL, NULL); //3���� : �ڵ�*/
//	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);


	//m_pD3dDevice->Present(NULL, NULL, NULL, NULL);//4���� : RGNDATA
}

void CDevice::Render_End(HWND& _hWnd)
{
//  	if(m_bIsZoomIn && g_eScene == SCENE_STAGE1)
//  		ZoomIn();
//  
//  	if(m_bIsZoomOut)
//  		ZoomOut();


	m_pSprite->End();
	m_pD3dDevice->EndScene();


	m_pD3dDevice->Present(&m_tSourRect, &m_tDestRect, _hWnd, NULL);
	//m_pD3dDevice->Present(NULL, NULL, _hWnd, NULL);

}

void CDevice::Release()
{
//	m_pLine->Release();
	m_pFont->Release();
	m_pSprite->Release();
	m_pD3dDevice->Release();
	m_pD3D->Release();
}

void CDevice::ZoomIn()
{
	m_fTime += GET_TIME;
	

	if(m_fTime >= m_fZoomTime)
	{
		if (m_fX < 180)
		{
			m_fX += 2;
		}

		if (m_fY < 90)
		{
			m_fY += 1;
		}

		m_fTime = 0.f;
	}

	// ���� �ɾ��ֱ�
	if(m_fY >= 90)
	{
		m_fZoomInTime += GET_TIME;

		g_bIsShaking = false;

		if(m_fZoomInTime >= 2.f)
		{
			g_bIsShaking = true;

			g_bIsZoomIn = false;
			g_bIsZoomOut = true;
			m_fZoomInTime = 0.f;
		}

	}
// 


	m_tSourRect.left = m_fX;
	m_tSourRect.right = (WINCX - m_fX);
	m_tSourRect.top = m_fY;
	m_tSourRect.bottom = (WINCY - m_fY);

}

void CDevice::ZoomOut()
{

	m_fTime += GET_TIME;

	if(m_fTime >= m_fZoomTime)
	{

		if (m_fX >= 0)
		{
			m_fX -= m_fZoomSpeedX;
		}

		if (m_fY >= 0)
		{
			m_fY -= m_fZoomSpeedY;
		}

		m_fTime = 0.f;

	}

	// �ܾƿ� ���� �ɾ��ֱ�
	if(m_fX <= 0.f && m_fY <= 0.f)
	{
		g_bIsZoomOut = false;

		if(m_bIsMakeCompleteSub1 == false)
		{
			CObjMgr::GetInstance()->AddObject( 
				CFactory<CSubTitle>::CreateSubTitle(1.f), OBJ_SCREEN);
			CSoundMgr::GetInstance()->PlaySound(L"Narrator02.MP3", CHANNEL_SUBTITLE02);

			m_bIsMakeCompleteSub1 = true;

			m_fZoomTime = 0.01f;
			
		}

		m_fX = 0;
		m_fY = 0;
	}



	m_tSourRect.left = m_fX;
	m_tSourRect.right = (WINCX - m_fX);
	m_tSourRect.top = m_fY;
	m_tSourRect.bottom = (WINCY - m_fY);

}
