#pragma once
#include <string>
//#include "public/string.h"
/***********************************************************
 * Class：公共基础类
 * Description：所有类都需继承该类
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

