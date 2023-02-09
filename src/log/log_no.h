#pragma once
#include "log_slice.h"
#include "public/string.h"
#include "log_define.h"
namespace newobj
{
	namespace log
	{
		class NEWOBJ_API no:private slice
		{
		public:
			no(const nstring& dirpath,bool only_filepath,bool print);
			~no();
			void print(bool param);
			bool print();

			newobj::log::no& succ(const nstring& value);
			newobj::log::no& warn(const nstring& value);
			newobj::log::no& info(const nstring& value);
			newobj::log::no& error(const nstring& value, const nstring& location_code_info = "");
			void exp(const nstring& value, const nstring& location_code_info);
		private:
			nstring make(const nstring& type,const nstring& value,const nstring& location_code_info,int32 color,int32 color_bg,int32 attr);
		private:
			bool m_print;
			nstring m_path;
		};
	}
}