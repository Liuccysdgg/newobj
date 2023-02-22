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
        class system:public noui::core
        {
        public:
            system(class newobj::browser::miniblink* miniblink);
            ~system();
        private:
            /*执行脚本*/
            nstring shell(const nstring& cmd);
            /*当前应用目录*/
            nstring current_dir();
            /*当前文件路径*/
            nstring current_filepath();
            /*屏幕宽度*/
            uint32 device_size_width();
            /*屏幕高度*/
            uint32 device_size_height();
            /*屏幕宽度*/
            uint32 work_size_width();
            /*屏幕高度*/
            uint32 work_size_height();
        };
    }
}
#endif
#endif