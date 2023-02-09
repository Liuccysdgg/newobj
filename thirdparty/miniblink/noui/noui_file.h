#ifndef MSVC_2010
#pragma once
#include "public/define.h"
#include "public/string.h"
#include "noui_core.h"
#include "public/buffer.h"
#ifdef LIB_QT
namespace newobj
{
    namespace browser
    {
        class miniblink;
    }
    namespace noui
    {
        class file:public noui::core
        {
        public:
            file(class newobj::browser::miniblink* miniblink);
            ~file();
        private:
            /*д����*/
            bool writeex(const nstring& path,const newobj::buffer& data);
            /*������*/
            bool readex(const nstring& path,newobj::buffer& data);

            /*************************************�ļ���������**********************************/
            /*��������*/
            nstring create();
            /*���ٶ���*/
            void destory(const nstring& obj);
            /*���ļ�*/
            bool open(const nstring& obj,const nstring& path);
            /*�ر��ļ�*/
            bool close(const nstring& obj);
            /*���ļ�*/
            bool read(const nstring& obj,uint32 idx,uint32 length,newobj::buffer& data);
            /*д�ļ�*/
            bool write(const nstring& obj,const newobj::buffer& data);
            /*����*/
            bool jump(const nstring& obj, uint32 idx);
            /*���*/
            bool clear(const nstring& obj);
            /*д����*/
            bool flush(const nstring& obj);
        };
    }
}
#endif
#endif