// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TOOL.h"
#include "MiniView.h"
#include "Device.h"
#include "BackGround.h"
#include "MyObj.h"
#include "MyUnit.h"
#include "MainFrm.h"
#include "TOOLView.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CBackGround* pBackGround = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetBackGround();
	CMyUnit* pMyUnit = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyUnit();
	CMyObj* pMyObj = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyObj();

	CDevice::GetInstance()->Render_Begin();

	pBackGround->MiniMapRender();
	pMyUnit->MiniRender();
	pMyObj->MiniRender();


	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.

void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
