//
//	registry/registry.h
//

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include <windows.h>

HRESULT RegisterInprocServer(PCWSTR pszModule, const CLSID& clsid, PCWSTR pszFriendlyName, PCWSTR pszThreadModel);
HRESULT UnregisterInprocServer(const CLSID& clsid);

HRESULT RegisterShellIconHandler(const CLSID& clsid, PCWSTR pszHandlerName, PCWSTR pszFileTypeName);
HRESULT UnregisterShellIconHandler(PCWSTR pszHandlerName);

HRESULT RegisterShellFileExtension(PCWSTR pszFileType, PCWSTR pszHandlerName);
HRESULT UnregisterShellFileExtension(PCWSTR pszFileType);

#endif // _REGISTRY_H_