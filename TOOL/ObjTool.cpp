// ObjTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TOOL.h"
#include "ObjTool.h"
#include "MainFrm.h"
#include "TOOLView.h"
#include "MiniView.h"
#include "MyObj.h"


// CObjTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CObjTool, CDialog)

CObjTool::CObjTool(CWnd* pParent /*=NULL*/)
	: CDialog(CObjTool::IDD, pParent), m_bIsStMode(TRUE)
	,m_fAngle(0.f)
	, m_iRenderOption(0), m_iObjID(0)
	, m_iStIndex(0), m_iGrIndex(0), m_bIsSettingStructure(true), m_iCollCount(0)
	, m_bIsAgreeToDamage(false)
{	  

}

CObjTool::~CObjTool()
{
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_PICTURE, m_ObjPicture);
	DDX_Control(pDX, IDC_LIST_OBJ_STRUCTURE, m_ListBoxStructure);
	DDX_Control(pDX, IDC_LIST_OBJ_GROUND, m_ListBoxGround);
	DDX_Control(pDX, IDC_COMBO_RENDERING_OPTION, m_ComboRenderOption);
	DDX_CBIndex(pDX, IDC_COMBO_RENDERING_OPTION, m_iRenderOption);
	DDV_MinMaxInt(pDX, m_iRenderOption, 0, 2);
	DDX_Control(pDX, IDC_LIST_GR_ADDED, m_ListBoxGrAdded);
	DDX_Control(pDX, IDC_LIST_ST_ADDED, m_ListBoxStAdded);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_GR_ST, &CObjTool::OnBnClickedCheckGrSt)
	ON_LBN_SELCHANGE(IDC_LIST_OBJ_STRUCTURE, &CObjTool::OnLbnSelchangeListObjStructure)
	ON_LBN_SELCHANGE(IDC_LIST_OBJ_GROUND, &CObjTool::OnLbnSelchangeListObjGround)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OBJ_DELETEALL, &CObjTool::OnBnClickedButtonObjDeleteall)
	ON_CBN_SELCHANGE(IDC_COMBO_RENDERING_OPTION, &CObjTool::OnCbnSelchangeComboRenderingOption)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_ST_SAVE, &CObjTool::OnBnClickedButtonStSave)
	ON_BN_CLICKED(IDC_BUTTON_ST_LOAD, &CObjTool::OnBnClickedButtonStLoad)
	ON_BN_CLICKED(IDC_BUTTON_GR_SAVE, &CObjTool::OnBnClickedButtonGrSave)
	ON_BN_CLICKED(IDC_BUTTON_GR_LOAD, &CObjTool::OnBnClickedButtonGrLoad)
	ON_BN_CLICKED(IDC_BUTTON_ST_DELETESELECTED, &CObjTool::OnBnClickedButtonStDeleteselected)
	ON_BN_CLICKED(IDC_BUTTON_GR_DELETESELECTED, &CObjTool::OnBnClickedButtonGrDeleteselected)
	ON_LBN_SELCHANGE(IDC_LIST_ST_ADDED, &CObjTool::OnLbnSelchangeListStAdded)
	ON_LBN_SELCHANGE(IDC_LIST_GR_ADDED, &CObjTool::OnLbnSelchangeListGrAdded)
	ON_BN_CLICKED(IDC_CHECK_AGREETOCOL, &CObjTool::OnBnClickedCheckAgreetocol)
END_MESSAGE_MAP()


// CObjTool �޽��� ó�����Դϴ�.
///////////////////////////////////////////////////////////////////////////////////////
///////////////// INITIALIZE
///////////////////////////////////////////////////////////////////////////////////////
BOOL CObjTool::OnInitDialog()
{
	CDialog::OnInitDialog();



	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// �����̴�
	// 	m_slAngle.SetRange(0, 360); // ���� ����
	// 	m_slAngle.SetRangeMin(0); // �ּ� ����
	// 	m_slAngle.SetRangeMax(360); // �ִ� ����
	// 	m_slAngle.SetPos(0); // ��ġ ����
	// 	m_slAngle.SetTicFreq(30); // ** ���� ���� ����
	// 	m_slAngle.SetLineSize(5); //Ű�� ������ �� ���� ũ��
	// 	m_slAngle.SetPageSize(15); // pgUp, pgDnŰ�� ���콺�� ���� Ŭ���� ������ ũ��



	// �޺��ڽ�
	m_ComboRenderOption.AddString(L"World");
	m_ComboRenderOption.AddString(L"Forward");
	m_ComboRenderOption.AddString(L"Backward");
	m_ComboRenderOption.SetCurSel(m_iRenderOption);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CObjTool::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if(nState)
		g_IsMainView = false;
}



