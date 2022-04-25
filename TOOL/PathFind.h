#pragma once


// CPathFind ��ȭ �����Դϴ�.

#include "Include.h"
#include "afxwin.h"

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

private:
	list<IMGPATH*>		m_ListPathInfo;

public:
	void Release();

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPathfindload();
	afx_msg void OnBnClickedButtonPathfindsave();
	CListBox m_ListBoxPathFind;
	afx_msg void OnLbnSelchangeListBoxPathFind();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
