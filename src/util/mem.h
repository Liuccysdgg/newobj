#pragma once
#include "../public/macro.h"
#include "../public/typedef.h"
#include "../public/define.h"
#include <stdio.h>

namespace newobj
{
    namespace mem
    {
        const static uint32 __big_size = 1024 * 1024 * 10;
        NEWOBJ_API void* malloc(size_t size);
        NEWOBJ_API void  free(void* src);
        NEWOBJ_API void* realloc(void* src,size_t length);
    }
}