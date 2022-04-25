
// TOOLView.h : CTOOLView Ŭ������ �������̽�
//


#pragma once

class CMyObj;
class CMyUnit;
class CSingleTexture;
class CBackGround;
class CTOOLDoc;
/*class CTOOLView : public CView*/
class CTOOLView : public CScrollView
{
private:
	CSingleTexture*		m_pSingleTexture;
	CBackGround*		m_pBackGround;
	CMyUnit*			m_pMyUnit;
	CMyObj*				m_pMyObj;

	bool				m_bIsLButtonPressed;

public:
	CBackGround* GetBackGround() { return m_pBackGround;  }
	CMyUnit*	 GetMyUnit()	 { return m_pMyUnit; }
	CMyObj*		 GetMyObj()		 { return m_pMyObj; }

protected: // serialization������ ��������ϴ�.
	CTOOLView();
	DECLARE_DYNCREATE(CTOOLView)

// Ư���Դϴ�.
public:
	CTOOLDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTOOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

#ifndef _DEBUG  // TOOLView.cpp�� ����� ����
inline CTOOLDoc* CTOOLView::GetDocument() const
   { return reinterpret_cast<CTOOLDoc*>(m_pDocument); }
#endif

