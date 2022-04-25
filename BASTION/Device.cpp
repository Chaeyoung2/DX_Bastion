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

	// D3dDevice를 사용하기 위한 D3D 객체 생성
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION); 

	// 장치 성능 조사, 저장 변수 선언
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		&DeviceCaps) ) )
	{
		//초기화 실패시
		//AfxMessageBox(L"장치 초기화 실패!");
		//ERROR_TEXT(L"장치 초기화 실패!");
		return E_FAIL;
	}

	DWORD vp;
	//불러들인 정보에 따라서
	//점과 조명에 대한 연산을 하드웨어가 할지 소프트웨어가 할지를 판단하게 된다.
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
		//그래픽 카드를 지원을 한다면
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		//Cpu
		//그래픽 카드를 지원하지 않는다면
	}

	vp |= D3DCREATE_MULTITHREADED;		//##쓰레드사용하고 싶다면 반드시 추가해야한다.



	//장치를 생성할때 개발자가 세팅해줘야하는 장치 정보
	D3DPRESENT_PARAMETERS d3dp;
	ZeroMemory(&d3dp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dp);		//장치 정보를 설정!

	if(FAILED(m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		vp,
		&d3dp,
		&m_pD3dDevice)) )
	{
		//AfxMessageBox(L"디바이스 초기화 실패!");
		//ERROR_TEXT(L"디바이스 초기화 실패!");
		return E_FAIL;
	}


	if(FAILED( D3DXCreateSprite(m_pD3dDevice, &m_pSprite) ))
	{
		//AfxMessageBox(L"스프라이트객체 초기화 실패!");
		return E_FAIL;
	}



	//폰트객체 초기화
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESC));

	FontInfo.Height = 10;
	FontInfo.Width = 10;

	FontInfo.Weight = FW_NORMAL;	//폰트 두께
	FontInfo.CharSet = HANGUL_CHARSET;		//한글사용

	//lstrcpy(FontInfo.FaceName, L"궁서");		//글꼴을 지정.

	if(FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		//AfxMessageBox(L"폰트객체 초기화 실패!");
		return E_FAIL;
	}

// 	if(FAILED(D3DXCreateLine(m_pD3dDevice, &m_pLine)))
// 	{
// 		MSG_BOX(L"라인 객체 생성 실패");
// 		return E_FAIL;
// 	}


	return S_OK;



}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dp)
{

 	d3dp.BackBufferWidth  = WINCX;
 	d3dp.BackBufferHeight = WINCY;

	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dp.BackBufferCount = 1;	//백버퍼 개수

	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;		//이미지를 키웠다 줄였다 할때.. 뭉개지는현상을 보정할것인가?
	//사용하지 않음.

	d3dp.MultiSampleQuality = 0;


	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//백버퍼의 더블 버퍼링 방식을 설정.

	//버퍼 : 내가 그림을 그리고 있는 공간을 의미한다.
	//		무조건 기본 버퍼 한개를 가지고 있고, 백버퍼를 추가적으로 1개더 만들어 
	//		주게 되는데
	//      화면보이는 버퍼를 전면버퍼
	//		사용자(개발자)가 무언가 그리기 위한 버퍼는 후면버퍼이다.

	//내부적으로 각자의 포인터를 가지고 있다.
	//포인터들을 통해서 픽셀들에 접근을해서 색상정보를 변경하게 된다.

	//포인터가 교환되면서 이미지가 교체된 효과를 얻게 된다.

	//->스왑 체인을 통해서 버퍼에 그림을 그린다.

	d3dp.hDeviceWindow = g_hWnd;		//윈도우 핸들

	d3dp.Windowed = true;		//전체화면(flase) / 창모드(TRUE)

	d3dp.EnableAutoDepthStencil = TRUE;
	//스텐실 버퍼
	//깊이 버퍼라고 어느정도 볼수가 있다.

	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//전체 모드로 실행하였을때 모니터의 주사율을 따라가도록 설정하는 옵션.

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

}

void CDevice::Render_Begin()
{
// 	m_pD3dDevice->Clear(0,	//버퍼의 개수
// 						NULL, //위치의 주소값
// 						D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : 후면버퍼, 
// 						D3DCOLOR_ARGB(255, 0, 0, 255),		//어떤색으로?
// 						1.f,	//삭제한후에 깊이버퍼의 초기값
// 						0);	//스텐실 버퍼의 초기값

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

	///////////////////// 0911 줌인 줌아웃

	m_pD3dDevice->Present(&m_tSourRect, &m_tDestRect, NULL, NULL); //3인자 : 핸들*/
//	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);


	//m_pD3dDevice->Present(NULL, NULL, NULL, NULL);//4인자 : RGNDATA
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

	// 조건 걸어주기
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

	// 줌아웃 조건 걸어주기
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
