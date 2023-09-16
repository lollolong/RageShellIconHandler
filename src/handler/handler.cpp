//
//	handler/handler.cpp
//

// Project
#include "handler.h"

// C/C++
#include <Shlwapi.h>
#include <msxml6.h>
#include <Guiddef.h>
#include <strsafe.h>

#include <string.h>

// Vars
extern long			g_DllRefCount;
extern HINSTANCE	g_hInstDLL;
extern CLSID		CLSID_RageShellIconHandler;


IconHandler::IconHandler() : m_RefCount(1), m_dwMode(0)
{
	memset(m_szFileName, 0, sizeof(m_szFileName));
	InterlockedIncrement(&g_DllRefCount);
}

IconHandler::~IconHandler()
{
	InterlockedDecrement(&g_DllRefCount);
}

IFACEMETHODIMP IconHandler::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(IconHandler, IPersistFile),
		QITABENT(IconHandler, IExtractIcon),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) IconHandler::AddRef()
{
	return InterlockedIncrement(&m_RefCount);
}

IFACEMETHODIMP_(ULONG) IconHandler::Release()
{
	ULONG cRef = InterlockedDecrement(&m_RefCount);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP IconHandler::GetClassID(CLSID* pCLSID)
{
	*pCLSID = CLSID_RageShellIconHandler;
	return S_OK;
}

IFACEMETHODIMP IconHandler::Load(PCWSTR pszFile, DWORD dwMode)
{
	m_dwMode = dwMode;
	return StringCchCopy(m_szFileName, ARRAYSIZE(m_szFileName), pszFile);
}

STDMETHODIMP IconHandler::GetIconLocation(UINT uFlags, PWSTR pszIconFile, UINT cchMax, int* piIndex, UINT* pwFlags)
{
	TCHAR szModulePath[MAX_PATH];

	GetModuleFileName(g_hInstDLL, szModulePath, MAX_PATH);
	(void)lstrcpyn(pszIconFile, szModulePath, cchMax);

	const wchar_t* ext = wcsrchr(m_szFileName, '.');
	if (ext)
	{
		const bool bIsPackfileExt = !_wcsicmp(ext, L".rpf");

		const bool bIsScriptResourceExt = !_wcsicmp(ext, L".xsc") ||
			!_wcsicmp(ext, L".csc") ||
			!_wcsicmp(ext, L".dsc") ||
			!_wcsicmp(ext, L".osc") ||
			!_wcsicmp(ext, L".psc") ||
			!_wcsicmp(ext, L".asc") ||
			!_wcsicmp(ext, L".ssc") ||
			!_wcsicmp(ext, L".wsc") ||
			!_wcsicmp(ext, L".ysc");

		if (bIsPackfileExt) {
			*piIndex = 0;
		}
		else if (bIsScriptResourceExt) {
			*piIndex = 2;
		}
	}

	HANDLE hFile = CreateFile(m_szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		char magic[4] = {};
		if (ReadFile(hFile, magic, sizeof(magic), NULL, NULL))
		{
			const bool bIsPackfile = (magic[0] == 'R' && magic[1] == 'P' && magic[2] == 'F') ||
				(magic[1] == 'F' && magic[2] == 'P' && magic[3] == 'R');

			if (bIsPackfile)
			{
				const bool bIsV3 = (magic[3] == '3' || magic[0] == '3');
				bIsV3 ? *piIndex = 1 : *piIndex = 0;
			}
		}
		CloseHandle(hFile);
	}
	else
	{
		return S_FALSE;
	}

	*pwFlags = 0;
	return S_OK;
}

IFACEMETHODIMP IconHandler::Extract(PCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize)
{
	return S_FALSE;
}