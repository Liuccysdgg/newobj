#pragma once
#include "public/define.h"

#ifdef _WIN32
#if defined(LIB_HPSOCKET_SSL) && !defined(THIRDPARTY_HPSOCKET)
#include "windows/include/ssl/HPSocket.h"
#include "windows/include/ssl/HPSocket-SSL.h"
#else
#include "windows/include/nossl/HPSocket.h"
#include "windows/include/nossl/HPSocket-SSL.h"
#endif
#else
#include "linux/include/HPSocket.h"
#include "linux/include/HPSocket-SSL.h"
#endif