void CObjTool::HorizontalScroll()
{

}
///////////////////////////////////////////////////////////////////////////////////////
///////////////// FUNCTION
///////////////////////////////////////////////////////////////////////////////////////
void CObjTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	UpdateData(TRUE);

	int iFileIndex = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_STR] = L"";

	for(int i = 0; i < iFileIndex; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_STR);

		wstring wstrRe = RelativePath(szFullPath);

		CString strImageName;
		strImageName = PathFindFileName(szFullPath);

		PathRemoveExtension( (LPWSTR)strImageName.operator LPCWSTR() );


		map<CString, CImage*>::iterator iter;

		// �׶��� ��� ON�϶�
		if(m_bIsStMode == false)
		{
			iter = m_mapGroundImage.find(strImageName);
			if(iter == m_mapGroundImage.end() )
			{
				CImage* pImage = new CImage;
				pImage->Load( wstrRe.c_str() );
				m_mapGroundImage.insert( make_pair(strImageName, pImage));
			}
			m_ListBoxGround.AddString(strImageName);
		}

		// ��Ʈ���� ��� ON�� ��
		else if(m_bIsStMode == true)
		{
			iter = m_mapStructureImage.find(strImageName);
			if(iter == m_mapStructureImage.end() )
			{
				CImage* pImage = new CImage;
				pImage->Load( wstrRe.c_str() );
				m_mapStructureImage.insert( make_pair(strImageName, pImage));
			}
			m_ListBoxStructure.AddString(strImageName);
		}
	}

	CDialog::OnDropFiles(hDropInfo);
}

void CObjTool::OnBnClickedCheckGrSt()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_bIsStMode = !m_bIsStMode;

	if(m_bIsStMode == true)
		m_iObjID = 0;
	else
		m_iObjID = 1;
}



void CObjTool::SetAddedObjList()
{

	list<PMYOBJ>::iterator iter = m_pStructureList->begin();
	list<PMYOBJ>::iterator iter_end = m_pStructureList->end();

	for(; iter != iter_end; ++iter)
	{
		if( (*iter)->bIsAdded == false )
		{
			//			std::wstring wstIndex = L"";
			std::wstring wstName = L"";
			wchar_t wstIndex[MIN_STR];
			//wsprintf(szName, (*iter)->wstrName, (*iter)->iStIndex);
			/*(*iter)->wstrName += wostringstream((*iter)->iStIndex);*/

			_itow_s((*iter)->iStIndex, wstIndex, 10); //

			wstName = (*iter)->wstrName;
			wstName += wstIndex;

			m_ListBoxStAdded.AddString(wstName.c_str());
			(*iter)->bIsAdded = true;
		}
	}


	iter = m_pGroundList->begin();
	iter_end = m_pGroundList->end();

	for(; iter != iter_end; ++iter)
	{
		if((*iter)->bIsAdded == false)
		{
			std::wstring wstName = L"";
			wchar_t wstIndex[MIN_STR];

			_itow_s((*iter)->iGrIndex, wstIndex, 10);

			wstName = (*iter)->wstrName;
			wstName += wstIndex;

			m_ListBoxGrAdded.AddString(wstName.c_str());
			(*iter)->bIsAdded = true;
		}
	}


}


