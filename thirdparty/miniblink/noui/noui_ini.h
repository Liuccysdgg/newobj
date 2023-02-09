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
        class ini:public noui::core
        {
        public:
            ini(class newobj::browser::miniblink* miniblink);
            ~ini();
        private:
            /*–¥≈‰÷√*/
            bool write(const nstring& path,const nstring& node,const nstring& name,const nstring& value);
            /*∂¡≈‰÷√*/
            nstring read(const nstring& path, const nstring& node, const nstring& name);
        };
    }
}
#endif
#endif