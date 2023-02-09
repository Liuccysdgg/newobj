#if 0
#include "bigfile_client.h"
#include "util/bytes.h"
#include "public/exception.h"
#include "util/json.h"
#include "util/system.h"
#include <Windows.h>
#include "util/codec.h"
#include "util/time.h"
#include "public/decimal.h"
/*int32转四字节流*/
newobj::buffer int32_to_buffer(int32 value)
{
	newobj::buffer lenData;
	lenData.setsize(4);
	bytes::to_char((char*)lenData.data(), (int32)value);
	return lenData;
}
DWORD CreateNullFile(uint64 size, char* pszName)
{
	DWORD dwHigh;
	DWORD dwLow;
	DWORD dwResult = 0;
	const uint64 G4 = UINT64_MAX;

	if (size < G4)
	{
		dwLow = size;
	}
	else
	{
		dwLow = size % G4;
	}
	dwHigh = (size - dwLow) / 4;


	HANDLE hFile = CreateFileA(pszName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return GetLastError();
	}

	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, dwHigh, dwLow, NULL);
	if (NULL == hFileMap)
	{
		return GetLastError();
	}

	CloseHandle(hFileMap);
	CloseHandle(hFile);

	return dwResult;
}
void bigfile_client_onaccept(newobj::network::iclient* pClient)
{

}
void bigfile_client_onclose(newobj::network::iclient* pClient)
{

}
void bigfile_client_onfilter(newobj::network::iclient* pClient,uint32 buf_len)
{
	if (buf_len < 9)
		return;
	auto pull = ((ITcpPullClient*)pClient->hp());
	while (true)
	{
		newobj::buffer temp;
		temp.setsize(9);
		if (pull->Peek((BYTE*)temp.data(), temp.length()) != FR_OK)
			break;
		//std::cout <<"[server][recv] => "<< temp.to_hex() << std::endl;
		if (temp[0] != 0x54 ||
			temp[1] != 0xC0 ||
			temp[2] != 0xBF ||
			temp[3] != 0xAA)
			break;

		auto reqType = (network::ReqType)temp[4];
		int32 data_len = 0;
		bytes::to_int(data_len, temp.data() + 5);
		temp.setsize(9 + data_len);
		if (pull->Fetch((BYTE*)temp.data(), data_len + 9) != FR_OK)
			break;
		network::bigfile_client_handle handle(((network::bigfile_client*)pClient->m_temp_data[1]), reqType, temp.right(temp.length() - 9));
	}

}
newobj::network::bigfile_client::bigfile_client()
{
	
	m_client.on_accept(bigfile_client_onaccept);
	m_client.on_close(bigfile_client_onclose);
	m_client.on_filter(bigfile_client_onfilter);
	m_client.m_temp_data[1] = (ptr)this;
	::newobj::thread::start();
}

newobj::network::bigfile_client::~bigfile_client()
{
}

bool newobj::network::bigfile_client::hello(const nstring& ipaddress, uint32 port, const nstring& privatekey, uint32 filecode)
{
	m_client.close();
	/*建立连接*/
	if (!m_client.start(ipaddress, port, false, false))
	{
		m_error = ERROR_CONNEDT_FAILED;
		return false;
	}
	/*获取文件信息*/
	{
		//m_mutex.lock();
		m_reply[RT_GET_FILEINFO].clear();
		//m_mutex.unlock();
		newobj::json reqData;
		reqData["encode"] = codec::des::en(nstring::from(time::now_time2().hour + time::now_time2().minute),privatekey).to_hex();
		reqData["filecode"] = filecode;
		if (request(RT_GET_FILEINFO, reqData.to_string()) == false)
		{
			m_client.close();
			m_error = ERROR_REQUEST_FAILED;
			m_flag = RT_GET_FILEINFO;
			return false;
		}
		//开始等待
		for (uint32 i = 0; i < 30; i++)
		{
			system::sleep_msec(100);
			//m_mutex.lock();
			if (m_reply[RT_GET_FILEINFO]["recved"].to<bool>())
			{
				if (m_reply[RT_GET_FILEINFO]["result"].to<bool>())
				{
					m_info.name = m_reply[RT_GET_FILEINFO]["name"].to<nstring>();
					m_info.packsize = m_reply[RT_GET_FILEINFO]["packsize"].to<uint32>();
					m_info.size = m_reply[RT_GET_FILEINFO]["size"].to<uint32>();
					m_info.filecode = filecode;
					//m_mutex.unlock();
					return true;
				}
				else
				{
					if (m_reply[RT_GET_FILEINFO]["errorcode"].to<uint32>() == 1)
						m_error = ERROR_FILEINFO_NOTFOUND;
					else if (m_reply[RT_GET_FILEINFO]["errorcode"].to<uint32>() == 2)
						m_error = ERROR_FILEINFO_EXP;
					//m_mutex.unlock();
					return false;
				}
			}
			//m_mutex.unlock();
		}
		return false;
	}
}

