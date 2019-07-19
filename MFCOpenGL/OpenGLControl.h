#pragma once
#include "afxwin.h"
#include "ObjectModel.h"

class COpenGLControl : public CWnd
{

public:
	COpenGLControl(void);
	UINT_PTR m_unpTimer;
	void oglCreate(CRect rect, CWnd *parent);
	void oglInitialize(void);
	void oglDrawScene(void);

	bool m_bIsMaximized;

private:

	CWnd    *hWnd;
	HDC     hdc;
	CDC*    m_pDC;
	HGLRC   hrc;
	int     m_nPixelFormat;
	CRect   m_rect;
	CRect   m_oldWindow;
	CRect   m_originalRect;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDraw(CDC *pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	BOOL GetOldStyleRenderingContext();


private:
	float m_fPosX;    // X position of model in camera view
	float m_fPosY;    // Y position of model in camera view
	float m_fZoom;   // Zoom on model in camera view
	float m_fRotX;    // Rotation on model in camera view
	float m_fRotY;    // Rotation on model in camera view
	float m_fLastX;
	float m_fLastY;
	ObjectModel* model;

	GLuint vertexbuffer;

	GLuint uvbuffer;
};

