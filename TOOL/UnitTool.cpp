// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TOOL.h"
#include "UnitTool.h"
#include "MainFrm.h"
#include "TOOLView.h"
#include "MyUnit.h"


// CUnitTool ��ȭ �����Դϴ�.

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


// CUnitTool �޽��� ó�����Դϴ�.

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

	// ���� ����Ʈ �ڽ��� ���� �ֱ�.
	//int iIndex = 8; // ���ָ��� �� 8���� ��������Ʈ �̹��� ���.

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

		
		// ���� �б�
//  		int iIndex = i;
//  		TCHAR szIndex[10] = L"";
//  		_itot_s(iIndex, szIndex, sizeof(szIndex));
//  
//  		//_tcscat(strImageName, szIndex);
// 		wsprintf(strImageName, strImageName.c_str(), i);
		


		// �� �����̳�
		map<CString, CImage*>::iterator iter;
		iter = m_mapUnitImage_Ex.find(strImageName);

		if(iter == m_mapUnitImage_Ex.end() ) // ������ �Ҵ�
		{
			CImage* pImage = new CImage;
			pImage->Load( wstrRe.c_str() );
			m_mapUnitImage_Ex.insert( make_pair(strImageName, pImage) );
		}

		// ����Ʈ �ڽ��� �ø� �̸�
		if(i == 0 || i == 8 || i == 16 || i == 24)
			m_ListBoxUnit.AddString(strImageName);

		
	}

	Invalidate(FALSE);


	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CUnitTool::OnLbnSelchangeListUnitAdded()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}

// ���� �޺��ڽ�
void CUnitTool::OnCbnSelchangeComboUnitDir()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	// CString ������ ������ ���� �־��ֱ�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	//////////////////////////////////////////////////
	// ���� �̹��� ����
	CString strSelectName;

	// ������ �ε��� ������ strSelectName�� �ְ�
	m_ListBoxUnit.GetText(m_ListBoxUnit.GetCurSel(), strSelectName);

	// ���� ���� ID�� �ְ�
	m_iUnitID = m_ListBoxUnit.GetCurSel();


	// �̹��� ����
	map<CString, CImage*>::iterator iter;
	iter = m_mapUnitImage_Ex.find(strSelectName);
	if(iter == m_mapUnitImage_Ex.end()) return;
	m_UnitPicture.SetBitmap( *iter->second );

	/////////////////////////////////////////////////
	// ����Ʈ�ڽ����� ������ �׸��� �̸� ������.
	
	int iSelectIndex = m_ListBoxUnit.GetCurSel();

	if(iSelectIndex == -1) return;

	// �ش� �ε����� �ִ� ������ strSelectName�� �ִ´�.
// 	m_ListBoxUnit.GetText(iSelectIndex, strSelectName);
// 
// 
// 	map<CString, PMYUNIT>::iterator iter;
// 
// 	iter = m_mapUnitData.find(strSelectName);
// 
// 	if( iter == m_mapUnitData.end() ) return; //���� Ű���� �ִ��� Ž��
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
// 	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
// 	for ( ; iter!= iter_end; ++iter)
// 	{
// 		iter->second->Destroy();
// 		::Safe_Delete(iter->second);
// 	}
// 	m_mapUnitImage.clear();

	// �߰�
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CUnitTool::OnBnClickedButtonUnitSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyUnit()
		->Release();

	m_ListBoxUnitAdded.ResetContent();


	list<MYUNIT*>* pListUnit = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyUnit()->GetUnitList();


	CFileDialog	Dlg(
		TRUE,
		L"dat",		//������ Ȯ���� ��
		L"*.dat",	//���� ���� �̸�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //�̸��� ������ �ߺ����� ó���� ���� �ɼ�.
		L"*.dat",	//��������
		this		//���ϴ��̾�αװ� �����Ǿ����� ������ �Ǵ� WIN
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CUnitTool::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(nState)
		g_IsMainView = false;

	//CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
