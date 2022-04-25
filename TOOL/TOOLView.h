
// TOOLView.h : CTOOLView 클래스의 인터페이스
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

protected: // serialization에서만 만들어집니다.
	CTOOLView();
	DECLARE_DYNCREATE(CTOOLView)

// 특성입니다.
public:
	CTOOLDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTOOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // TOOLView.cpp의 디버그 버전
inline CTOOLDoc* CTOOLView::GetDocument() const
   { return reinterpret_cast<CTOOLDoc*>(m_pDocument); }
#endif

