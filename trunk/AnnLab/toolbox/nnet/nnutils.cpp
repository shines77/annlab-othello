
#include "StdAfx.h"
#include "nnutils.h"

_USING_NAMESPACE_MATLAB;

_MATLAB_BEGIN

int trimString(const TCHAR *szString, TCHAR *szBuffer, DWORD dwBufSize)
{
	ASSERT(szBuffer != NULL && szString != NULL && dwBufSize != 0);
	if (szBuffer == NULL || szString == NULL || dwBufSize == 0)
		return -1;

	ASSERT(szBuffer != szString);
	if (szBuffer == szString)
		return -1;

	TCHAR *pszHead, *pszTail;
	pszHead = (TCHAR *)szString;
	pszTail = (TCHAR *)szString + _tcslen(szString) - 1;
	while (pszHead <= pszTail && _tcsncicmp(pszHead, _T(" "), 1) <= 0)
		pszHead++;

	if (pszHead > pszTail) {
		_tcscpy_s(szBuffer, dwBufSize, _T(""));
		return 0;
	}
	else {
		while (_tcsncicmp(pszTail, _T(" "), 1) <= 0)
			pszTail--;

		if (pszTail < pszHead) {
			_tcscpy_s(szBuffer, dwBufSize, _T(""));
			return 0;
		}
		else {
			int nNewLength = pszTail - pszHead + 1;
			if (nNewLength <= (int)dwBufSize) {
				if (nNewLength > 0)
					_tcsncpy_s(szBuffer, dwBufSize, pszHead, nNewLength);
				szBuffer[nNewLength] = 0;
				return nNewLength;
			}
		}
	}
	return -1;
}

int calcperf2( CAnnNetwork *net, CAnnXArray *X, double *Pd, double *Tl, double *Ai, int Q, int TS )
{
	int nRetCode = ERR_NN_NONE;
	int retval = 0;

	ASSERT(net != NULL && X != NULL);
	if (net == NULL || X == NULL)
		return ERR_NN_INVALID_PARAM;

	TCHAR szBuffer[512];
	TCHAR szText[512];
	_tcscpy_s(szText, _countof(szText), _T(" "));
	nRetCode = trimString(szText, szBuffer, _countof(szBuffer));
	TRACE(_T("calcperf2(\"%s\") = %s. (nRetcode = %d)\n"), szText, szBuffer, nRetCode);

	return nRetCode;
}

_MATLAB_END
