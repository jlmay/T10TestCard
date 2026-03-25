// T10TestTool.cpp : Defines the class behaviors for the application.
#include "stdafx.h"
#include "T10TestTool.h"
#include "T10TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CT10TestToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CT10TestToolApp::CT10TestToolApp()
{
}

CT10TestToolApp theApp;

BOOL CT10TestToolApp::InitInstance()
{
	CWinApp::InitInstance();

	CT10TestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	// application, rather than start the application's message pump.
	return FALSE;
}
