// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TOOL.h"
#include "UnitTool.h"
#include "MainFrm.h"
#include "TOOLView.h"
#include "MyUnit.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(CUnitTool::IDD, pParent)
	, m_strComboUnitDir(_T(""))
	, m_iUnitDir(0)
	, m_iUnitID(0)
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UNIT_DIR, m_ComboDirList);
	DDX_CBString(pDX, IDC_COMBO_UNIT_DIR, m_strComboUnitDir);
	DDX_Control(pDX, IDC_LIST_UNIT, m_ListBoxUnit);
	DDX_Control(pDX, IDC_LIST_UNIT_ADDED, m_ListBoxUnitAdded);
	DDX_Control(pDX, IDC_UNIT_PICTURE, m_UnitPicture);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_UNIT_ADDED, &CUnitTool::OnLbnSelchangeListUnitAdded)
	ON_CBN_SELCHANGE(IDC_COMBO_UNIT_DIR, &CUnitTool::OnCbnSelchangeComboUnitDir)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_ADD, &CUnitTool::OnBnClickedButtonUnitAdd)
	ON_LBN_SELCHANGE(IDC_LIST_UNIT, &CUnitTool::OnLbnSelchangeListUnit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UNIT_SAVE, &CUnitTool::OnBnClickedButtonUnitSave)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_LOAD, &CUnitTool::OnBnClickedButtonUnitLoad)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_DELETEALL, &CUnitTool::OnBnClickedButtonUnitDeleteall)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_DELETESELECTED, &CUnitTool::OnBnClickedButtonUnitDeleteselected)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.

BOOL CUnitTool::OnInitDialog()
{

	CDialog::OnInitDialog();
	m_ComboDirList.AddString(L"DIR_DOWN");
	m_ComboDirList.AddString(L"DIR_LEFT");
	m_ComboDirList.AddString(L"DIR_LEFTDOWN");
	m_ComboDirList.AddString(L"DIR_LEFTUP");
	m_ComboDirList.AddString(L"DIR_RIGHT");
	m_ComboDirList.AddString(L"DIR_RIGHTDOWN");
	m_ComboDirList.AddString(L"DIR_RIGHTUP");
	m_ComboDirList.AddString(L"DIR_UP");


	m_ComboDirList.SetCurSel(m_iUnitDir);

	// 유닛 리스트 박스에 정보 넣기.
	//int iIndex = 8; // 유닛마다 총 8개의 스프라이트 이미지 띄움.

	TCHAR szPath[MAX_STR] = L"";
	TCHAR szPath2[MAX_STR] = L"../Texture/UnitTool/Ex/%d.png";

	for(int i = 0; i < 32; ++i)
	{
		wsprintf(szPath, szPath2, i);
		wstring wstrRe = szPath;

		CString strImageName;

		if(i < 8)				strImageName = L"Player";

		if(i >= 8 && i < 16)	strImageName = L"Gaspella";

		if(i >= 16 && i < 24)	strImageName = L"Defense";

		if(i >= 24 && i < 32)	strImageName = L"Boss";

		
		// 방향 분기
//  		int iIndex = i;
//  		TCHAR szIndex[10] = L"";
//  		_itot_s(iIndex, szIndex, sizeof(szIndex));
//  
//  		//_tcscat(strImageName, szIndex);
// 		wsprintf(strImageName, strImageName.c_str(), i);
		


		// 맵 컨테이너
		map<CString, CImage*>::iterator iter;
		iter = m_mapUnitImage_Ex.find(strImageName);

		if(iter == m_mapUnitImage_Ex.end() ) // 없으면 할당
		{
			CImage* pImage = new CImage;
			pImage->Load( wstrRe.c_str() );
			m_mapUnitImage_Ex.insert( make_pair(strImageName, pImage) );
		}

		// 리스트 박스에 올릴 이름
		if(i == 0 || i == 8 || i == 16 || i == 24)
			m_ListBoxUnit.AddString(strImageName);

		
	}

	Invalidate(FALSE);


	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUnitTool::OnLbnSelchangeListUnitAdded()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

// 방향 콤보박스
void CUnitTool::OnCbnSelchangeComboUnitDir()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// CString 변수에 선택한 방향 넣어주기
	CString str = __T("");

	int i = m_ComboDirList.GetCurSel();
	m_ComboDirList.GetLBText(i, str);

	m_iUnitDir = i;
	m_strComboUnitDir = str;


	Invalidate(FALSE);
}

