// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TOOL.h"
#include "PathFind.h"
#include "FileInfo.h"


// CPathFind 대화 상자입니다.

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


// CPathFind 메시지 처리기입니다.

void CPathFind::OnBnClickedButtonPathfindload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		//라인을 불러온다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// C++ 입출력 객체
	//#include <fstream>
	wofstream SaveFile;

	SaveFile.open(L"../Data/MultiImagePath.txt", ios::out );
	//파일 개방.


	int iCount = 0;

	for(list<IMGPATH*>::iterator iter = m_ListPathInfo.begin();
		iter != m_ListPathInfo.end(); ++iter)
	{
		++iCount;

		//파일 쓰기.
		//cout과 비슷한 형태로 사용이 가능하다.
		//cout << 123123 << endl;

		SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
		SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
		SaveFile << (*iter)->iCount << L"|";


		if(iCount == m_ListPathInfo.size() )
		{
			//출력버퍼를 비워준다.
			//모든 파일을 읽었을때
			SaveFile << (*iter)->wstrPath.c_str() << flush;
		}
		else
			SaveFile << (*iter)->wstrPath.c_str() << endl;

	}

	SaveFile.close();

	//클라이언트에서 특정 파일을 실행시킬수 있는 함수.
	WinExec("notepad.exe ../Data/MultiImagePath.txt", SW_SHOW);
}

void CPathFind::OnLbnSelchangeListBoxPathFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
			//iter에 있는 숫자를 10진수 문자로
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
