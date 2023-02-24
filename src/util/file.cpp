
#include "file.h"
#include <fstream>
#include "forstring.h"
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <cstring>        // for strcat()
#else
#include <unistd.h>
#endif // _WIN32
#ifdef LIB_BOOST
#include <boost/filesystem.hpp>
#endif
#if LIB_QT == 1
#include <QtCore/qfile.h>
#endif
#include "public/exception.h"
namespace newobj
{
	file::file()
	{
		m_file = 0;
	}
	file::~file()
	{
		this->close();
	}
	bool file::open(const nstring& filepath, open_type type)
	{
#ifdef _WIN32
		if(type == OT_R)
            fopen_s(&this->m_file, filepath.c_str(), "rb");
		else if (type == OT_RW_C)
			fopen_s(&this->m_file, filepath.c_str(), "ab+");
#else
		if (type == OT_R)
            this->m_file = fopen(filepath.c_str(), "rb");
		else if (type == OT_RW_C)
            this->m_file =  fopen(filepath.c_str(), "ab+");
#endif
		if (this->m_file == NULL)
			return false;
        this->m_filepath = filepath;
		return true;
	}
	bool file::close()
	{
		if (this->m_file == NULL)
			return true;
		if (fclose(this->m_file) != 0)
		{
			//  this->m_lastErrorDesc = strerror(errno);
			this->m_file = NULL;
			return false;
		}
		this->m_file = NULL;
		return true;

	}
	bool file::flush()
	{
		if (m_file == nullptr)
			return false;
		return fflush(this->m_file) == 0;
	}
	bool file::clear()
	{
		return feof(m_file)==0;
	}
	bool file::read(uint64 index, uint64 len, char* buf) const
	{
		return read_rsize(index, len, buf) == len;
	}
	uint64 file::read_rsize(uint64 idx, uint64 len, char* buf) const
	{
		if (this->m_file == NULL)
			return 0;
		if (fseek(this->m_file, (long)idx, 0) != 0)
			return 0;
		return (uint64)fread(buf, 1, (size_t)len, this->m_file);
	}
	bool file::write(const char* buf, size_t len)
	{
		if (this->m_file == NULL)
			return false;
		//for(int i=0;i<4;i++)
		return fwrite(buf, len, 1, this->m_file) == 1 ? true : false;
	}
	bool file::write(const nstring& value)
	{
		return write(value.c_str(),value.length());
	}
	bool file::write(const buffer& value)
	{
		return write(value.data(), (size_t)value.length());
	}
	bool file::jump(uint64 idx) const
	{
		return fseek(m_file,(long)idx,SEEK_SET)==0;
	}
	const nstring& file::filepath() const
	{
		return this->m_filepath;
	}
	char file::operator[](size_t index) const
	{
		if (jump(index) == false)
		{
			trw_str("Move to index location failed");
		}
		char result;
		if (read(index, 1, &result))
			return result;
		trw_str("Failed to read file index location");
	}
	//uint64 FileTimeToUInt64(FILETIME  ft) {
	//	uint64 time = 0;
	//	LONGLONG  ll;
	//	ULARGE_INTEGER            ui;
	//	ui.LowPart = ft.dwLowDateTime;
	//	ui.HighPart = ft.dwHighDateTime;
	//	ll = ((uint64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;  //这一步是不是多余的
	//	time = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	//	return time;
	//};
	bool  file::details(const nstring& filepath, FileConf& fileconf)
	{

#ifdef _WIN32
		

#if 0
		/*坑人代码，切勿使用！必坑！！！！！！！！！！！*/
		WIN32_FIND_DATAA attr;
		t_ret_f(FindFirstFileA(filepath.c_str(), &attr) == INVALID_HANDLE_VALUE);
		fileconf.lastWriteTime = FileTimeToUInt64(attr.ftLastWriteTime);
		fileconf.lastAccessTime = FileTimeToUInt64(attr.ftLastAccessTime);
		fileconf.createTime = FileTimeToUInt64(attr.ftCreationTime);
		fileconf.isDir = attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			fileconf.fileattr = FA_ARCHIVE;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
			fileconf.fileattr = FA_COMPRESSED;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
			fileconf.fileattr = FA_ENCRYPTED;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			fileconf.fileattr = FA_HIDDEN;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			fileconf.fileattr = FA_READONLY;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			fileconf.fileattr = FA_SYSTEM;
		if (attr.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
			fileconf.fileattr = FA_TEMPORARY;
		return true;
#else
		struct stat s;
		if (stat(filepath.c_str(), &s) != 0)
			return false;
		fileconf.lastWriteTime = s.st_mtime;
		return true;
#endif
#else
		abort();
		return false;
#endif
	}

	bool file::size(const nstring& filepath, uint64* size)
	{
#ifdef _WIN32
		WIN32_FIND_DATA fileInfo;
		HANDLE hFind;
		hFind = FindFirstFileA(filepath.c_str(), &fileInfo);
		if (hFind != INVALID_HANDLE_VALUE)
			*size = fileInfo.nFileSizeLow;
		else
			return false;
		FindClose(hFind);
		return true;
#else
		struct stat statbuf;
        if (stat(newobj::file::format_slash(filepath).c_str(), &statbuf) != 0)
			return false;
		*size = statbuf.st_size;
		return true;
#endif
	}
	/*文件是否存在*/
	bool file::exist(const nstring& filepath)
	{
#ifdef _WIN32
        return _access(newobj::file::format_slash(filepath).c_str(), 0) == 0;
#else
        return access(newobj::file::format_slash(filepath).c_str(), 0) == 0;
#endif
	}
	bool file::remove(const nstring& filepath)
	{
        return ::remove(newobj::file::format_slash(filepath).c_str()) == 0;
	}
	//            void  GetDirFiles(const nstring& rootPath, std::vector<nstring>& name, const std::vector<nstring>& exts)
	//            {
	//#ifdef LIB_BOOST
	//                namespace fs = boost::filesystem;
	//                fs::path fullpath(rootPath);
	//                fs::recursive_directory_iterator end_iter;
	//                for (fs::recursive_directory_iterator iter(fullpath); iter != end_iter; iter++)
	//                {
	//                    try
	//                    {
	//                        if (!fs::is_directory(*iter))
	//                        {
	//                            //std::cout << iter->path().filename() << std::endl;
	//                            nstring file = iter->path().string();
	//                            //ret.push_back(iter->path().string());
	//                            fs::path filePath(file);
	//                            //if()
	//                            
	//                            if (exts.size() != 0)
	//                            {
	//                                nstring extf = filePath.extension().string();
	//                                for_l(i, exts.size())
	//                                {
	//                                    if (exts[i] == extf)
	//                                    {
	//                                        name.push_back(filePath.filename().string());
	//                                    }
	//                                }
	//                            }
	//                            else
	//                            {
	//                                name.push_back(filePath.filename().string());
	//                            }
	//                            
	//
	//                        }
	//                    }
	//                    catch (const std::exception& ex)
	//                    {
	//                        LOG_EXP(0, ex.what());
	//                    }
	//                }
	//#endif
	//            }
	bool  file::list(const nstring& rootPath, std::map<nstring, bool>& list)
	{
		list.clear();
#ifdef _WIN32
		char dirNew[200];
		strcpy_s(dirNew, rootPath.c_str());
		strcat_s(dirNew, "\\*.*");

		intptr_t handle;
		_finddata_t findData;

		handle = _findfirst(dirNew, &findData);    // 查找目录中的第一个文件
		if (handle == -1)
		{
			list.insert(std::make_pair(rootPath,false));
		}
		else
		{
			do
			{
				list.insert(std::make_pair(findData.name, GetFileAttributesA(nstring(rootPath + "\\" + findData.name).c_str()) & FILE_ATTRIBUTE_DIRECTORY));
			} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
		}
		

		//cout << "Done!\n";
		_findclose(handle);    // 关闭搜索句柄
		return true;
#else
		return false;
#endif // _WIN32

	}
	bool  file::write(const nstring& filepath, const char* data, uint32 len)
	{
        nstring _filepath = newobj::file::format_slash(filepath);
        if (exist(_filepath))
		{
            f_ret_f(newobj::file::remove(_filepath.c_str()));
		}
		file file;
        f_ret_f(file.open(_filepath));
		f_ret_f(file.write(data, len));
		file.close();
		return true;
	}
	bool  file::write(const nstring& filepath, const buffer& data)
	{
        return file::write(newobj::file::format_slash(filepath), data.data(), (uint32)data.length());
	}
	bool  file::write(const nstring& filepath, const nstring& data)
	{
        return file::write(newobj::file::format_slash(filepath), data.c_str(), data.length());
	}
	bool  file::read(const nstring& filepath, buffer& data,newobj::open_type type)
	{
        nstring _filepath = newobj::file::format_slash(filepath);
		data.clear();
        if (_filepath.length() == 0)
			return false;
        if (_filepath[0] == ':')
		{
#if LIB_QT == 1
            QFile file(_filepath);
			f_ret_f(file.open(QIODevice::ReadOnly));
			data.append(file.readAll());
			return true;
#else
			return false;
#endif
		}
		else
		{
			file file;
			uint64 size = 0;
			if (newobj::file::size(_filepath, &size) == false)
				return false;

            f_ret_f(file.open(_filepath, type));
			if (size == 0)
				return true;
			data.resize((size_t)size);
			file.read(0, size, (char*)data.data());
			file.close();
			return true;
		}
	}
	bool  file::create_dir(const nstring& filepath, bool recursion)
	{
#ifdef _WIN32
		if(recursion == false)
			return CreateDirectoryA(filepath.c_str(), NULL);
		else
		{
			std::vector<nstring> dirs;
			nstring td = filepath;
			dirs.push_back(td);
			while ((td = file::pre_dir(td)) != "")
				dirs.push_back(td);
			for (uint32 i = dirs.size(); i > 0; i--)
			{
				create_dir(dirs[i - 1]);
			}
			return true;
		}
#else
        return mkdir(newobj::file::format_slash(filepath).c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO)==0;
#endif
	}
	bool  file::copy_dir(const nstring& src, const nstring& dst, bool cover)
 	{
        file::create_dir(newobj::file::format_slash(dst));
		std::list<nstring> dirs;
		std::list<nstring> files;
        f_ret_f(file::ergodic_all(newobj::file::format_slash(src), dirs, files));
		for_iter(iter, dirs)
            file::create_dir(newobj::file::format_slash(dst) + (*iter).right(iter->length() - newobj::file::format_slash(src).length()));
		for_iter(iter, files)
            file::copy_file(*iter, newobj::file::format_slash(dst) + (*iter).right(iter->length() - newobj::file::format_slash(src).length()), cover);
		return true;
	}
	void file::ext(const nstring& path, nstring& ext)
	{
		int32 idx = path.rfind('.');
		t_ret(idx == -1);
		int32 sublen = path.length() - idx - 1;
		ext.append(path.c_str() + path.length() - sublen, sublen);
	}
	bool  file::copy_file(const nstring& src, const nstring& dst, bool cover)
	{
#ifdef _WIN32
		for (uint32 i = 0; i < 3; i++)
		{
			if (CopyFileA(src.c_str(), dst.c_str(), !cover))
				return true;
		}
		return false;
#else
		return false;
#endif
	}
	bool  file::remove_dir(const nstring& dirpath, bool recycle)
	{
        nstring _dirpath = newobj::file::format_slash(dirpath);
		if (recycle == false)
		{
			std::list<nstring> dirs;
			std::list<nstring> files;
            f_ret_f(file::ergodic_all(_dirpath, dirs, files));
			for_iter(iter, files)
				file::remove((*iter));
			for (auto rit = dirs.rbegin(); rit != dirs.rend(); rit++)
			{
#ifdef WIN32
				return RemoveDirectoryA(rit->c_str());
#else
				return mkdir(rit->c_str(), 0755) == 0;
#endif
			}
#ifdef WIN32
			return RemoveDirectoryA(dirpath.c_str());
#else
            return mkdir(_dirpath.c_str(), 0755) == 0;
#endif			
		}
		else
		{
#ifdef _WIN32
			SHFILEOPSTRUCT  shDelFile;
			memset(&shDelFile, 0, sizeof(SHFILEOPSTRUCT));
			shDelFile.fFlags |= FOF_SILENT;                // 不显示进度
			shDelFile.fFlags |= FOF_NOERRORUI;            // 不报告错误信息
			shDelFile.fFlags |= FOF_NOCONFIRMATION;        // 直接删除，不进行确认

			// 设置SHFILEOPSTRUCT的参数为删除做准备
			shDelFile.wFunc = FO_DELETE;        // 执行的操作
			newobj::buffer path;
			path.append(dirpath);
			path.append('\0');
			path.append('\0');
			shDelFile.pFrom = (LPCSTR)path.data();            // 操作的对象，也就是目录（注意：以“\0\0”结尾）
			shDelFile.pTo = NULL;                // 必须设置为NULL
			shDelFile.fFlags |= FOF_ALLOWUNDO;    //删除到回收站
			BOOL bres = SHFileOperation(&shDelFile);    //删除
			return !bres;
#else
			return false;
#endif
		}
	}

	nstring  newobj::file::pre_dir(const nstring& path)
	{
        nstring _path = newobj::file::format_slash(path);

        int32 idx = _path.rfind("\\");
        int32 idx2 = _path.rfind("/");

		if (idx == -1)
		{
			if (idx2 == -1)
			{
				return "";
				//if (path.length() != 0 && path[path.length() - 1] == ':')
				//	return "";
				//return path;
			}
			else
				idx = idx2;
		}
        return _path.substr(0, idx);
	}

	

	nstring  file::filename(const nstring& path)
	{
        nstring _path = newobj::file::format_slash(path);
        auto idx = _path.rfind('\\');
		if (idx < 0)
            return _path;
        return _path.right(_path.length() - idx - 1);
	}
	nstring  file::format(const nstring& path, bool trailshash)
	{
#ifdef WIN32
        char shash_find = '/';
        char shash_replace = '\\';
#else
        char shash_find = '\\';
        char shash_replace = '/';
#endif
		nstring result  = path;
		if (result.length() == 0)
			return path;
        result = format_slash(path);

		if (trailshash)
		{
			if (result[result.length() - 1] != shash_replace)
				result.append(&shash_replace,1);
		}
		else
		{
			if (result[result.length() - 1] == shash_find || result[result.length() - 1] == shash_replace)
				result = result.left(result.length()-1);
		}
        return result;
    }

    nstring file::format_slash(const nstring &path)
    {
        nstring result  = path;
#ifdef WIN32
        char shash_find = '/';
        char shash_replace = '\\';
#else
        char shash_find = '\\';
        char shash_replace = '/';
#endif
        for (uint32 i = 0; i < result.length(); i++)
        {
            if (result[i] == shash_find)
                result[i] = shash_replace;
        }
        return result;
    }
	bool  file::ergodic_all(const nstring& path,std::list<nstring>& dirs, std::list<nstring>& files, int32 depth)
	{
        nstring _path = newobj::file::format_slash(path);

		if (depth == 0)
			return true;
        if (_path == "")
            return false;
		std::map<nstring, bool> path_list;
        f_ret_f(list(_path, path_list));
		nstring newpath;
		for_iter(iter,path_list)
		{
			if (iter->first == "." || iter->first == "..")
				continue;
            newpath = _path + "\\" + iter->first;
			if (iter->second == true)
			{
				dirs.push_back(newpath);
				f_ret_f(ergodic_all(newpath, dirs, files, depth-1));
			}
			else
				files.push_back(newpath);
		}
		return true;
	}

	tempfile::tempfile(const nstring& filepath)
	{
		if (!this->open(filepath))
		{
			trw_str("open \""+filepath+"\" failed。");
		}
	}

	tempfile::~tempfile()
	{
		this->close();
        file::remove(this->filepath());
    }

    newobj::buffer newobj::file::read(const nstring &filepath, open_type type)
    {
        newobj::buffer data;
        newobj::file::read(filepath,data,type);
        return data;
    }

}


