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
            /*ִ�нű�*/
            nstring shell(const nstring& cmd);
            /*��ǰӦ��Ŀ¼*/
            nstring current_dir();
            /*��ǰ�ļ�·��*/
            nstring current_filepath();
            /*��Ļ���*/
            uint32 device_size_width();
            /*��Ļ�߶�*/
            uint32 device_size_height();
            /*��Ļ���*/
            uint32 work_size_width();
            /*��Ļ�߶�*/
            uint32 work_size_height();
        };
    }
}
#endif
#endif