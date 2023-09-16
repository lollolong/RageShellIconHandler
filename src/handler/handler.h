

#pragma once

#include <windows.h>
#include <wincodec.h> 
#include <ShlObj.h>

#pragma comment(lib, "windowscodecs.lib")


class RockstarPackFileHandler :
	public IPersistFile,
	public IExtractIcon
{
public:
	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	// IPersistFile
	IFACEMETHODIMP IsDirty(void)											{ return E_NOTIMPL; }
	IFACEMETHODIMP Save(LPCOLESTR /*pszFileName*/, BOOL /*fRemember*/)		{ return E_NOTIMPL; }
	IFACEMETHODIMP SaveCompleted(LPCOLESTR /*pszFileName*/)					{ return E_NOTIMPL; }
	IFACEMETHODIMP GetCurFile(LPOLESTR* /*ppszFileName*/)					{ return E_NOTIMPL; }
	IFACEMETHODIMP GetClassID(CLSID* pCLSID);
	IFACEMETHODIMP Load(PCWSTR pszFileName, DWORD dwMode);

	// IExtractIcon
	IFACEMETHODIMP GetIconLocation(UINT uFlags, LPTSTR szIconFile, UINT cchMax,
		int* piIndex, UINT* pwFlags);

	IFACEMETHODIMP Extract(LPCTSTR pszFile, UINT nIconIndex, HICON* phiconLarge,
		HICON* phiconSmall, UINT nIconSize);

	RockstarPackFileHandler();

protected:
	~RockstarPackFileHandler();

private:
	// Reference count of component.
	long m_cRef;

	wchar_t m_szFileName[MAX_PATH];    // The file name
	DWORD m_dwMode;                  // The file access mode
};