bool newobj::network::bigfile_client::download(const nstring& filepath, CALLBACK_BIGFILE_DOWNLOADED downloaded, CALLBACK_BIGFILE_DOWNLOADING downloading)
{
	m_callback_downloaded = downloaded;
	m_callback_downloading = downloading;
	m_recvCache.clear();
	m_filepath = filepath;
	m_file.close();
	// 创建文件开始处理
	{
		file::remove(filepath);
		if (!m_file.open(m_filepath))
		{
			m_error = ERROR_LOCALFILE_CREATE_FAILED;
			return false;
		}
		/*建立接收缓存*/
		{
			uint32 packCount = 0;
			uint32 t1 = m_info.size/m_info.packsize;

			if (m_info.size % m_info.packsize == 0)
				packCount = t1;
			else
				packCount = t1 + 1;
			std::cout << "下载准备中..." << std::endl;
			for (uint32 i = 0; i < packCount; i++)
				m_recvCache.insert(std::pair<uint32,bool>(i, false));
		}
	}
	std::cout << "开始下载请求" << std::endl;
	{
		/*首次请求*/
		std::vector<uint32> reqPackIdxs;
		uint32 loop = m_recvCache.size() > BIGFILE_RQUEST_PS ? BIGFILE_RQUEST_PS : m_recvCache.size();
		for (uint32 i = 0; i < loop ; i++)
			reqPackIdxs.push_back(i);
		dpacks(reqPackIdxs);
	}
	
	return true;
}
newobj::network::bigfile_client::FileInfo newobj::network::bigfile_client::fileinfo()
{
	return m_info;
}

bool newobj::network::bigfile_client::request(ReqType type, const newobj::buffer& data)
{
	newobj::buffer reqData;

	newobj::buffer lenData;
	lenData.setsize(4);
	bytes::to_char((char*)lenData.data(), (int32)data.length());
	
	reqData.append({ 0x54,0xC0,0xBF,0xAA });
	reqData.append((uchar)type);
	reqData.append(lenData);
	reqData.append(data);

	try
	{

		//std::cout << "[client][send] => " << reqData.to_hex() << std::endl;
		m_client.send(reqData);
	}
	catch (const newobj::exception & e)
	{
		return false;
	}
	return true;
}

bool newobj::network::bigfile_client::dpacks(const std::vector<uint32>& packs)
{
	//RT_DPACKS
	newobj::buffer data;
	newobj::buffer intData;


	intData.setsize(4);
	bytes::to_char((char*)intData.data(), (int32)m_info.filecode);
	data.append(intData);
	for (uint32 i = 0; i < packs.size(); i++)
	{
		bytes::to_char((char*)intData.data(),(int32)packs[i]);
		data.append(intData);
	}
	return request(network::RT_DPACKS, data);
}

