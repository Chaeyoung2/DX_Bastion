#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

#include "Include.h"


class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

private:
	map<CString, PMYUNIT>		m_mapUnitData;
	map<CString, CImage*>		m_mapUnitImage;
	map<CString, CImage*>		m_mapUnitImage_Ex; // ������ ������

	bool	m_bRadioButtonStartCheck;


public:
	int				m_iUnitID; // 0�÷��̾�, 1�������, 2���ý���, 3����
	int				m_iUnitDir;
	

	// MyUnit
public:
	void			SetUnitList(list<PMYUNIT>* pUnitList) { m_pUnitList = pUnitList; }
	TCHAR*			wstrName;
	list<PMYUNIT>*	m_pUnitList;



public:
	void Release();
	void HorizontalScroll();
	void SetAddedUnitList();

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UNITTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// // ������ ���� ����Ʈ
	CComboBox m_ComboDirList;


	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListUnitAdded();
	afx_msg void OnCbnSelchangeComboUnitDir();
	// ���� ����
	CString m_strComboUnitDir;
	CListBox m_ListBoxUnit;
	afx_msg void OnBnClickedButtonUnitAdd();
	// �߰��� ���� ����Ʈ
	CListBox m_ListBoxUnitAdded;
	CStatic m_UnitPicture;
	afx_msg void OnLbnSelchangeListUnit();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonUnitSave();
	afx_msg void OnBnClickedButtonUnitLoad();
	afx_msg void OnBnClickedButtonUnitDeleteall();
	afx_msg void OnBnClickedButtonUnitDeleteselected();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
