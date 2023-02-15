#include "build.h"
#include "system.h"
#include "util/forstring.h"
#ifndef MSVC_2010
bool  build::build(const nstring& cl_path, const nstring& src, const nstring& dst_path, const std::vector<nstring>& param, const std::vector<nstring>& include, nstring& strResult)
{
#ifdef _WIN32
    nstring str_param;
    nstring str_include;
    for (auto p : param)
        str_param += " " + p;
    for (auto inc : include)
        str_include += " /I\"" + inc + "\"";
    nstring result;
    nstring cmd = cl_path + " \"" + src + "\" " + str_param + " " + str_include + " /Fo\"" + dst_path + "\"";
    int result_id = system::shell(cmd, result, "");
    strResult = "\n[build]£º" + cmd + "\n[result][" + nstring::from(result_id) + "]£º" + result;

    if (result_id == 0)
    {
        //LOG_INFO(0, strResult);
    }
    else
    {
        //LOG_ERROR(0, strResult);
    }
    return result_id == 0;
#else
    return false;
#endif
}
bool  build::link(const nstring& link_path, const std::vector<nstring>& obj, const nstring& dst, const std::vector<nstring>& param, const std::vector<nstring>& libpath, nstring& strResult)
{
#ifdef _WIN32
    nstring str_obj;
    nstring str_param;
    nstring str_libpath;
    for (auto o : obj)
        str_obj += " " + o;
    for (auto p : param)
        str_param += " " + p;
    for (auto lib : libpath)
    {
        nstring t = " /LIBPATH:\"" + lib;
        t.append("\"");
        str_libpath += t;
    }


    nstring result;
    nstring cmd = link_path + " " + str_obj + " /OUT:\"" + dst + "\" " + str_param + " " + str_libpath;
    int result_id = system::shell(cmd, result, "");
    strResult = "\n[build]£º" + cmd + "\n[result][" + nstring::from(result_id) + "]£º" + result;
    if (result_id == 0)
    {
        //LOG_INFO(0, strResult);
    }
    else
    {
        //LOG_ERROR(0, strResult);
    }
    return result_id == 0;
#else
    return false;
#endif
}
#endif
