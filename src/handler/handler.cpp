

#include "handler/handler.h"
#include <Shlwapi.h>
#include <Wincrypt.h>   // For CryptStringToBinary.
#include <msxml6.h>
#include <Guiddef.h>
#include <strsafe.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "msxml6.lib")


extern HINSTANCE g_hInstDLL;
extern long g_DllRefCount;
extern CLSID CLSID_RageShellIconHandler;

RockstarPackFileHandler::RockstarPackFileHandler() : m_cRef(1)
{
	InterlockedIncrement(&g_DllRefCount);
}

RockstarPackFileHandler::~RockstarPackFileHandler()
{
	InterlockedDecrement(&g_DllRefCount);
}

IFACEMETHODIMP RockstarPackFileHandler::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(RockstarPackFileHandler, IPersistFile),
		QITABENT(RockstarPackFileHandler, IExtractIcon),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) RockstarPackFileHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) RockstarPackFileHandler::Release()
{
	ULONG cRef = InterlockedDecrement(&m_cRef);
	if (0 == cRef)
	{
		delete this;
	}

	return cRef;
}

//


STDMETHODIMP RockstarPackFileHandler::GetClassID(CLSID* pCLSID)
{
	*pCLSID = CLSID_RageShellIconHandler;
	return S_OK;
}

IFACEMETHODIMP RockstarPackFileHandler::Load(PCWSTR pszFile, DWORD dwMode)
{
	m_dwMode = dwMode;
	return StringCchCopy(m_szFileName, ARRAYSIZE(m_szFileName), pszFile);
}

STDMETHODIMP RockstarPackFileHandler::GetIconLocation(
	UINT uFlags, LPTSTR szIconFile, UINT cchMax,
	int* piIndex, UINT* pwFlags)
{
	DWORD     dwFileSizeLo, dwFileSizeHi;
	DWORDLONG qwSize;
	HANDLE    hFile;

	hFile = CreateFile(m_szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return S_FALSE;    // tell the shell to use a default icon

	dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	CloseHandle(hFile);

	if ((DWORD)-1 == dwFileSizeLo && GetLastError() != NO_ERROR)
		return S_FALSE;    // tell the shell to use a default icon

	qwSize = DWORDLONG(dwFileSizeHi) << 32 | dwFileSizeLo;

	TCHAR szModulePath[MAX_PATH];

	GetModuleFileName(g_hInstDLL,
		szModulePath, MAX_PATH);

	*piIndex = 0;

	lstrcpyn(szIconFile, szModulePath, cchMax);
	*pwFlags = 0;
	return S_OK;
}




IFACEMETHODIMP RockstarPackFileHandler::Extract(
	LPCTSTR pszFile, UINT nIconIndex, HICON* phiconLarge,
	HICON* phiconSmall, UINT nIconSize)
{
	return S_FALSE;    // Tell the shell to do the extracting itself.
}