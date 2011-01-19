// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "gsl.h"

using namespace gsl;
using namespace std;

void ShowMessage( HWND hWnd, const TCHAR *szValName, int nValue, UINT nType = MB_OK );
void ShowMessage( HWND hWnd, const TCHAR *szValName, UINT nType = MB_OK );

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);

DWORD GetSysPageSize(void);
DWORD LS1B(DWORD dwNum);
void *AddrAlignToBytes(void *src, DWORD dwAlignByte);


#define SHOWMESSAGE_DECLEAR \
void ShowMessage( const TCHAR *szValName, int nValue, UINT nType = MB_OK );

#define SHOWMESSAGE_IMP(ClassName) \
void ClassName::ShowMessage( const TCHAR *szValName, int nValue, UINT nType ) \
{ \
	TCHAR szText[512]; \
	if (szValName != NULL) \
		_stprintf_s(szText, _countof(szText), _T("%s = %d."), szValName, nValue); \
	else \
		_stprintf_s(szText, _countof(szText), _T("Var = %d."), nValue); \
	::MessageBox(GetSafeHwnd(), szText, _T("Variable's Value"), nType); \
}


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


