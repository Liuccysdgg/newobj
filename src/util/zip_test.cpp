#include "zip_test.h"
#include "file.h"
#if 0
newobj::zip::zip()
{
    m_zip = nullptr;
}

newobj::zip::~zip()
{
    close();
}

bool newobj::zip::open(const nstring& filepath)
{
    close();
    int error = 0;
    m_zip = (::zip*)zip_open(filepath.c_str(), ZIP_CHECKCONS, &error);
    if (m_zip == nullptr)
    {
        m_lastErrorDesc = nstring::from(error);
        return false;
    }
    return true;
}

void newobj::zip::close()
{
    if(m_zip != nullptr)
        zip_close(this->m_zip);
    m_zip = nullptr;
}

const std::map<uint32, newobj::zip::zip_file_info>& newobj::zip::files()
{
	m_files.clear();
	uint32 size = this->size();
	for (uint32 i = 0; i < size; i++) {
		analysis_noret(i);
	}
    return m_files;
}

bool newobj::zip::write(uint32 index, const nstring& filepath)
{
	
	zip_file_t* zip_file = zip_fopen_index(m_zip, index, 0);
	if (zip_file == nullptr)
	{
		m_lastErrorDesc = "zip_fopen_index failed¡£index: "+nstring::from(index);
		return false;
	}

	newobj::file write_file;
	if (write_file.open(filepath) == false)
	{
		m_lastErrorDesc = "open file failed¡£filepath: " + filepath;
		return false;
	}
	struct zip_stat stat = m_files[index].stat;
	uint64 iLen = 0;
	uint32 iread = 0;
	char temp_buf[1024];
	while (iLen < stat.size)
	{
	//	iread = zip_fread((zip_file_t*)zip_file, (void*)temp_buf, (zip_int64_t)1024);
		if (iread < 0)
			return true;
		write_file.write(temp_buf,iread);
		iLen += iread;
	}
	return true;
}

uint32 newobj::zip::size()
{
    int result = zip_get_num_files(m_zip);
    if (result < 0)
        return 0;
    return result;
}
#define ANALYSIS_CODE																\
newobj::zip::zip_file_info info;														\
info.index = index;																		\
zip_stat_init(&info.stat);																	\
if (zip_stat_index(m_zip, index, 0, &info.stat) != 0)							\
{																									\
	info.status = false;																	\
}																									\
else																								\
{																									\
	info.status = true;																		\
	info.name = info.stat.name;														\
}																									\
m_files.insert(std::pair<uint32, zip_file_info>(index, info))
newobj::zip::zip_file_info newobj::zip::analysis(uint32 index)
{
	ANALYSIS_CODE;
	return info;
}

void newobj::zip::analysis_noret(uint32 index)
{
	ANALYSIS_CODE;
	//newobj::zip::zip_file_info info;														
	//info.index = index;																		
	//zip_stat_init(&info.stat);																	
	//if (zip_stat_index(m_zip, index, 0, &info.stat) != 0)							
	//{																									
	//	info.status = false;																	
	//}																									
	//else																								
	//{																									
	//	info.status = true;					
	//	std::wstring w = info.stat.name;
	//	info.name = info.stat.name;											
	//}
}
#endif