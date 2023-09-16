//
//	registry/registry.cpp
//

// Project
#include "registry.h"

// C/C++
#include <strsafe.h>

HRESULT WriteRegistryValue(HKEY hKey, PCWSTR pszSubKey, PCWSTR pszValueName, PCWSTR pszData)
{
	HRESULT hr;
	HKEY hSubKey = NULL;

	hr = HRESULT_FROM_WIN32(RegCreateKeyEx(hKey, pszSubKey, 0,
		NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL));

	if (SUCCEEDED(hr))
	{
		if (pszData != NULL)
		{
			DWORD cbData = lstrlen(pszData) * sizeof(*pszData);
			hr = HRESULT_FROM_WIN32(RegSetValueEx(hSubKey, pszValueName, 0,
				REG_SZ, reinterpret_cast<const BYTE*>(pszData), cbData));
		}

		RegCloseKey(hSubKey);
	}
	return hr;
}


/*
	https://learn.microsoft.com/en-us/windows/win32/com/inprocserver32
	https://learn.microsoft.com/en-us/windows/win32/cossdk/threading-model-attribute

	HKEY_CLASSES_ROOT\CLSID
	   {CLSID}
		  InprocServer32
			 (Default) = path
			 ThreadingModel = value
*/

HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, PCWSTR pszFriendlyName, PCWSTR pszThreadModel)
{
	HRESULT hr = S_OK;

	wchar_t szCLSID[MAX_PATH];
	wchar_t szSubkey[MAX_PATH];

	(void)StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));
	hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), TEXT("CLSID\\%s"), szCLSID);

	if (SUCCEEDED(hr))
	{
		hr = WriteRegistryValue(HKEY_CLASSES_ROOT, szSubkey, NULL, pszFriendlyName);

		if (SUCCEEDED(hr))
		{
			hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), TEXT("CLSID\\%s\\InprocServer32"), szCLSID);

			if (SUCCEEDED(hr))
			{
				hr = WriteRegistryValue(HKEY_CLASSES_ROOT, szSubkey, NULL, pszModule);

				if (SUCCEEDED(hr))
				{
					hr = WriteRegistryValue(HKEY_CLASSES_ROOT, szSubkey, TEXT("ThreadingModel"), pszThreadModel);
				}
			}
		}
	}
	return hr;
}

HRESULT UnregisterInprocServer(const CLSID& clsid)
{
	HRESULT hr = S_OK;

	wchar_t szCLSID[MAX_PATH];
	wchar_t szSubkey[MAX_PATH];

	(void)StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));
	hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), TEXT("CLSID\\%s"), szCLSID);

	if (SUCCEEDED(hr))
	{
		hr = HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, szSubkey));
	}
	return hr;
}


/*
	https://learn.microsoft.com/en-us/windows/win32/shell/how-to-create-icon-handlers

	HKEY_CLASSES_ROOT
	   .myp
		  (Default) = MyProgram.1
	   MyProgram.1
		  (Default) = MyProgram Application
		  DefaultIcon
			 (Default) = %1
		  Shellex
			 IconHandler
				(Default) = {The handler's CLSID GUID}
*/

HRESULT RegisterShellIconHandler(const CLSID& clsid, PCWSTR pszHandlerName, PCWSTR pszFileTypeName)
{
	HRESULT hr = S_OK;

	hr = WriteRegistryValue(HKEY_CLASSES_ROOT, pszHandlerName, NULL, pszFileTypeName);

	if (SUCCEEDED(hr))
	{
		hr = WriteRegistryValue(HKEY_CLASSES_ROOT, pszHandlerName, TEXT("DefaultIcon"), TEXT("%1"));

		if (SUCCEEDED(hr))
		{
			wchar_t szCLSID[MAX_PATH];
			wchar_t szSubkey[MAX_PATH];

			(void)StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID));
			hr = StringCchPrintf(szSubkey, ARRAYSIZE(szSubkey), TEXT("%s\\shellex\\IconHandler"), pszHandlerName);

			if (SUCCEEDED(hr))
			{
				hr = WriteRegistryValue(HKEY_CLASSES_ROOT, szSubkey, NULL, szCLSID);
			}
		}
	}
	return hr;
}

HRESULT UnregisterShellIconHandler(PCWSTR pszHandlerName)
{
	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, pszHandlerName));
}


HRESULT RegisterShellFileExtension(PCWSTR pszFileType, PCWSTR pszHandlerName)
{
	return WriteRegistryValue(HKEY_CLASSES_ROOT, pszFileType, NULL, pszHandlerName);
}

HRESULT UnregisterShellFileExtension(PCWSTR pszFileType)
{
	return HRESULT_FROM_WIN32(RegDeleteTree(HKEY_CLASSES_ROOT, pszFileType));
}