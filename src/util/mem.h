#pragma once
#include "../public/macro.h"
#include "../public/typedef.h"
#include <stdio.h>

namespace newobj
{
    namespace mem
    {
        const static uint32 __big_size = 1024 * 1024 * 10;
        //template<typename T>
        //NEWOBJ_API T* new_()
        //{
        //    return new T();
        //}
        //template<typename T>
        //NEWOBJ_API inline void  delete_(T* p)
        //{
        //    if (p == nullptr)
        //        return;
        //    delete p;
        //}
        //template<typename T>
        //NEWOBJ_API inline T* new_array(size_t size)
        //{
        //    
        //    if (sizeof(T) * size > BIG_SIZE) { printf("[Warn] try to apply for too much memory, please note"); }
        //    return new T[size];
        //}
        //template<typename T>
        //NEWOBJ_API inline void  delete_array(T* p)
        //{
        //    if (p == nullptr)
        //        return;
        //    delete[] p;
        //}
        NEWOBJ_API void* malloc(size_t size);
        NEWOBJ_API void  free(void* src);
        NEWOBJ_API void* realloc(void* src,size_t length);
    }
}