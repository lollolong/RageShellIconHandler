//
//	main/dllmain.cpp
//

// Project
#include "config.h"
#include "factory/factory.h"
#include "registry/registry.h"

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
	hr = RegisterInprocServer(szModule, CLSID_RageShellIconHandler, TEXT(INPROCSERVERNAME), TEXT("Apartment"));

	// Tell the shell the type name
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, TEXT(RPF_HANDLER),					TEXT(RPF_FILE_DESCRIPTION));
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, TEXT(AWC_HANDLER),					TEXT(AWC_FILE_DESCRIPTION));
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, TEXT(SCRIPT_HANDLER),					TEXT(SCRIPT_FILE_DESCRIPTION));
	hr = RegisterShellIconHandler(CLSID_RageShellIconHandler, TEXT(TEXTURE_DICTIONARY_HANDLER),		TEXT(TEXTURE_DICTIONARY_FILE_DESCRIPTION));

	// Platform independent
	hr = RegisterShellFileExtension(TEXT(".rpf"), TEXT(RPF_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".awc"), TEXT(AWC_HANDLER));

	// Script Resources
	hr = RegisterShellFileExtension(TEXT(".xsc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".csc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".dsc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".osc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".psc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".asc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".ssc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".wsc"), TEXT(SCRIPT_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".ysc"), TEXT(SCRIPT_HANDLER));

	// Texture Dictionary
	hr = RegisterShellFileExtension(TEXT(".xtd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".ctd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".dtd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".otd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".ptd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".atd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".std"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".wtd"), TEXT(TEXTURE_DICTIONARY_HANDLER));
	hr = RegisterShellFileExtension(TEXT(".ytd"), TEXT(TEXTURE_DICTIONARY_HANDLER));

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

	hr = UnregisterShellIconHandler(TEXT(RPF_HANDLER));
	hr = UnregisterShellIconHandler(TEXT(AWC_HANDLER));
	hr = UnregisterShellIconHandler(TEXT(SCRIPT_HANDLER));
	hr = UnregisterShellIconHandler(TEXT(TEXTURE_DICTIONARY_HANDLER));

	hr = UnregisterShellFileExtension(TEXT(".rpf"));
	hr = UnregisterShellFileExtension(TEXT(".awc"));

	hr = UnregisterShellFileExtension(TEXT(".xsc"));
	hr = UnregisterShellFileExtension(TEXT(".csc"));
	hr = UnregisterShellFileExtension(TEXT(".dsc"));
	hr = UnregisterShellFileExtension(TEXT(".osc"));
	hr = UnregisterShellFileExtension(TEXT(".psc"));
	hr = UnregisterShellFileExtension(TEXT(".asc"));
	hr = UnregisterShellFileExtension(TEXT(".ssc"));
	hr = UnregisterShellFileExtension(TEXT(".wsc"));
	hr = UnregisterShellFileExtension(TEXT(".ysc"));

	hr = UnregisterShellFileExtension(TEXT(".xtd"));
	hr = UnregisterShellFileExtension(TEXT(".ctd"));
	hr = UnregisterShellFileExtension(TEXT(".dtd"));
	hr = UnregisterShellFileExtension(TEXT(".otd"));
	hr = UnregisterShellFileExtension(TEXT(".ptd"));
	hr = UnregisterShellFileExtension(TEXT(".atd"));
	hr = UnregisterShellFileExtension(TEXT(".std"));
	hr = UnregisterShellFileExtension(TEXT(".wtd"));
	hr = UnregisterShellFileExtension(TEXT(".ytd"));

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
