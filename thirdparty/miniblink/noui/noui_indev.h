#ifndef MSVC_2010
#pragma once
#include "public/define.h"
#include "public/string.h"
#include "noui_core.h"
#ifdef LIB_QT
namespace newobj
{
    namespace browser
    {
        class miniblink;
    }
    namespace noui
    {
        class indev:public noui::core
        {
        public:
            indev(class newobj::browser::miniblink* miniblink);
            ~indev();
        private:
            /*��� x*/
            uint32 cursor_x();
            /*��� y*/
            uint32 cursor_y();
        };
    }
}
#endif
#endif