#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)


HRESULT CDevice::Init3D(void)
{
	m_pD3D = NULL;
	m_pD3dDevice = NULL;

	//D3D객체를 생성. -> D3dDevice를 사용하기 위해서.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//장치의 성능 조사. -> 저장하는 변수 선언.
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		&DeviceCaps) ) )
	{
		//초기화가 실패한다면 여기로!
		AfxMessageBox(L"장치 초기화 실패!");
		//ERROR_TEXT(L"장치 초기화 실패!");
		return E_FAIL;
	}

	DWORD vp;

	//불러들인 정보에 따라서
	//점과 조명에 대한 연산을 하드웨어가 할지 소프트웨어가 할지를 판단하게 된다.
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		//그래픽 카드를 지원을 한다면
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//Cpu
		//그래픽 카드를 지원하지 않는다면
	}

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
		AfxMessageBox(L"디바이스 초기화 실패!");
		//ERROR_TEXT(L"디바이스 초기화 실패!");
		return E_FAIL;
	}


	if(FAILED( D3DXCreateSprite(m_pD3dDevice, &m_pSprite) ))
	{
		AfxMessageBox(L"스프라이트객체 초기화 실패!");
		return E_FAIL;
	}

	//폰트객체 초기화
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESC));

	FontInfo.Height = 20;
	FontInfo.Width = 10;

	FontInfo.Weight = FW_BOLD;	//폰트 두께
	FontInfo.CharSet = HANGUL_CHARSET;		//한글사용

	lstrcpy(FontInfo.FaceName, L"돋움");		//글꼴을 지정.

	if(FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		AfxMessageBox(L"폰트객체 초기화 실패!");
		return E_FAIL;
	}

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

void CDevice::Render_Begin(void)
{
	m_pD3dDevice->Clear(0,	//버퍼의 개수
		NULL, //위치의 주소값
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET : 후면버퍼, 
		D3DCOLOR_ARGB(255, 0, 0, 0),		//어떤색으로?
		1.f,	//삭제한후에 깊이버퍼의 초기값
		0);	//스텐실 버퍼의 초기값

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
