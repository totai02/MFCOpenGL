
// MFCOpenGLDlg.h : header file
//

#pragma once
#include "OpenGLControl.h"
#include "afxwin.h"

// CMFCOpenGLDlg dialog
class CMFCOpenGLDlg : public CDialogEx
{
// Construction
public:
	CMFCOpenGLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCOPENGL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void initComponents();

// Implementation
protected:
	HICON m_hIcon;
	COpenGLControl m_oglWindow;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMCustomdrawSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
