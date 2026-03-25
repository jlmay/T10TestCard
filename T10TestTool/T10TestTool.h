// T10TestTool.h : main header file for the T10TestTool application
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CT10TestToolApp : public CWinApp
{
public:
	CT10TestToolApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CT10TestToolApp theApp;
