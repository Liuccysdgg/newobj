#ifndef MSVC_2010
#pragma once
#include "public/define.h"
#include "basetype/string.h"
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
            /*光标 x*/
            uint32 cursor_x();
            /*光标 y*/
            uint32 cursor_y();
        };
    }
}
#endif
#endif