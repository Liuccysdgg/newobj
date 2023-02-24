#pragma once
#include "public/define.h"
#include "basetype/string.h"

namespace newobj
{
    /*
     * @class：Log4cplus 日志封装类
     * @desc： 
     *      1、程序默认开启，如需更改请修改 public/environment.cpp 中的全局变量实现(注释new即可)
     *      2、默认配置文件放置程序目录 res/log.properties, 示例文件参考：res/log.properties
     */
    class NEWOBJ_API log4
    {
     public:
         log4(const nstring& config_filepath);
         ~log4();
         newobj::log4& info(const nstring& value,const nstring& name = "ALL");
         newobj::log4& error(const nstring& value,const nstring& name = "ALL");
         newobj::log4& fatal(const nstring& value,const nstring& name = "ALL");
         newobj::log4& warn(const nstring& value,const nstring& name = "ALL");
         newobj::log4& debug(const nstring& value,const nstring& name = "ALL");
    private:
        void* m_appender;
        void* m_appender_console;
    };
}
