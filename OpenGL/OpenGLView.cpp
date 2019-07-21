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

// OpenGLView.cpp : implementation of the COpenGLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

	BEGIN_MESSAGE_MAP(COpenGLView, CView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_SIZE()
	END_MESSAGE_MAP()

	// You will add stuff here!!!!
	const char* const COpenGLView::_ErrorStrings[]= {
		{"No Error"},                     // 0
		{"Unable to get a DC"},           // 1
		{"ChoosePixelFormat failed"},     // 2
		{"SelectPixelFormat failed"},     // 3
		{"wglCreateContext failed"},      // 4
		{"wglMakeCurrent failed"},        // 5
		{"wglDeleteContext failed"},      // 6
		{"SwapBuffers failed"},           // 7
	};

	// COpenGLView construction/destruction

	COpenGLView::COpenGLView() : m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0]), m_frame(NULL)
	{
		// TODO: add construction code here
		m_frame = new CFrameWnd();
	}

	COpenGLView::~COpenGLView()
	{

	}

	BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		return CView::PreCreateWindow(cs);
	}

	// COpenGLView drawing

	void COpenGLView::OnDraw(CDC* /*pDC*/)
	{
		COpenGLDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here
		//Swap buffers to show result
		if ( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc() ) )
		{
			SetError(7);
		}
	}

	void COpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void COpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// COpenGLView diagnostics

#ifdef _DEBUG
	void COpenGLView::AssertValid() const
	{
		CView::AssertValid();
	}

	void COpenGLView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	COpenGLDoc* COpenGLView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
		return (COpenGLDoc*)m_pDocument;
	}
#endif //_DEBUG


	// COpenGLView message handlers
	template <typename T>
	std::string NumberToString ( T Number )
	{
		std::ostringstream ss;
		ss << Number;
		return ss.str();
	}

	int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here
		if (!GetRenderingContext())
		{
			//Something went wrong with getting the rendering context.
			//Create a popup with the error message, then quit.
			AfxMessageBox(CString(m_ErrorString));
			return -1;
		}

		CRect rect;
		this->GetParent()->GetWindowRect(rect);
		ScreenToClient(rect);
		m_frame->Create(NULL, _T("OpenGL View"), WS_CHILD, rect, this);
		m_frame->ShowWindow(SW_SHOW);

		return 0;
	}


	void COpenGLView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: Add your message handler code here
		if ( FALSE == ::wglDeleteContext( m_hRC ) )
		{
			SetError(6);
		}

		if ( m_pDC )
		{
			delete m_pDC;
		}
	}


	void COpenGLView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_frame->SetWindowPos(NULL, 0, 0, cx + 10, cy + 55, SWP_NOMOVE);
	}
	void COpenGLView::SetError( int e )
	{
		// if there was no previous error,
		// then save this one
		if ( _ErrorStrings[0] == m_ErrorString )
		{
			m_ErrorString = _ErrorStrings[e];
		}
	}

	BOOL COpenGLView::GetRenderingContext()
	{
		// Can we put this in the constructor?
		m_pDC = new CClientDC(this);

		if ( NULL == m_pDC ) // failure to get DC
		{
			SetError(1);
			return FALSE;
		}


		if (!GetOldStyleRenderingContext())
		{
			return TRUE;
		}

		//Get access to modern OpenGL functionality from this old style context.
		glewExperimental = GL_TRUE;
		if (GLEW_OK != glewInit())
		{
			AfxMessageBox(_T("GLEW could not be initialized!"));
			return FALSE;
		}

		//Get a new style pixel format
		if (!SetupPixelFormat())
		{
			return FALSE;
		}

		//Setup request for OpenGL 3.2 Core Profile
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,   3,
			WGL_CONTEXT_MINOR_VERSION_ARB,   2,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 
			//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0, 0  //End
		};

		if(wglewIsSupported("WGL_ARB_create_context") == 1)
		{
			//If this driver supports new style rendering contexts, create one
			HGLRC oldContext = m_hRC;
			if ( 0 == (m_hRC = m_hRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(),0, attribs) ) )
			{
				SetError(4);
				return FALSE;
			}

			if(!wglMakeCurrent(NULL,NULL) )
				wglDeleteContext(oldContext);
			if ( FALSE == wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ) )
			{
				SetError(5);
				return FALSE;
			}     
		}
		else
		{  
			//Otherwise use the old style rendering context we created earlier.
			AfxMessageBox(_T("GL 3.2 Context not possible. Using old style context. (GL 2.1 and before)"));
		}

		return TRUE;
	}

	BOOL COpenGLView::GetOldStyleRenderingContext()
	{
		//A generic pixel format descriptor. This will be replaced with a more
		//specific and modern one later, so don't worry about it too much.
		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |            // support window
			PFD_SUPPORT_OPENGL |            // support OpenGL
			PFD_DOUBLEBUFFER,               // double buffered
			PFD_TYPE_RGBA,                  // RGBA type
			32,                             // 32-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			24,                        // 24-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
		};

		// Get the id number for the best match supported by the hardware device context
		// to what is described in pfd
		int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

		//If there's no match, report an error
		if ( 0 == pixelFormat )
		{
			SetError(2);
			return FALSE;
		}

		//If there is an acceptable match, set it as the current 
		if ( FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd) )
		{
			SetError(3);
			return FALSE;
		}

		//Create a context with this pixel format
		if ( 0 == (m_hRC = wglCreateContext( m_pDC->GetSafeHdc() ) ) )
		{
			SetError(4);
			return FALSE;
		}

		//Make it current. Now we're ready to get extended features.
		if ( FALSE == wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ) )
		{
			SetError(5);
			return FALSE;
		}
		return TRUE;
	}

	BOOL COpenGLView::SetupPixelFormat()
	{
		//This is a modern pixel format attribute list.
		//It has an extensible structure. Just add in more argument pairs 
		//befroe the null to request more features.
		const int attribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,     32,
			WGL_DEPTH_BITS_ARB,     24,
			WGL_STENCIL_BITS_ARB,   8,
			0, 0  //End
		};


		unsigned int numFormats;
		int pixelFormat;
		PIXELFORMATDESCRIPTOR pfd;

		//Select a pixel format number
		wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);

		//Optional: Get the pixel format's description. We must provide a 
		//description to SetPixelFormat(), but its contents mean little.
		//According to MSDN: 
		//  The system's metafile component uses this structure to record the logical
		//  pixel format specification. The structure has no other effect upon the
		//  behavior of the SetPixelFormat function.
		//DescribePixelFormat(m_pDC->GetSafeHdc(), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		//Set it as the current 
		if ( FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd) )
		{
			SetError(3);
			return FALSE;
		}

		return TRUE;
	}

