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