#pragma once

// CMyForm �� ���Դϴ�.

#include "UnitTool.h"
#include "ObjTool.h"
#include "MapTool.h"
//#include "PopUp.h"
#include "PathFind.h"
#include "afxwin.h"


class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

private:
	CUnitTool		m_UnitTool;
	CMapTool		m_MapTool;
	CObjTool		m_ObjTool;
	CPathFind		m_PathFind;

public:
	CMapTool*		GetMapTool() { return &m_MapTool;}
	CUnitTool*		GetUnitTool() { return &m_UnitTool; }
	CObjTool*		GetObjTool() { return &m_ObjTool; }

private:
	CFont			m_Font;

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMaptool();
	afx_msg void OnBnClickedButtonObjtool();
	afx_msg void OnBnClickedButtonUnittool();
	afx_msg void OnBnClickedButtonPathfind();
	afx_msg void OnCbnSelchangeComboStage();
	virtual void OnInitialUpdate();
	CComboBox m_ComboListStage;
	int m_iStage;
};


