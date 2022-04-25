
// TOOLView.cpp : CTOOLView 클래스의 구현
//

#include "stdafx.h"
#include "TOOL.h"

#include "TOOLDoc.h"
#include "TOOLView.h"

#include "Include.h"

#include "Device.h"
#include "TextureMgr.h"
#include "SingleTexture.h"

#include "BackGround.h"

#include "MainFrm.h"
#include "Mouse.h"

#include "MiniView.h"
#include "MapTool.h"
#include "MyForm.h"

#include "MyUnit.h"
#include "MyObj.h"

#include "KeyMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CTOOLView

IMPLEMENT_DYNCREATE(CTOOLView, CScrollView)

BEGIN_MESSAGE_MAP(CTOOLView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTOOLView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTOOLView 생성/소멸

CTOOLView::CTOOLView()
:m_bIsLButtonPressed(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTOOLView::~CTOOLView()
{
}

BOOL CTOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTOOLView 그리기

void CTOOLView::OnDraw(CDC* /*pDC*/)
{
	CTOOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	// 오오!

	CDevice::GetInstance()->Render_Begin();

	
	
	m_pBackGround->Render();
	m_pMyObj->Render();

	CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();

	if(g_eViewID == VIEW_OBJTOOL 
		&& pObjTool->m_bIsSettingStructure == false) // 세팅하기 전이라면
	{
		if(pObjTool->m_iCount >= 0)
			m_pMyObj->m_iTempCount = pObjTool->m_iCount;

		if(pObjTool->m_bIsStMode) // 스트럭쳐 모드면
			m_pMyObj->TempRenderStruct();
		else
			m_pMyObj->TempRenderGround();
	}
		
	m_pMyUnit->Render();

	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CTOOLView 인쇄


void CTOOLView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTOOLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTOOLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTOOLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CTOOLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTOOLView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTOOLView 진단

#ifdef _DEBUG
void CTOOLView::AssertValid() const
{
	CView::AssertValid();
}

void CTOOLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTOOLDoc* CTOOLView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTOOLDoc)));
	return (CTOOLDoc*)m_pDocument;
}
#endif //_DEBUG


// CTOOLView 메시지 처리기

void CTOOLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// 스크롤 뷰 사이즈 정의.
	SetScrollSizes(MM_TEXT, CSize(static_cast<int>(TILECX * TILEX),
								static_cast<int>((TILECY * 0.5f) * TILEY )));

	//////////////////////////////////////////////////////////////////////////////////////

	// View창 크기 재정의. 창 테두리를 얻어오는 함수.
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;

	// 프레임 사이즈 얻어오기.
	pMainFrame->GetWindowRect(&rcWindow);

	// 얻어온 프레임의 크기를 0,0 기준으로 변경
	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);



	RECT rcMainView;

	// 순수 view 창의 크기 얻어오기
	GetClientRect(&rcMainView);

	// 프레임의 길이
	float fX = float(rcWindow.right - rcMainView.right);
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(NULL,  0, 0, int(WINCX + fX), int(WINCY + fY), SWP_NOZORDER);

	//////////////////////////////////////////////////////////////////////////////////////

	g_hWnd = m_hWnd;

	if(FAILED( CDevice::GetInstance()->Init3D() ))
	{
		AfxMessageBox(L"Device Create Failed!");
	}

	/////////////////////////////////////////////////////////////////////////////////////

	// 백그라운드 (타일) 할당

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();
	m_pBackGround->SetMainView(this);

	// 유닛 할당
	m_pMyUnit	 = new CMyUnit;
	m_pMyUnit->Initialize();
	m_pMyUnit->SetMainView(this);

	// 오브젝트 할당
	m_pMyObj	 = new CMyObj;
	m_pMyObj->Initialize();
	m_pMyObj->SetMainView(this);

	/////////////////////////////////////////////////////////////////////////

	

	// 맵툴에 백그라운드 주소 넘기기
	CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
	pMapTool->SetBackGround(m_pBackGround);

	// 유닛툴에 유닛 주소 넘기기 (?)

	// 오브젝트 툴에 오브젝트 리스트들 주소 넘기기
	CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();
	pObjTool->SetStructureList(m_pMyObj->GetStructureList());
	pObjTool->SetGroundList(m_pMyObj->GetGroundList());


	// 텍스쳐
	//m_pSingleTexture = new CSingleTexture;

	////////////////////////////////////////// 
	// 맵 툴
	// 백그라운드
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/BackGround/BackGround%d.png"
		, L"BACKGROUND", TEX_MULTI, L"BackGround", 4)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	// 타일
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/Tile/Tile%d.png"
		, L"Back", TEX_MULTI, L"Tile", 124)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	// 타일 옵션
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/TileOption/TileOption%d.png"
		, L"TILEOPTION", TEX_MULTI, L"TileOption", 6)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	// 타일 윤곽선
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/TileBorder/TileBorder%d.png"
		, L"TILEBORDER", TEX_MULTI, L"TileBorder", 3)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	///////////////////////////////////// 
	// 유닛 툴
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/UnitTool/Player/%d.png"
		, L"UnitPlayer", TEX_MULTI, L"Player", 8)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/UnitTool/Gaspella/%d.png"
		, L"UnitGaspella", TEX_MULTI, L"Gaspella", 8)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/UnitTool/Defense/%d.png"
		, L"UnitDefense", TEX_MULTI, L"Defense", 8)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/UnitTool/Boss/%d.png"
		, L"UnitBoss", TEX_MULTI, L"Boss", 8)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	////////////////////////////////////////////////////////////////////////
	// 오브젝트 툴
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/Structure/%d.png"
		, L"STRUCT", TEX_MULTI, L"Struct", 71)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/Ground/Ground%d.png"
		, L"GROUND", TEX_MULTI, L"Ground", 11)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}



	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CTOOLView::OnDestroy()
{
	CScrollView::OnDestroy();

	CDevice::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();

	// Safe_Delete(m_pSingleTexture);
	Safe_Delete(m_pBackGround);
	Safe_Delete(m_pMyUnit);
	Safe_Delete(m_pMyObj);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CTOOLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(true == g_IsMainView)
		return;

	// 타일 찍기 (ID, option)

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos.x += GetScrollPos(0);
	vMousePos.y += GetScrollPos(1);

	if(g_eViewID == VIEW_MAPTOOL)
	{
		CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
// 		if(GetAsyncKeyState(VK_LBUTTON))
// 		{
			m_pBackGround->TileChange(vMousePos, pMapTool->m_iDrawID, pMapTool->m_iOption);
//		}
	}


	Invalidate(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);

	//CMiniView* pMiniView = ((CMainFrame*)AfxGetMainWnd())->GetMiniView();

	//pMiniView->InvalidateRect(FALSE);

//	CView::OnLButtonDown(nFlags, point);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTOOLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case VK_ESCAPE:

		//(CMainFrame*)AfxGetMainWnd()
		dynamic_cast<CMainFrame*>(AfxGetMainWnd())->DestroyWindow();

		break;
	}
}

void CTOOLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);



	if(g_eViewID == VIEW_MAPTOOL)
	{
		if(GetAsyncKeyState(VK_LBUTTON))
			OnLButtonDown(nFlags, point); // 계속 피킹하려고
	}


	

	if(g_eViewID == VIEW_OBJTOOL)
	{
		// 오브젝트 따라다니게 하기
		CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();

		if(pObjTool->m_bIsSettingStructure == false) // 세팅하기 전이라면
		{
			if(pObjTool->m_iCount >= 0)
				m_pMyObj->m_iTempCount = pObjTool->m_iCount;
		}

	}
	

	Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE); // 미니뷰 갱신


}



void CTOOLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(true == g_IsMainView)
		return;

	CScrollView::OnLButtonUp(nFlags, point);

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos.x += GetScrollPos(0);
	vMousePos.y += GetScrollPos(1);

	// 타일 찍기

	if(g_eViewID == VIEW_MAPTOOL)
	{
		CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
		m_pBackGround->TileSetting(vMousePos, pMapTool->m_fHeight, pMapTool->m_fMoveSpeed);
	}

	

	// 유닛 찍기
	if(g_eViewID == VIEW_UNITTOOL)
	{
		CUnitTool* pUnitTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetUnitTool();
		//pUnitTool->OnBnClickedButtonUnitAdd();
		pUnitTool->SetUnitList(m_pMyUnit->GetUnitList());
		m_pMyUnit->SetUnit(vMousePos, pUnitTool->m_iUnitID, pUnitTool->m_iUnitDir);
		pUnitTool->SetAddedUnitList();
	}

	// 오브젝트 찍기
	if(g_eViewID == VIEW_OBJTOOL)
	{
		CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();
		pObjTool->SetStructureList( m_pMyObj->GetStructureList() );
		pObjTool->SetGroundList( m_pMyObj->GetGroundList() );
		m_pMyObj->SetObject( vMousePos, 
			pObjTool->m_iObjID, pObjTool->m_iRenderOption, pObjTool->m_iCount, 
			pObjTool->GetStIndex(), pObjTool->GetGrIndex(), pObjTool->m_iCollCount, 
			pObjTool->m_bIsAgreeToDamage);

		pObjTool->SetAddedObjList();

		int iOldCount = pObjTool->m_iCount;

		pObjTool->m_bIsSettingStructure = false;
	}

	Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
}

void CTOOLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 메인뷰가 선택되면 이 함수의 bActivate가 true로 들어오고, 그게 아니면 false로 들어온다.
// 	if(bActivate)
// 	{
// 		g_IsMainView = true;
// 	}


	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
