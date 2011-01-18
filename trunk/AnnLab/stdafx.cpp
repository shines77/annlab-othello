// stdafx.cpp : 只包括标准包含文件的源文件
// AnnLab.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#define DEFAULT_PAGESIZE        4096

void ShowMessage( HWND hWnd, const TCHAR *szValName, int nValue, UINT nType )
{
	TCHAR szText[512];
	if (szValName != NULL)
		_stprintf_s(szText, _countof(szText), _T("%s = %d."), szValName, nValue);
	else
		_stprintf_s(szText, _countof(szText), _T("Var = %d."), nValue);
	::MessageBox(hWnd, szText, _T("Variable's Value"), nType);

}

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
	TRACE("in filter.\n");
	if (code == EXCEPTION_ACCESS_VIOLATION) {
		TRACE("caught AV as expected.\n");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else {
		TRACE("didn't catch AV, unexpected.\n");
		return EXCEPTION_CONTINUE_SEARCH;
	};
}

DWORD GetSysPageSize(void)
{
	DWORD dwPageSize;

	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	dwPageSize = siSysInfo.dwPageSize;

	if (dwPageSize == 0)
		dwPageSize = DEFAULT_PAGESIZE;

	return dwPageSize;
}

// LS1B: BitScanForward
DWORD LS1B(DWORD dwNum)
{
	DWORD dwLS1B;
#if 0
	dwLS1B = 1;
	while (dwLS1B <= dwNum) {
		if (dwLS1B * 2 <= dwNum)
			dwLS1B <<= 1;
		else
			break;
	}
#else
	dwLS1B = dwNum;
	dwLS1B &= -(int)dwLS1B; // LS1B-Isolation
#endif
	if (!(!(dwLS1B & (dwLS1B - 1)) && dwLS1B)) {
		dwLS1B = 1;
		while (dwLS1B <= dwNum) {
			if (dwLS1B * 2 <= dwNum)
				dwLS1B <<= 1;
			else
				break;
		}
	}
	return dwLS1B;
}

void *AddrAlignToBytes(void *src, DWORD dwAlignByte)
{
	void *dstAlignAddr;
	DWORD dwAlignMask;
	dwAlignByte = LS1B(dwAlignByte);
	dwAlignMask = dwAlignByte - 1;
	if (dwAlignByte != 0) {
		if (sizeof(UINT *) <= sizeof(UINT))
			dstAlignAddr = (void *)(((DWORD)(src) + dwAlignMask) & (~dwAlignMask));
		else
			dstAlignAddr = (void *)(((unsigned __int64)(src) + dwAlignMask) & (~((unsigned __int64)dwAlignMask)));
	}
	else
		dstAlignAddr = src;
	return dstAlignAddr;
}
