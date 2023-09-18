//
//	main/dllmain.cpp
//

// Project
#include "config.h"
#include "factory/factory.h"
#include "registry/registry.h"
#include "util/util.h"

// C/C++
#include <windows.h>
#include <shlobj.h>
#include <Guiddef.h>

#pragma warning (disable : 28251) // Inconsistent annotation?

// Vars
long        g_DllRefCount;
HINSTANCE   g_hInstDLL;

// {BBDA5482-6402-4B00-AE5F-5C20F896545A}
CLSID CLSID_RageShellIconHandler =
{ 0xbbda5482, 0x6402, 0x4b00, { 0xae, 0x5f, 0x5c, 0x20, 0xf8, 0x96, 0x54, 0x5a } };


BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstDLL = hinstDLL;
		DisableThreadLibraryCalls(hinstDLL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppvObj)
{
	HRESULT hr = E_OUTOFMEMORY;
	*ppvObj = NULL;

	ClassFactory* pClassFactory = new ClassFactory();
	if (pClassFactory) {
		hr = pClassFactory->QueryInterface(riid, ppvObj);
		pClassFactory->Release();
	}
	return hr;
}

STDAPI DllRegisterServer()
{
	HRESULT hr = S_OK;

	wchar_t szModule[MAX_PATH];
	if (GetModuleFileName(g_hInstDLL, szModule, ARRAYSIZE(szModule)) == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	// TODO: Better error handling!
	hr = RegisterInprocServer(szModule, CLSID_RageShellIconHandler, INPROCSERVERNAME, TEXT("Apartment"));

	// Extension descriptions (e.g rpf = Rockstar Pack File)
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, RPF_HANDLER,					RPF_FILE_DESCRIPTION);
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, AWC_HANDLER,					AWC_FILE_DESCRIPTION);
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, SCRIPT_HANDLER,				SCRIPT_FILE_DESCRIPTION);
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, TEXTURE_DICTIONARY_HANDLER,	TEXTURE_DICTIONARY_FILE_DESCRIPTION);

	// Platform independent
	hr = RegisterShellFileExtension(TEXT(".rpf"), RPF_HANDLER);
	hr = RegisterShellFileExtension(TEXT(".awc"), AWC_HANDLER);

	// Platform extensions
	hr = RegisterMaskedPlatformExtension(TEXT(".#sc"), SCRIPT_HANDLER);
	hr = RegisterMaskedPlatformExtension(TEXT(".#td"), TEXTURE_DICTIONARY_HANDLER);

	if (SUCCEEDED(hr))
	{
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}
	return hr;
}

STDAPI DllUnregisterServer()
{
	HRESULT hr = S_OK;

	wchar_t szModule[MAX_PATH];
	if (GetModuleFileName(g_hInstDLL, szModule, ARRAYSIZE(szModule)) == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	// TODO: Better error handling!
	hr = UnregisterInprocServer(CLSID_RageShellIconHandler);

	// Handlers
	hr = UnregisterShellIconHandler(RPF_HANDLER);
	hr = UnregisterShellIconHandler(AWC_HANDLER);
	hr = UnregisterShellIconHandler(SCRIPT_HANDLER);
	hr = UnregisterShellIconHandler(TEXTURE_DICTIONARY_HANDLER);

	// Platform independent
	hr = UnregisterShellFileExtension(TEXT(".rpf"));
	hr = UnregisterShellFileExtension(TEXT(".awc"));

	// Platform extensions
	hr = UnregisterMaskedPlatformExtension(TEXT(".#sc"));
	hr = UnregisterMaskedPlatformExtension(TEXT(".#td"));

	if (SUCCEEDED(hr))
	{
		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}
	return hr;
}

STDAPI DllCanUnloadNow()
{
	return g_DllRefCount > 0 ? S_FALSE : S_OK;
}
