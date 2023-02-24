#pragma once
#ifndef MSVC_2010
#include "public/define.h"
#include "basetype/string.h"
#if LIB_QT == 1
#ifdef QT4
#include "Qt/qwidget.h"
#else
#include "QtWidgets/qwidget.h"
#endif
#endif
namespace newobj
{
    namespace browser
    {
        class miniblink;
    }
    namespace noui
    {
#if LIB_QT == 1
        class core
        {
        public:
            core(class newobj::browser::miniblink* miniblink);
            ~core();
            class newobj::browser::miniblink* web();
            inline void web(newobj::browser::miniblink* miniblink) { m_miniblink = miniblink; }
        private:
            class newobj::browser::miniblink* m_miniblink;
        };
#endif
    }
}
#endif