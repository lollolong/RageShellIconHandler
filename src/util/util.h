//
//	util/util.h
//

#ifndef _UTIL_H_
#define _UTIL_H_

#include <Windows.h>

HRESULT RegisterMaskedPlatformExtension(const wchar_t* mask, const wchar_t* handlerName, bool bRegister = true);
HRESULT UnregisterMaskedPlatformExtension(const wchar_t* mask);

#endif // _UTIL_H_