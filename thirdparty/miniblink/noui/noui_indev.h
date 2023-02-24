#ifndef MSVC_2010
#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include "noui_core.h"
#if LIB_QT == 1
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