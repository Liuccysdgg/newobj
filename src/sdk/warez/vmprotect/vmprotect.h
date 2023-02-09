#pragma once
#if defined(LIB_VMPROTECT)
#include "VMProtectSDK.h"
#define VMP_BEGIN(TITLE) VMProtectBegin(TITLE)
#define VMP_STRING(STRING) VMProtectDecryptStringA(STRING)
#define VMP_END VMProtectEnd()
#else
#define VMP_BEGIN(TITLE)
#define VMP_STRING(STRING) STRING
#define VMP_END
#endif
