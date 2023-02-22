#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include <vector>
#ifndef MSVC_2010
namespace newobj
{
	namespace build
	{
		/*构建编译*/
		bool NEWOBJ_API build(const nstring& cl_path, const nstring& src, const nstring& dst_path, const std::vector<nstring>& param, const std::vector<nstring>& include, nstring& strResult);
		/*链接程序*/
		bool NEWOBJ_API link(const nstring& link_path, const std::vector<nstring>& obj, const nstring& dst, const std::vector<nstring>& param, const std::vector<nstring>& libpath, nstring& strResult);
	}
}
#endif