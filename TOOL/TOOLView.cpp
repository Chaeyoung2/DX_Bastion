
// TOOLView.cpp : CTOOLView Ŭ������ ����
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTOOLView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTOOLView ����/�Ҹ�

CTOOLView::CTOOLView()
:m_bIsLButtonPressed(false)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTOOLView::~CTOOLView()
{
}

BOOL CTOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CTOOLView �׸���

void CTOOLView::OnDraw(CDC* /*pDC*/)
{
	CTOOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	// ����!

	CDevice::GetInstance()->Render_Begin();

	
	
	m_pBackGround->Render();
	m_pMyObj->Render();

	CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();

	if(g_eViewID == VIEW_OBJTOOL 
		&& pObjTool->m_bIsSettingStructure == false) // �����ϱ� ���̶��
	{
		if(pObjTool->m_iCount >= 0)
			m_pMyObj->m_iTempCount = pObjTool->m_iCount;

		if(pObjTool->m_bIsStMode) // ��Ʈ���� ����
			m_pMyObj->TempRenderStruct();
		else
			m_pMyObj->TempRenderGround();
	}
		
	m_pMyUnit->Render();

	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CTOOLView �μ�


void CTOOLView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTOOLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTOOLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTOOLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CTOOLView ����

#ifdef _DEBUG
void CTOOLView::AssertValid() const
{
	CView::AssertValid();
}

void CTOOLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTOOLDoc* CTOOLView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTOOLDoc)));
	return (CTOOLDoc*)m_pDocument;
}
#endif //_DEBUG


// CTOOLView �޽��� ó����

void CTOOLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// ��ũ�� �� ������ ����.
	SetScrollSizes(MM_TEXT, CSize(static_cast<int>(TILECX * TILEX),
								static_cast<int>((TILECY * 0.5f) * TILEY )));

	//////////////////////////////////////////////////////////////////////////////////////

	// Viewâ ũ�� ������. â �׵θ��� ������ �Լ�.
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;

	// ������ ������ ������.
	pMainFrame->GetWindowRect(&rcWindow);

	// ���� �������� ũ�⸦ 0,0 �������� ����
	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);



	RECT rcMainView;

	// ���� view â�� ũ�� ������
	GetClientRect(&rcMainView);

	// �������� ����
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

	// ��׶��� (Ÿ��) �Ҵ�

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();
	m_pBackGround->SetMainView(this);

	// ���� �Ҵ�
	m_pMyUnit	 = new CMyUnit;
	m_pMyUnit->Initialize();
	m_pMyUnit->SetMainView(this);

	// ������Ʈ �Ҵ�
	m_pMyObj	 = new CMyObj;
	m_pMyObj->Initialize();
	m_pMyObj->SetMainView(this);

	/////////////////////////////////////////////////////////////////////////

	

	// ������ ��׶��� �ּ� �ѱ��
	CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
	pMapTool->SetBackGround(m_pBackGround);

	// �������� ���� �ּ� �ѱ�� (?)

	// ������Ʈ ���� ������Ʈ ����Ʈ�� �ּ� �ѱ��
	CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();
	pObjTool->SetStructureList(m_pMyObj->GetStructureList());
	pObjTool->SetGroundList(m_pMyObj->GetGroundList());


	// �ؽ���
	//m_pSingleTexture = new CSingleTexture;

	////////////////////////////////////////// 
	// �� ��
	// ��׶���
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/BackGround/BackGround%d.png"
		, L"BACKGROUND", TEX_MULTI, L"BackGround", 4)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	// Ÿ��
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/Tile/Tile%d.png"
		, L"Back", TEX_MULTI, L"Tile", 124)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	// Ÿ�� �ɼ�
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/TileOption/TileOption%d.png"
		, L"TILEOPTION", TEX_MULTI, L"TileOption", 6)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	// Ÿ�� ������
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/BackGround/TileBorder/TileBorder%d.png"
		, L"TILEBORDER", TEX_MULTI, L"TileBorder", 3)))
	{
		AfxMessageBox(L"TextureMgr Multi Load Error!");
	}

	///////////////////////////////////// 
	// ���� ��
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
	// ������Ʈ ��
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



	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CTOOLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(true == g_IsMainView)
		return;

	// Ÿ�� ��� (ID, option)

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);



	if(g_eViewID == VIEW_MAPTOOL)
	{
		if(GetAsyncKeyState(VK_LBUTTON))
			OnLButtonDown(nFlags, point); // ��� ��ŷ�Ϸ���
	}


	

	if(g_eViewID == VIEW_OBJTOOL)
	{
		// ������Ʈ ����ٴϰ� �ϱ�
		CObjTool* pObjTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjTool();

		if(pObjTool->m_bIsSettingStructure == false) // �����ϱ� ���̶��
		{
			if(pObjTool->m_iCount >= 0)
				m_pMyObj->m_iTempCount = pObjTool->m_iCount;
		}

	}
	

	Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE); // �̴Ϻ� ����


}



void CTOOLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(true == g_IsMainView)
		return;

	CScrollView::OnLButtonUp(nFlags, point);

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos.x += GetScrollPos(0);
	vMousePos.y += GetScrollPos(1);

	// Ÿ�� ���

	if(g_eViewID == VIEW_MAPTOOL)
	{
		CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
		m_pBackGround->TileSetting(vMousePos, pMapTool->m_fHeight, pMapTool->m_fMoveSpeed);
	}

	

	// ���� ���
	if(g_eViewID == VIEW_UNITTOOL)
	{
		CUnitTool* pUnitTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetUnitTool();
		//pUnitTool->OnBnClickedButtonUnitAdd();
		pUnitTool->SetUnitList(m_pMyUnit->GetUnitList());
		m_pMyUnit->SetUnit(vMousePos, pUnitTool->m_iUnitID, pUnitTool->m_iUnitDir);
		pUnitTool->SetAddedUnitList();
	}

	// ������Ʈ ���
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// ���κ䰡 ���õǸ� �� �Լ��� bActivate�� true�� ������, �װ� �ƴϸ� false�� ���´�.
// 	if(bActivate)
// 	{
// 		g_IsMainView = true;
// 	}


	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
