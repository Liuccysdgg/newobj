
#include "kaguya/kaguya.hpp"
#include "util/file.h"

namespace newobj
{
	namespace lua
	{
		namespace module
		{
			class file
			{
			public:
				file();
				~file();
				bool open(const std::string& filepath);
				bool write(const std::string& value);
				bool jump(uint64 idx);
				bool close();
				bool flush();
				bool clear();
				std::string read(uint32 idx, uint32 len);
				std::string filepath();
			private:
				newobj::file m_file;
			public:
				static std::string read_all(const std::string& path);
				static uint64 size(const std::string& path);
				static bool exist(const std::string& path);
				static bool remove(const std::string& filepath);
				static bool write_all(const std::string& filepath, const std::string& data);
				static bool create_dir(const std::string& filepath, bool recursion = false);
				static bool copy_dir(const std::string& src, const std::string& dst, bool cover = false);
				static bool copy_file(const std::string& src, const std::string& dst, bool cover = true);
				static bool remove_dir(const std::string& dirpath, bool recycle = false);
				static std::string pre_dir(const std::string& path);
				static std::string ext(const std::string& path);
				static std::string filename(const std::string& path);
				static std::string format(const std::string& path, bool trailshash = false);
			};
			void regist_file(void *config,kaguya::State* L);
		}

	}
}