void CUnitTool::Release()
{
	for( map<CString, CImage*>::iterator iter = m_mapUnitImage_Ex.begin();
		iter != m_mapUnitImage_Ex.end(); ++iter)
	{
		iter->second->Destroy();
		Safe_Delete(iter->second);
	}
	m_mapUnitImage_Ex.clear();

// 	map<CString, UNIT_DATA*>::iterator iter;
// 	map<CString, UNIT_DATA*>::iterator iter_end;
// 
// 	iter = m_mapUnitData.begin();
// 	iter_end = m_mapUnitData.end();
// 
// 	for(iter; iter != iter_end; ++iter)
// 		Safe_Delete(iter->second);
// 
// 	m_mapUnitData.clear();
}

void CUnitTool::OnBnClickedButtonUnitAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//map<CString, UNIT_DATA*>::iterator find_iter;

	UpdateData(TRUE);

}

void CUnitTool::SetAddedUnitList()
{

	list<PMYUNIT>::iterator iter = m_pUnitList->begin();
	list<PMYUNIT>::iterator iter_end = m_pUnitList->end();

	for(; iter != iter_end; ++iter)
	{
		if((*iter)->bIsAdded == false)
		{
			m_ListBoxUnitAdded.AddString((*iter)->wstrName.c_str());
			(*iter)->bIsAdded = true;
		}
	}
// 	list<PMYUNIT>::iterator iter = m_pUnitList->end();
// 	--iter;
// 	m_ListBoxUnitAdded.AddString((*iter)->wstrName);

}


void CUnitTool::OnLbnSelchangeListUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//////////////////////////////////////////////////
	// 유닛 이미지 띄우기
	CString strSelectName;

	// 선택한 인덱스 내용을 strSelectName에 넣고
	m_ListBoxUnit.GetText(m_ListBoxUnit.GetCurSel(), strSelectName);

	// 현재 유닛 ID를 넣고
	m_iUnitID = m_ListBoxUnit.GetCurSel();


	// 이미지 세팅
	map<CString, CImage*>::iterator iter;
	iter = m_mapUnitImage_Ex.find(strSelectName);
	if(iter == m_mapUnitImage_Ex.end()) return;
	m_UnitPicture.SetBitmap( *iter->second );

	/////////////////////////////////////////////////
	// 리스트박스에서 선택한 항목의 이름 얻어오기.
	
	int iSelectIndex = m_ListBoxUnit.GetCurSel();

	if(iSelectIndex == -1) return;

	// 해당 인덱스에 있는 내용을 strSelectName에 넣는다.
// 	m_ListBoxUnit.GetText(iSelectIndex, strSelectName);
// 
// 
// 	map<CString, PMYUNIT>::iterator iter;
// 
// 	iter = m_mapUnitData.find(strSelectName);
// 
// 	if( iter == m_mapUnitData.end() ) return; //동일 키값이 있는지 탐색
// 
// 	




	UpdateData(FALSE);
}

void CUnitTool::HorizontalScroll()
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_ListBoxUnitAdded.GetDC();

	for(int i = 0; i < m_ListBoxUnitAdded.GetCount(); ++i)
	{
		m_ListBoxUnitAdded.GetText(i, strName);

		Size = pDc->GetTextExtent(strName);

		if(Size.cx > iCX) iCX = Size.cx;
	}

	m_ListBoxUnitAdded.ReleaseDC(pDc);

	if(m_ListBoxUnitAdded.GetHorizontalExtent() < iCX)
	{
		m_ListBoxUnitAdded.SetHorizontalExtent(iCX);
	}

}

