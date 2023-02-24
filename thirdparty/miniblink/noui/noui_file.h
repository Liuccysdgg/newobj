#ifndef MSVC_2010
#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include "noui_core.h"
#include "public/buffer.h"
#if LIB_QT == 1
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
            /*写配置*/
            bool writeex(const nstring& path,const newobj::buffer& data);
            /*读配置*/
            bool readex(const nstring& path,newobj::buffer& data);

            /*************************************文件操作对象**********************************/
            /*创建对象*/
            nstring create();
            /*销毁对象*/
            void destory(const nstring& obj);
            /*打开文件*/
            bool open(const nstring& obj,const nstring& path);
            /*关闭文件*/
            bool close(const nstring& obj);
            /*读文件*/
            bool read(const nstring& obj,uint32 idx,uint32 length,newobj::buffer& data);
            /*写文件*/
            bool write(const nstring& obj,const newobj::buffer& data);
            /*跳到*/
            bool jump(const nstring& obj, uint32 idx);
            /*清空*/
            bool clear(const nstring& obj);
            /*写缓存*/
            bool flush(const nstring& obj);
        };
    }
}
#endif
#endif