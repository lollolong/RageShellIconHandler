//
//	handler/handler.cpp
//

// Project
#include "handler.h"
#include "main/config.h"

// C/C++
#include <Shlwapi.h>
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

	if (IsOfPlatformExtension(L"sc")) {
		*piIndex = 2;
	}
	else if (IsOfPlatformExtension(L"td")) {
		*piIndex = 3;
	}
	else if (IsOfExtension(L".awc")) {
		*piIndex = 4;
	}
	else if (IsOfExtension(L".rpf")) {
		unsigned int uMagic = ReadMagic();
		(uMagic == 'RPF3' || uMagic == '3FPR') ? *piIndex = 1 : *piIndex = 0;
	}
	else {
		return S_FALSE;
	}

	*pwFlags = 0;
	return S_OK;
}

IFACEMETHODIMP IconHandler::Extract(PCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize)
{
	return S_FALSE;
}

bool IconHandler::IsOfExtension(const wchar_t* extension) const
{
	const wchar_t* ext = wcsrchr(m_szFileName, L'.');
	return (ext != NULL ? !_wcsicmp(ext, extension) : false);
}

bool IconHandler::IsOfPlatformExtension(const wchar_t* extension) const
{
	const wchar_t* ext = wcsrchr(m_szFileName, L'.');
	if (ext)
	{
		bool bPlatformMatch = false;
		for (const wchar_t platform : g_Platforms)
		{
			if (towlower(ext[1]) == platform)
			{
				bPlatformMatch = true;
				break;
			}
		}
		return (bPlatformMatch && !_wcsicmp(ext + 2, extension));
	}
	return false;
}

unsigned int IconHandler::ReadMagic() const
{
	HANDLE hFile = CreateFile(m_szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		return 0;
	}

	unsigned int uMagic{};
	if (!ReadFile(hFile, &uMagic, sizeof(uMagic), NULL, NULL)) {
		return 0;
	}

	CloseHandle(hFile);
	return uMagic;
}