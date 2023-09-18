//
//	handler/handler.h
//

#ifndef _HANDLER_H_
#define _HANDLER_H_

// C/C++
#include <windows.h>
#include <ShlObj.h>

class IconHandler :
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
	// 
	// Added COM_DECLSPEC_NOTHROW macro because of error Error E0766: (C++ exception specification for virtual function is incompatible with that of overridden function)
	// See combaseapi.h around line 160
	COM_DECLSPEC_NOTHROW IFACEMETHODIMP GetIconLocation(UINT uFlags, PWSTR pszIconFile, UINT cchMax, int* piIndex, UINT* pwFlags);
	COM_DECLSPEC_NOTHROW IFACEMETHODIMP Extract(PCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize);

	IconHandler();

protected:
	~IconHandler();

private:
	bool IsOfExtension(const wchar_t* extension) const;
	bool IsOfPlatformExtension(const wchar_t* extension) const;
	unsigned int ReadMagic() const;

private:
	long m_RefCount;

	DWORD m_dwMode;
	wchar_t m_szFileName[MAX_PATH];
};

#endif // _HANDLER_H_