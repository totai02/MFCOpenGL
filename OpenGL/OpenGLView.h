// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// OpenGLView.h : interface of the COpenGLView class
//

#pragma once

const UINT IDC_BUTTON1 = 10001;

class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

	// Attributes
public:
	COpenGLDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL GetOldStyleRenderingContext( void );
	virtual BOOL SetupPixelFormat( void );

private:
	//OpenGL Setup
	BOOL GetRenderingContext();
	//Rendering Context and Device Context Pointers
	HGLRC     m_hRC;
	CDC*      m_pDC;

	//Error Handling
	void SetError( int e );
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;
	CFrameWnd* m_frame;

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

#ifndef _DEBUG  // debug version in OpenGLView.cpp
inline COpenGLDoc* COpenGLView::GetDocument() const
{ return reinterpret_cast<COpenGLDoc*>(m_pDocument); }
#endif

