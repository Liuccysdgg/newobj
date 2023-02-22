#pragma once
#include "public/define.h"
namespace newobj
{
    class NEWOBJ_API object
    {
    public:
        object(){}
        ~object(){}
        inline bool is_null(){return m_null;}
        inline void init_obj(){m_null = true;}

    private:
        bool m_null;
    };
}