bool newobj::network::bigfile_client::run()
{
	while (true)
	{
		if (this->m_state == RT_DPACKS)
		{
			/*下载中*/
			if (this->m_recv_pre_msec!= 0 && this->m_recv_pre_msec + 1000 < time::now_msec())
			{
				// 需要重新检索包
				bigfile_client_handle handle(this, RT_DPACKS_OK, {});
			}
		}
		system::sleep_msec(1000);
	}
}

newobj::network::bigfile_client_handle::bigfile_client_handle(bigfile_client* client, ReqType type, const newobj::buffer& data)
{
	m_client = client;
	m_type = type;
	m_data = data;
	m_client->m_state = RT_DPACKS;

	if (data.length() >= 2 && data[0] == '{' && data[data.length() - 1] == '}')
		m_json.parse(data.to_string());


	switch (type)
	{
	case newobj::network::RT_NONE:
		break;
	case newobj::network::RT_GET_FILEINFO:
		fileinfo();
		break;
	case newobj::network::RT_DPACKS:
		dpacks();
		break;
	case newobj::network::RT_DPACKS_OK:
		dpacks_ok();
		break;
	default:
		break;
	}
}

newobj::network::bigfile_client_handle::~bigfile_client_handle()
{
}

void newobj::network::bigfile_client_handle::fileinfo()
{
	std::unique_lock<std::mutex> uni(m_client->m_mutex);
	m_client->m_reply[m_type] = m_json;
	m_client->m_reply[m_type]["recved"] = true;
}

void newobj::network::bigfile_client_handle::dpacks()
{
	std::unique_lock<std::mutex> uni(m_client->m_mutex);
	m_client->m_recv_pre_msec = time::now_msec();
	{
		m_client->m_down_recv_pack++;
		
		double allSize = 0;
		for_iter(iter, m_client->m_recvCache)
		{
			if (iter->second)
				allSize += m_client->m_info.packsize;
		}
		// 总下载百分比
		double per = allSize / m_client->m_info.size;
		per *= 100;
		

		m_client->m_down_per = per;
		
		// 流量计算
		if (m_client->m_down_pre_msec == 0)
			m_client->m_down_pre_msec = time::now_msec();

		if (m_client->m_down_pre_msec + 1000 < time::now_msec())
		{
			
			std::cout << "recvPack => " << m_client->m_down_recv_pack << std::endl;
			if (m_client->m_callback_downloading != nullptr)
			{
				m_client->m_callback_downloading(m_client, m_client->m_down_per, allSize - m_client->m_down_pre_size);
			}

			m_client->m_down_pre_size = allSize;
			m_client->m_down_pre_msec = time::now_msec();
		}
	}
	



	short length = 0;
	int32 packIdx = 0;

	bytes::to_int(packIdx,m_data.data());
	bytes::to_short(length, m_data.data()+4);
	m_client->m_file.jump(packIdx*BIGFILE_PACKSIZE);
	m_client->m_file.write(m_data.right(m_data.length()-6));
	m_client->m_recvCache[packIdx] = true;
	return;
}

void newobj::network::bigfile_client_handle::dpacks_ok()
{
	/*开始结算,判断剩余*/
	std::unique_lock<std::mutex> uni(m_client->m_mutex);
	std::vector<uint32> packs;
	bool reReq = false;
	for_iter(iter, m_client->m_recvCache)
	{
		if (iter->second == false)
		{
			packs.push_back(iter->first);
			if (packs.size() >= 500)
			{
				//std::cout << "=========REQUEST===========" << std::endl;
				reReq = true;
				m_client->dpacks(packs);
				packs.clear();
				break;
			}
		}
			
	}
	if (packs.size() != 0)
	{
		m_client->dpacks(packs);
	}
	else
	{
		if (reReq == false)
		{
			m_client->m_state = RT_DPACKS_OK;
			// 下载完成
			m_client->m_file.close();
			if (m_client->m_callback_downloaded != nullptr)
			{
				m_client->m_callback_downloaded(m_client);
			}
		}
	}

}
#endif