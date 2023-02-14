#pragma once
#include "public/define.h"

#ifndef _WIN32
#include "hpsocket/HPSocket.h"
#include "hpsocket/HPSocket-SSL.h"
#else
#include "hpsocket/Include/HPSocket/HPSocket.h"
#include "hpsocket/Include/HPSocket/HPSocket-SSL.h"
#endif

