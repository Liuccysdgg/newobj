
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
        class miniblink :public noui::core
        {
        public:
            miniblink();
            ~miniblink();
        public:
            /*´´½¨*/
            static newobj::browser::miniblink* create(uint32 x, uint32 y, uint32 height, uint32 width,QWidget* parent = nullptr, bool dialog = false);
            static void destory(newobj::browser::miniblink* miniblink);
        };
    }
}
#endif
#endif