///////////////////////////////////////////////////////////////////////////////////////
///////////////// SELCHANGE
///////////////////////////////////////////////////////////////////////////////////////
void CObjTool::OnLbnSelchangeListObjStructure()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_bIsStMode = true;
	m_iObjID = 0;

	CString strSelectName;

	m_ListBoxStructure.GetText(m_ListBoxStructure.GetCurSel(), strSelectName);


	map<CString, CImage*>::iterator iter;
	iter = m_mapStructureImage.find(strSelectName);
	if(iter == m_mapStructureImage.end()) return;
	m_ObjPicture.SetBitmap( *iter->second );


	m_iCount = m_ListBoxStructure.GetCurSel();

	m_bIsSettingStructure = false;

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

}

void CObjTool::OnLbnSelchangeListObjGround()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_bIsStMode = false;
	m_iObjID = 1;

	CString strSelectName;

	m_ListBoxGround.GetText(m_ListBoxGround.GetCurSel(), strSelectName);


	map<CString, CImage*>::iterator iter;
	iter = m_mapGroundImage.find(strSelectName);
	if(iter == m_mapGroundImage.end()) return;
	m_ObjPicture.SetBitmap( *iter->second );

	m_iCount = m_ListBoxGround.GetCurSel();

}


void CObjTool::OnLbnSelchangeListStAdded()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// �ε��� �޾ƿ���
	CString		strSelectName;
	int iSelectNumber = m_ListBoxStAdded.GetCurSel();

	if(iSelectNumber < 0) return;

	m_ListBoxStAdded.GetText(iSelectNumber, strSelectName);


	int i = 0;
	for(i = 0; i < strSelectName.GetLength(); ++i)
	{
		if(isdigit(strSelectName[i]) != 0)
			break;
	}

	strSelectName.Delete(0, i);
	m_iSelectedIndex = _tstoi(strSelectName);

	// �ش� �ε����� �ش��ϴ� ������Ʈ �������ϰ� �����ϱ�
	// 0902 �������!
	list<PMYOBJ>::iterator iter = m_pStructureList->begin();
	list<PMYOBJ>::iterator iter_end = m_pStructureList->end();

	for(; iter != iter_end; ++iter)
	{
		if( (*iter)->iStIndex == m_iSelectedIndex )
		{
			(*iter)->bIsClicked = true;
		}
		else
			(*iter)->bIsClicked = false;
	}

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
}

void CObjTool::OnLbnSelchangeListGrAdded()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString		strSelectName;
	int iSelectNumber = m_ListBoxGrAdded.GetCurSel();

	if(iSelectNumber < 0) return;

	m_ListBoxGrAdded.GetText(iSelectNumber, strSelectName);


	int i = 0;
	
	for(i = 0; i < strSelectName.GetLength(); ++i)
	{
		if(isdigit(strSelectName[i]) != 0)
			break;
	}

	strSelectName.Delete(0, i);
	m_iSelectedIndex = _tstoi(strSelectName);

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
}

void CObjTool::OnCbnSelchangeComboRenderingOption()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iRenderOption = m_ComboRenderOption.GetCurSel();
}



///////////////////////////////////////////////////////////////////////////////////////
///////////////// SAVE LOAD
///////////////////////////////////////////////////////////////////////////////////////
void CObjTool::OnBnClickedButtonStSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<MYOBJ*>* pListSt =
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyObj()->GetStructureList();


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

	list<MYOBJ*>::iterator iter = pListSt->begin();
	list<MYOBJ*>::iterator iter_end = pListSt->end();

	for(; iter != iter_end; ++iter)
	{
		WriteFile(hFile, (*iter), sizeof(MYOBJ), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CObjTool::OnBnClickedButtonStLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	 	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyObj()
	 		->Release();

	//m_ListBoxStAdded.ResetContent();


	list<MYOBJ*>* pStList = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyObj()->GetStructureList();


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
		MYOBJ* pObj = new MYOBJ;

		ReadFile(hFile, pObj, sizeof(MYOBJ), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pObj);
			break;
		}

		pStList->push_back(pObj);
		//

		std::wstring wstName = L"";
		wchar_t wstIndex[MIN_STR];

		_itow_s(pObj->iStIndex, wstIndex, 10);

		wstName = pObj->wstrName;
		wstName += wstIndex;

		m_ListBoxStAdded.AddString(wstName.c_str());
		//

//		m_ListBoxStAdded.AddString((pObj->wstrName).c_str());
	}

	CloseHandle(hFile);

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
}

void CObjTool::OnBnClickedButtonGrSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	list<MYOBJ*>* pListGr =
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyObj()->GetGroundList();

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

	list<MYOBJ*>::iterator iter = pListGr->begin();
	list<MYOBJ*>::iterator iter_end = pListGr->end();

	for(; iter != iter_end; ++iter)
	{
		WriteFile(hFile, (*iter), sizeof(MYOBJ), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CObjTool::OnBnClickedButtonGrLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.//
	
// 	 ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetMyObj()
// 	 		->Release();

	m_ListBoxGrAdded.ResetContent();


	list<MYOBJ*>* pGrList = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyObj()->GetGroundList();


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
		MYOBJ* pObj = new MYOBJ;

		ReadFile(hFile, pObj, sizeof(MYOBJ), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pObj);
			break;
		}


			//			pObj->wstrName = L"Ground";
			pGrList->push_back(pObj);

			std::wstring wstName = L"";
			wchar_t wstIndex[MIN_STR];

			_itow_s(pObj->iGrIndex, wstIndex, 10);

			wstName = pObj->wstrName;
			wstName += wstIndex;

			m_ListBoxGrAdded.AddString(wstName.c_str());

//		m_ListBoxGrAdded.AddString((pObj->wstrName).c_str());
	}

	CloseHandle(hFile);

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////// DELETE
///////////////////////////////////////////////////////////////////////////////////////
void CObjTool::OnBnClickedButtonStDeleteselected()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<PMYOBJ>::iterator iter = m_pStructureList->begin();
	list<PMYOBJ>::iterator iter_end = m_pStructureList->end();

	for(; iter != iter_end;)
	{
		if( (*iter)->iStIndex == m_iSelectedIndex )
		{
			m_ListBoxStAdded.DeleteString(m_iSelectedIndex);
			Safe_Delete((*iter));
			iter = m_pStructureList->erase(iter);

			UpdateData(FALSE);
			((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
			((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
			return;
		}
		else
			++iter;
	}


}

void CObjTool::OnBnClickedButtonGrDeleteselected()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<PMYOBJ>::iterator iter = m_pGroundList->begin();
	list<PMYOBJ>::iterator iter_end = m_pGroundList->end();

	for(; iter != iter_end;)
	{
		if((*iter)->iStIndex == m_iSelectedIndex)
		{
			Safe_Delete((*iter));
			iter = m_pGroundList->erase(iter);
			return;
		}
		else
			++iter;
	}

	m_ListBoxGrAdded.DeleteString(m_iSelectedIndex);

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);
}

void CObjTool::OnDestroy()
{
	map<CString, CImage*>::iterator iter = m_mapGroundImage.begin();
	map<CString, CImage*>::iterator iter_end = m_mapGroundImage.end();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	for ( ; iter!= iter_end; ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_mapGroundImage.clear();

	//
	g_IsMainView = true;


	iter = m_mapStructureImage.begin();
	iter_end = m_mapStructureImage.end();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	for ( ; iter!= iter_end; ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_mapStructureImage.clear();

	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CObjTool::OnBnClickedButtonObjDeleteall()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<MYOBJ*>* pList = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyObj()->GetStructureList();

	list<PMYOBJ>::iterator iter = pList->begin();
	list<PMYOBJ>::iterator iter_end = pList->end();

	for(; iter != iter_end; )
	{
		Safe_Delete((*iter));
		iter = pList->erase(iter);
	}
	m_ListBoxStAdded.ResetContent();
	//

	pList = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetMyObj()->GetGroundList();

	iter = pList->begin();
	iter_end = pList->end();


	for(; iter != iter_end; )
	{
		Safe_Delete((*iter));
		iter = pList->erase(iter);
	}
	m_ListBoxGrAdded.ResetContent();
	//
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);


}


void CObjTool::Release()
{

}



void CObjTool::OnBnClickedCheckAgreetocol()
{
	if(m_bIsAgreeToDamage == false)
		m_bIsAgreeToDamage = true;
	else
		m_bIsAgreeToDamage = false;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
