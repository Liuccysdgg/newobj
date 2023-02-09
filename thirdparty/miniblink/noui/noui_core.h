#pragma once
#ifndef MSVC_2010
#include "public/define.h"
#include "public/string.h"
#ifdef LIB_QT
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
#ifdef LIB_QT
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