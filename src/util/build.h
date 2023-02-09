#pragma once
#include "public/define.h"
#include "public/string.h"
#include <vector>
#ifndef MSVC_2010
namespace newobj
{
	namespace build
	{
		/*��������*/
		bool NEWOBJ_API build(const nstring& cl_path, const nstring& src, const nstring& dst_path, const std::vector<nstring>& param, const std::vector<nstring>& include, nstring& strResult);
		/*���ӳ���*/
		bool NEWOBJ_API link(const nstring& link_path, const std::vector<nstring>& obj, const nstring& dst, const std::vector<nstring>& param, const std::vector<nstring>& libpath, nstring& strResult);
	}
}
#endif