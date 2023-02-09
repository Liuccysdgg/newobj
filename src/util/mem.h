#pragma once
#include "public/define.h"
#include <stdio.h>
namespace newobj
{
    namespace mem
    {
        NEWOBJ_API void* malloc(size_t size);
        NEWOBJ_API void  free(void* src);
        NEWOBJ_API void* realloc(void* src,size_t length);
    }
}