void CUnitTool::OnDestroy()
{
	map<CString, CImage*>::iterator iter = m_mapUnitImage_Ex.begin();
	map<CString, CImage*>::iterator iter_end = m_mapUnitImage_Ex.end();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for ( ; iter!= iter_end; ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_mapUnitImage_Ex.clear();

	//
	g_IsMainView = true;

	//
// 	map<CString, CImage*>::iterator iter = m_mapUnitImage.begin();
// 	map<CString, CImage*>::iterator iter_end = m_mapUnitImage.end();
// 
// 	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
// 	for ( ; iter!= iter_end; ++iter)
// 	{
// 		iter->second->Destroy();
// 		::Safe_Delete(iter->second);
// 	}
// 	m_mapUnitImage.clear();

	// 추가
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CUnitTool::OnBnClickedButtonUnitSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 
// 	list<MYUNIT*>* pListUnit =
// 		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyUnit()->GetUnitList();
// 
// 	DWORD dwByte = 0;
// 
// 	HANDLE hUnitFile = CreateFile(L"../Data/UnitData.dat",
// 		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
// 
// 	list<MYUNIT*>::iterator iter = pListUnit->begin();
// 	list<MYUNIT*>::iterator iter_end = pListUnit->end();
// 
// 	for(; iter != iter_end; ++iter)
// 	{
// 		WriteFile(hUnitFile, (*iter), sizeof(MYUNIT), &dwByte, NULL);
// 	}
// 	CloseHandle(hUnitFile);


	list<MYUNIT*>* pListUnit =
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyUnit()->GetUnitList();

	CFileDialog Dlg(
		FALSE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat",
		this);

	if(Dlg.DoModal() == IDCANCEL) return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";

	HANDLE hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte = 0;

	list<MYUNIT*>::iterator iter = pListUnit->begin();
	list<MYUNIT*>::iterator iter_end = pListUnit->end();

	for(; iter != iter_end; ++iter)
	{
		WriteFile(hFile, (*iter), sizeof(MYUNIT), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CUnitTool::OnBnClickedButtonUnitLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyUnit()
		->Release();

	m_ListBoxUnitAdded.ResetContent();


	list<MYUNIT*>* pListUnit = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyUnit()->GetUnitList();


	CFileDialog	Dlg(
		TRUE,
		L"dat",		//파일의 확장자 명
		L"*.dat",	//최초 파일 이름
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //이름이 동일한 중복파일 처리에 대한 옵션.
		L"*.dat",	//파일형식
		this		//파일다이얼로그가 생성되었을때 기준이 되는 WIN
		);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";


	HANDLE hFile = CreateFile(
		Dlg.GetPathName(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwByte = 0;

	while(true)
	{
		MYUNIT* pUnit = new MYUNIT;

		ReadFile(hFile, pUnit, sizeof(MYUNIT), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pUnit);
			break;
		}

		pListUnit->push_back(pUnit);
		m_ListBoxUnitAdded.AddString((pUnit->wstrName).c_str());
	}

	CloseHandle(hFile);

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

// 
// 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyUnit()
// 		->Release();
// 
// 	m_ListBoxUnitAdded.ResetContent();
// 
// 
// 	list<MYUNIT*>* pListUnit = ((CMainFrame*)AfxGetMainWnd())
// 		->GetMainView()->GetMyUnit()->GetUnitList();
// 
// 	DWORD dwByte = 0;
// 
// 	HANDLE hFile = CreateFile(L"../Data/UnitData.dat",
// 		GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
// 		NULL);
// 
// 	while(true)
// 	{
// 		PMYUNIT pUnit = new MYUNIT;
// 
// 		ReadFile(hFile, pUnit, sizeof(MYUNIT), &dwByte, NULL);
// 
// 		if(dwByte == 0)
// 		{
// 			Safe_Delete(pUnit);
// 			break;
// 		}
// 		pListUnit->push_back(pUnit);
// 		m_ListBoxUnitAdded.AddString(pUnit->wstrName);
// 	}
// 
// 	CloseHandle(hFile);
// 
// 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate();
}

void CUnitTool::OnBnClickedButtonUnitDeleteall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	list<MYUNIT*>* pListUnit = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyUnit()->GetUnitList();

	list<PMYUNIT>::iterator iter = pListUnit->begin();
	list<PMYUNIT>::iterator iter_end = pListUnit->end();

	for(; iter != iter_end;)
	{
		Safe_Delete((*iter));
		iter = pListUnit->erase(iter);
	}

	//
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	m_ListBoxUnitAdded.ResetContent();
}

void CUnitTool::OnBnClickedButtonUnitDeleteselected()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CUnitTool::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(nState)
		g_IsMainView = false;

	//CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
