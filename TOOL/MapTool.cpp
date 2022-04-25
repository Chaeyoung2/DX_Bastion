// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TOOL.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "TOOLView.h"
#include "BackGround.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
	, m_iDrawID(0)
	, m_iOption(0)	
	, m_bRenderOp(TRUE)
	, m_bRenderBor(TRUE)
	, m_bRenderIndex(TRUE)
	, m_fHeight(0), m_bIsSetHeight(FALSE)
	, m_fMoveSpeed(0)
{					

}

CMapTool::~CMapTool()
{
	Release();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MAPOPTION, m_ComboOptionList);
	DDX_Control(pDX, IDC_LIST_TILE, m_ListBoxTilePath);
	DDX_Control(pDX, IDC_TILE_PICTURE, m_TilePicture);
	DDX_CBIndex(pDX, IDC_COMBO_MAPOPTION, m_iOption);
	DDX_Check(pDX, IDC_CHECK_Render_Option, m_bRenderOp);
	DDX_Check(pDX, IDC_CHECK_Render_Border, m_bRenderBor);
	DDX_Check(pDX, IDC_CHECK_Render_Index, m_bRenderIndex);
	DDX_Control(pDX, IDC_CHECK_Render_Border, m_CheckRenderBor);
	DDX_Control(pDX, IDC_CHECK_Render_Option, m_CheckRenderOp);
	DDX_Control(pDX, IDC_CHECK_Render_Index, m_CheckRenderIndex);
	DDX_Control(pDX, IDC_SLIDER_HEIGHT, m_slHeight);
	DDX_Control(pDX, IDC_EDIT_MAP_HEIGHT, m_editHeight);
	DDX_Text(pDX, IDC_EDIT_MAP_HEIGHT, m_fHeight);
	DDV_MinMaxFloat(pDX, m_fHeight, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MAP_SPEED, m_fMoveSpeed);
	DDX_Control(pDX, IDC_EDIT_MAP_SPEED, m_EditMoveSpeed);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_slMoveSpeed);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_TILE, &CMapTool::OnLbnSelchangeListTile)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_MAPSAVE, &CMapTool::OnBnClickedButtonMapsave)
	ON_BN_CLICKED(IDC_BUTTON_MAPLOAD, &CMapTool::OnBnClickedButtonMapload)
	ON_BN_CLICKED(IDC_BUTTON_MAPDELETEALL, &CMapTool::OnBnClickedButtonMapdeleteall)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_MAPOPTION, &CMapTool::OnCbnSelchangeComboMapoption)
	ON_BN_CLICKED(IDC_CHECK_Render_Border, &CMapTool::OnBnClickedCheckRenderBorder)
	ON_BN_CLICKED(IDC_CHECK_Render_Option, &CMapTool::OnBnClickedCheckRenderOption)
	ON_BN_CLICKED(IDC_CHECK_Render_Index, &CMapTool::OnBnClickedCheckRenderIndex)
	ON_WM_ACTIVATE()
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_HEIGHT, &CMapTool::OnTRBNThumbPosChangingSliderHeight)
	ON_WM_HSCROLL()
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_SPEED, &CMapTool::OnTRBNThumbPosChangingSliderSpeed)
END_MESSAGE_MAP()

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	


	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// �����̴�
	// Height
	m_slHeight.SetRange(0, 100); // ���� ����
	m_slHeight.SetRangeMin(0); // �ּ� ����
	m_slHeight.SetRangeMax(100); // �ִ� ����
	m_slHeight.SetPos(0); // ��ġ ����
	m_slHeight.SetTicFreq(10); // ** ���� ���� ����
	m_slHeight.SetLineSize(1); //Ű�� ������ �� ���� ũ��
	m_slHeight.SetPageSize(5); // pgUp, pgDnŰ�� ���콺�� ���� Ŭ���� ������ ũ��

	// Height
	m_slMoveSpeed.SetRange(10, 100); // ���� ����
	m_slMoveSpeed.SetRangeMin(10); // �ּ� ����
	m_slMoveSpeed.SetRangeMax(100); // �ִ� ����
	m_slMoveSpeed.SetPos(10); // ��ġ ����
	m_slMoveSpeed.SetTicFreq(10); // ** ���� ���� ����
	m_slMoveSpeed.SetLineSize(5); //Ű�� ������ �� ���� ũ��
	m_slMoveSpeed.SetPageSize(10); // pgUp, pgDnŰ�� ���콺�� ���� Ŭ���� ������ ũ��




	// �޺��ڽ�
	m_ComboOptionList.AddString(L"0. �̵� �Ұ�");
	m_ComboOptionList.AddString(L"1. �̵� ����");
	m_ComboOptionList.AddString(L"2. �߶� ����");
	m_ComboOptionList.AddString(L"3. ��� Ÿ��");
	m_ComboOptionList.AddString(L"4. �ϰ� Ÿ��");
	m_ComboOptionList.AddString(L"5. �ڸ� Ÿ��");

	m_ComboOptionList.SetCurSel(m_iOption);

	// üũ�ڽ�
	m_CheckRenderBor.SetCheck(m_bRenderBor);
	m_CheckRenderIndex.SetCheck(m_bRenderIndex);
	m_CheckRenderOp.SetCheck(m_bRenderOp);

	return TRUE;  // return TRUE unless you set the focus to a controlr
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



// CMapTool �޽��� ó�����Դϴ�.

