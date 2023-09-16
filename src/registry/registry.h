

#pragma once

#include <windows.h>

HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, PCWSTR pszFriendlyName, PCWSTR pszThreadModel);
HRESULT UnregisterInprocServer(const CLSID& clsid);

HRESULT RegisterShellIconHandler(const CLSID& clsid, PCWSTR pszHandlerName, PCWSTR pszFileTypeName);
HRESULT UnregisterShellIconHandler(PCWSTR pszHandlerName);

HRESULT RegisterShellFileExtension(PCWSTR pszFileType, PCWSTR pszHandlerName);
HRESULT UnregisterShellFileExtension(PCWSTR pszFileType);