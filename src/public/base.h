#pragma once
#include <string>
//#include "public/string.h"
/***********************************************************
 * Class������������
 * Description�������඼��̳и���
 ***********************************************************/
namespace newobj
{
    class base
    {
    public:
        inline const std::string& last_error()
        {
            return this->m_lastErrorDesc;
        }
    public:
        std::string m_lastErrorDesc;
    };
}