void CMapTool::OnLbnSelchangeListTile()
{
	UpdateData(TRUE);

	CString strSelectName;

	m_ListBoxTilePath.GetText( m_ListBoxTilePath.GetCurSel(),
		strSelectName);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strSelectName);

	if(iter == m_mapPngImage.end()) return;

	m_TilePicture.SetBitmap( *iter->second );



	int i = 0;
	for(i = 0; i < strSelectName.GetLength(); ++i)
	{
		// ���ڿ��� 0�� �ε������� �˻� : �������� ��������.
		// 0�� �ƴ� ���� ��ȯ�ϸ� ������.
		if( isdigit(strSelectName[i]) != 0)
		{
			break;
		}
	}

	strSelectName.Delete(0, i);
	// 0�� �ε������� ���� �ε��� �ձ����� ���ڿ� ����.

	m_iDrawID = _tstoi(strSelectName);

	UpdateData(FALSE);

	//
	// m_TilePicture.MoveWindow()




	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	int iFileIndex = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_STR] = L"";


	for(int i = 0; i < iFileIndex; ++i)
	{
		// ����Ʈ �ڽ��� ���� �ֱ�.
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_STR);

		wstring wstrRe = RelativePath(szFullPath);

		CString strImageName;

		strImageName = PathFindFileName(szFullPath);


		PathRemoveExtension( (LPWSTR)strImageName.operator LPCWSTR() );

		// �� �����̳�
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if(iter == m_mapPngImage.end() )
		{
			CImage* pImage = new CImage;

			pImage->Load( wstrRe.c_str() );

			m_mapPngImage.insert( make_pair(strImageName, pImage));
		}

		m_ListBoxTilePath.AddString(strImageName);
	}

	CDialog::OnDropFiles(hDropInfo);

	HorizontalScroll();

	UpdateData(FALSE);

}

void CMapTool::OnBnClickedButtonMapsave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	vector<TILE*>* pVecTile =
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()
		->GetTile();

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL) return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";

	DWORD dwByte = 0;

	HANDLE hTileFile = CreateFile(Dlg.GetPathName(),
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		WriteFile(hTileFile, (*pVecTile)[i], sizeof(TILE), &dwByte, NULL);
	}
	CloseHandle(hTileFile);

}

void CMapTool::OnBnClickedButtonMapload()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()
		->Release();

	vector<TILE*>* pVecTile = ((CMainFrame*)AfxGetMainWnd())
		->GetMainView()->GetBackGround()->GetTile();

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL) return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	while(true)
	{
		PTILE pTile = new TILE;


		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pTile);
			break;
		}
		pVecTile->push_back(pTile);
	}

	CloseHandle(hFile);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate();

}

void CMapTool::OnBnClickedButtonMapdeleteall()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	vector<TILE*>* pVecTile =
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()
		->GetTile();

	vector<TILE*>::iterator iter = pVecTile->begin();
	vector<TILE*>::iterator iter_end = pVecTile->end();


	for(; iter != iter_end; ++iter)
	{
		(*iter)->byDrawId = 123;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
// 	for(size_t i = 0; i < pVecTile->size(); ++i)
// 	{
// 		pVecTile[i]->byDrawId = 123;
	

}



void CMapTool::OnDestroy()
{
	map<CString, CImage*>::iterator iter = m_mapPngImage.begin();
	map<CString, CImage*>::iterator iter_end = m_mapPngImage.end();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	for ( ; iter!= iter_end; ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_mapPngImage.clear();

	//
	g_IsMainView = true;

	// �߰�
	CDialog::OnDestroy();


	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMapTool::HorizontalScroll()
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_ListBoxTilePath.GetDC();

	for(int i = 0; i < m_ListBoxTilePath.GetCount(); ++i)
	{
		m_ListBoxTilePath.GetText(i, strName);

		Size = pDc->GetTextExtent(strName);

		if(Size.cx > iCX)
			iCX = Size.cx;
	}

	m_ListBoxTilePath.ReleaseDC(pDc);

	if(m_ListBoxTilePath.GetHorizontalExtent() < iCX)
	{
		m_ListBoxTilePath.SetHorizontalExtent(iCX);
		// ���� �� ���ڿ� ũ�⸸ŭ ��ũ�� �� ����
	}
}

void CMapTool::Release()
{
	for( map<CString, CImage*>::iterator iter = m_mapPngImage.begin();
		iter != m_mapPngImage.end(); ++iter)
	{
		iter->second->Destroy();
		Safe_Delete(iter->second);
	}
	m_mapPngImage.clear();
}

void CMapTool::OnCbnSelchangeComboMapoption()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_iOption = m_ComboOptionList.GetCurSel();	

}

void CMapTool::OnBnClickedCheckRenderBorder()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bRenderBor = !m_bRenderBor;
	m_pBackGround->SetRenderBor(m_bRenderBor);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);

}

void CMapTool::OnBnClickedCheckRenderOption()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bRenderOp = !m_bRenderOp;
	m_pBackGround->SetRenderOp(m_bRenderOp);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}

void CMapTool::OnBnClickedCheckRenderIndex()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bRenderIndex = !m_bRenderIndex;
	m_pBackGround->SetRenderIndex(m_bRenderIndex);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}

void CMapTool::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(nState)
		g_IsMainView = false;
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMapTool::OnTRBNThumbPosChangingSliderHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	m_fHeight = (float)m_slHeight.GetPos(); // ���� ��ġ�� �˾Ƴ���.

}

void CMapTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	UpdateData(TRUE);

	if(pScrollBar == NULL)
		return;

	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_slHeight) // ���� ��ũ�ѹٶ��
		{
			m_fHeight = (float)m_slHeight.GetPos();
		}

		else if(pScrollBar == (CScrollBar*)&m_slMoveSpeed) // ���� ��ũ�ѹٶ��
		{
			m_fMoveSpeed= (float)m_slMoveSpeed.GetPos();
		}
	}
// 
// 	else
// 	{
// 
// 	}
	UpdateData(FALSE);


}

void CMapTool::OnTRBNThumbPosChangingSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}
