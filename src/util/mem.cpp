#include "mem.h"
#include <string>

#define LIB_MIMALLOC
//#define PRINT_MEM_INFO
#if defined(LIB_MIMALLOC) && !defined(MSVC_2010)
#if defined(_WIN32)
#include "../mimalloc/include/mimalloc.h"
#else
#include "mimalloc/include/mimalloc.h"
#endif
#endif
#include "util/lock.h"
static newobj::mutex m_mutex;
static size_t mem_size = 0;
void* newobj::mem::malloc(size_t size)
{
    m_mutex.lock();
    mem_size++;
    m_mutex.unlock();
    if (size > __big_size) { printf("[Warn] try to apply for too much memory, please note"); }

#if defined(_WIN32) && !defined(MSVC_2010)
    void* result = mi_malloc(size);
#else
    void* result = ::malloc(size);
#endif
    
#ifdef PRINT_MEM_INFO
    std::cout << "mem::malloc\t\t" <<std::dec<< size << "\t\t" << std::hex << result << std::endl;
#endif
    return result;
}

void newobj::mem::free(void* src)
{
    m_mutex.lock();
    mem_size--;
    m_mutex.unlock();
#ifdef PRINT_MEM_INFO
    std::cout << "mem::free\t\t\t\t" << std::hex << src << std::endl;
#endif
    if (src == nullptr)
        return;
#if defined(_WIN32) && !defined(MSVC_2010)
    mi_free(src);
#else
    ::free(src);
#endif
}

void* newobj::mem::realloc(void* src,size_t length)
{
#if defined(_WIN32) && !defined(MSVC_2010)
    void* result = mi_realloc(src, length);;
#else
    void* result = ::realloc(src, length);;
#endif
#ifdef PRINT_MEM_INFO
    std::cout << "mem::realloc\t\t"<< std::dec << length<<"\t\t" << std::hex << src << "->" << std::hex << result << std::endl;
#endif
    return result;
}
