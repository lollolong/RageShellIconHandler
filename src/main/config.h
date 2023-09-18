//
//	main/config.h
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

// RAGE Platforms
static const wchar_t g_Platforms[9] = {
	L'x' /* Xenon */,
	L'c' /* Cell */,
	L'd' /* Durango */,
	L'o' /* Orbis */,
	L'p' /* Prospero */,
	L'a' /* Android */,
	L's' /* iOS */,
	L'w' /* x32 */,
	L'y' /* x64 */
};

// Descriptions
#define RPF_FILE_DESCRIPTION						L"Rockstar Pack File"	// .rpf
#define AWC_FILE_DESCRIPTION						L"Audio Wave Container"	// .awc
#define SCRIPT_FILE_DESCRIPTION						L"Script Resource"		// .#sc
#define TEXTURE_DICTIONARY_FILE_DESCRIPTION			L"Texture Dictionary"	// .#td


// Internal Handler Names
#define INPROCSERVERNAME				L"RAGE Shell Icon Handler"

#define RPF_HANDLER						L"RageRpfShellIconHandler"
#define AWC_HANDLER						L"RageAwcShellIconHandler"
#define SCRIPT_HANDLER					L"RageScriptShellIconHandler"
#define TEXTURE_DICTIONARY_HANDLER		L"RageTextureDictShellIconHandler"

#endif // _CONFIG_H_