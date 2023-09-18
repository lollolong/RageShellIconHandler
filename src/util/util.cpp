//
//	util/util.cpp
//

// Project
#include "util.h"
#include "main/config.h" // for RAGE Platforms array
#include "registry/registry.h"	// for the extension registration

HRESULT RegisterMaskedPlatformExtension(const wchar_t* mask, const wchar_t* handlerName, bool bRegister/* = true*/)
{
	HRESULT hr = S_OK;

	wchar_t maskedExt[16] = {};
	size_t maxLength = sizeof(maskedExt) / sizeof(wchar_t);

	(void)lstrcpyn(maskedExt, mask, (int)maxLength);

	wchar_t* curChar = maskedExt;
	while (*curChar)
	{
		if (*curChar == L'#')
		{
			for (const wchar_t platform : g_Platforms)
			{
				*curChar = platform;
				hr = bRegister && handlerName != NULL ? RegisterShellFileExtension(maskedExt, handlerName)
					: UnregisterShellFileExtension(maskedExt);

				if (FAILED(hr)) {
					break;
				}
			}
			break;
		}
		else
		{
			curChar++;
		}
	}
	return hr;
}

HRESULT UnregisterMaskedPlatformExtension(const wchar_t* mask)
{
	return RegisterMaskedPlatformExtension(mask, NULL, false);
}