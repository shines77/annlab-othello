// AnnLabApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "../../res/gui/resource.h"		// ������


// CAnnLabApp:
// �йش����ʵ�֣������ AnnLabApp.cpp
//

class CAnnLabApp : public CWinApp
{
public:
	CAnnLabApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAnnLabApp theApp;
