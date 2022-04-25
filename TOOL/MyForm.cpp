// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TOOL.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "TOOLView.h"
#include "BackGround.h"


eViewID g_eViewID = VIEW_MAINVIEW;


// MyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
	, m_iStage(0)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Stage, m_ComboListStage);
	DDX_CBIndex(pDX, IDC_COMBO_Stage, m_iStage);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_MAPTOOL, &CMyForm::OnBnClickedButtonMaptool)
	ON_BN_CLICKED(IDC_BUTTON_OBJTOOL, &CMyForm::OnBnClickedButtonObjtool)
	ON_BN_CLICKED(IDC_BUTTON_UNITTOOL, &CMyForm::OnBnClickedButtonUnittool)
	ON_BN_CLICKED(IDC_BUTTON_PATHFIND, &CMyForm::OnBnClickedButtonPathfind)
	ON_CBN_SELCHANGE(IDC_COMBO_Stage, &CMyForm::OnCbnSelchangeComboStage)
END_MESSAGE_MAP()


// MyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyForm 메시지 처리기입니다.

void CMyForm::OnBnClickedButtonMaptool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_eViewID = VIEW_MAPTOOL;

	if(m_MapTool.GetSafeHwnd() == NULL)
		m_MapTool.Create(IDD_MAPTOOL);

	m_MapTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedButtonObjtool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_eViewID = VIEW_OBJTOOL;

	if(m_ObjTool.GetSafeHwnd() == NULL)
		m_ObjTool.Create(IDD_OBJTOOL);
	m_ObjTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedButtonUnittool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_eViewID = VIEW_UNITTOOL;

	if(m_UnitTool.GetSafeHwnd() == NULL)
		m_UnitTool.Create(IDD_UNITTOOL);
	m_UnitTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedButtonPathfind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_eViewID = VIEW_PATHFIND;

	if(m_PathFind.GetSafeHwnd() == NULL)
		m_PathFind.Create(IDD_PATHFIND);
	m_PathFind.ShowWindow(SW_SHOW);
}

void CMyForm::OnCbnSelchangeComboStage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iStage = m_ComboListStage.GetCurSel();
	m_iStage = iStage;

	CBackGround* pBackGround = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->
		GetBackGround();

	pBackGround->m_iStage = m_iStage;


	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

}

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ComboListStage.AddString(L"Stage1");
	m_ComboListStage.AddString(L"DiaPub");
	m_ComboListStage.AddString(L"Stage2");
	m_ComboListStage.AddString(L"Stage3");

	m_ComboListStage.SetCurSel(m_iStage);


	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
