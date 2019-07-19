
// MFCOpenGLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCOpenGL.h"
#include "MFCOpenGLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCOpenGLDlg dialog



CMFCOpenGLDlg::CMFCOpenGLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCOpenGLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCOpenGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCOpenGLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCOpenGLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCOpenGLDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCOpenGLDlg message handlers

BOOL CMFCOpenGLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	initComponents();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCOpenGLDlg::initComponents()
{
	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL_WINDOW)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);

	// Fonts
	CFont* font = new CFont();
	CFont* headerFont = new CFont();
	CFont* groupFont = new CFont();

	font->CreateFont(
		28,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Comic Sans MS"));  

	headerFont->CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Comic Sans MS"));  

	groupFont->CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Comic Sans MS"));  

	GetDlgItem(IDC_STATIC_TEXT)->SetFont(font, TRUE);

	GetDlgItem(IDC_STATIC_FILE)->SetFont(groupFont, TRUE);
	GetDlgItem(IDC_STATIC_TRANSFORM)->SetFont(groupFont, TRUE);
	GetDlgItem(IDC_STATIC_ANIMATION)->SetFont(groupFont, TRUE);

	GetDlgItem(IDC_STATIC_POSITION)->SetFont(headerFont, TRUE);
	GetDlgItem(IDC_STATIC_ROTATION)->SetFont(headerFont, TRUE);
	GetDlgItem(IDC_STATIC_SCALE)->SetFont(headerFont, TRUE);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCOpenGLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCOpenGLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CMFCOpenGLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT) {
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode( TRANSPARENT );
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	else 
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


void CMFCOpenGLDlg::OnBnClickedButton1()
{
	const TCHAR szFilter[] = _T("OBJ Files (*.obj)|*.obj|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("obj"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);    
	if(dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		ObjectModel model;
		model.LoadModel((char*)(LPCTSTR)sFilePath);
	}
}


void CMFCOpenGLDlg::OnBnClickedButton2()
{
	const TCHAR szFilter[] = _T("OBJ Files (*.obj)|*.obj|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("obj"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);    
	if(dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
	}
}
