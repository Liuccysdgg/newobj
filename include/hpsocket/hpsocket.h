#pragma once
#include "public/define.h"

#ifdef _WIN32
#include "hpsocket/windows/HPSocket.h"
#include "hpsocket/windows/HPSocket-SSL.h"
#else
#include "hpsocket/linux/HPSocket.h"
#include "hpsocket/linux/HPSocket-SSL.h"
#endif

