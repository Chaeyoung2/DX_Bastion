// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TOOL.h"
#include "PathFind.h"
#include "FileInfo.h"


// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(CPathFind::IDD, pParent)
{

}

CPathFind::~CPathFind()
{
	Release();
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PATHFIND, m_ListBoxPathFind);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PATHFINDLOAD, &CPathFind::OnBnClickedButtonPathfindload)
	ON_BN_CLICKED(IDC_BUTTON_PATHFINDSAVE, &CPathFind::OnBnClickedButtonPathfindsave)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnLbnSelchangeListBoxPathFind)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind �޽��� ó�����Դϴ�.

void CPathFind::OnBnClickedButtonPathfindload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	wifstream LoadFile;


	LoadFile.open(L"../Data/MultiImagePath.txt", ios::in );


	TCHAR szObjKey[MAX_STR]		= L"";
	TCHAR szStateKey[MAX_STR]	= L"";
	TCHAR szCount[MAX_STR]		= L"";
	TCHAR szPaht[MAX_STR]		= L"";


	wstring wstrCombine = L"";


	m_ListBoxPathFind.ResetContent();


	while( !LoadFile.eof() )
	{
		//������ �ҷ��´�.
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szPaht, MAX_PATH);

		wstrCombine = wstring(szObjKey) + L"|";
		wstrCombine += wstring(szStateKey) + L"|";
		wstrCombine += wstring(szCount) + L"|";
		wstrCombine += wstring(szPaht);

		m_ListBoxPathFind.AddString(wstrCombine.c_str());
	}

	LoadFile.close();

	UpdateData(FALSE);
}

void CPathFind::OnBnClickedButtonPathfindsave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// C++ ����� ��ü
	//#include <fstream>
	wofstream SaveFile;

	SaveFile.open(L"../Data/MultiImagePath.txt", ios::out );
	//���� ����.


	int iCount = 0;

	for(list<IMGPATH*>::iterator iter = m_ListPathInfo.begin();
		iter != m_ListPathInfo.end(); ++iter)
	{
		++iCount;

		//���� ����.
		//cout�� ����� ���·� ����� �����ϴ�.
		//cout << 123123 << endl;

		SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
		SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
		SaveFile << (*iter)->iCount << L"|";


		if(iCount == m_ListPathInfo.size() )
		{
			//��¹��۸� ����ش�.
			//��� ������ �о�����
			SaveFile << (*iter)->wstrPath.c_str() << flush;
		}
		else
			SaveFile << (*iter)->wstrPath.c_str() << endl;

	}

	SaveFile.close();

	//Ŭ���̾�Ʈ���� Ư�� ������ �����ų�� �ִ� �Լ�.
	WinExec("notepad.exe ../Data/MultiImagePath.txt", SW_SHOW);
}

void CPathFind::OnLbnSelchangeListBoxPathFind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	int iFileIndex = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";

	for(int i = 0; i < iFileIndex; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

		CFileInfo		FileInfo;

		FileInfo.DirInfoExtraction(szFullPath, m_ListPathInfo);



		wstring wstrCombie = L"";
		TCHAR szBuff[MIN_STR] = L"";

		for(list<IMGPATH*>::iterator iter = m_ListPathInfo.begin();
			iter != m_ListPathInfo.end(); ++iter)
		{
			//iter�� �ִ� ���ڸ� 10���� ���ڷ�
			_itow_s( (*iter)->iCount, szBuff, 10);

			wstrCombie = (*iter)->wstrObjKey + L"|" + (*iter)->wstrStateKey + L"|";

			wstrCombie += szBuff;
			wstrCombie += L"|" + (*iter)->wstrPath;

			m_ListBoxPathFind.AddString(wstrCombie.c_str());
		}
	}

	UpdateData(FALSE);

	CDialog::OnDropFiles(hDropInfo);
}

void CPathFind::Release()
{
	for(list<IMGPATH*>::iterator iter = m_ListPathInfo.begin();
		iter != m_ListPathInfo.end(); ++iter)
	{
		Safe_Delete(*iter);
	}
	m_ListPathInfo.clear();
}
