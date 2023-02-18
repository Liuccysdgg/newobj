#include "http_response.h"
#if USE_NET_HTTP_WEBSITE
#include "hpsocket/hpsocket.h"
#include "util/file.h"
#include "http_util.h"
#include "util/forstring.h"
#include "util/time.h"
#include "util/system.h"
#include "util/codec.h"
#include "http_server.h"
#include "public/define.h"
#include "http_center.h"
#include "http_reqpack.h"
#include "http_router.h"
#include "http_website.h"
#include "util/mem.h"
#include <sys/stat.h>
#define HPSERVER ((IHttpServer*)m_reqpack->server()->hpserver())
#ifdef MSVC_2010
#define HEADER_SET(NAME,VALUE) m_headers.insert(std::pair<nstring,nstring>(NAME,VALUE))
#else
#define HEADER_SET(NAME,VALUE) m_headers.emplace(NAME,VALUE)
#endif
namespace newobj
{
	namespace network
	{
		namespace http
		{
			response::response()
			{
                m_reqpack = nullptr;
                m_response = false;
			}

			response::~response()
			{
				t_ret(m_response == true);
				if (!sjson.is_empty())
				{
					send(sjson);
				}
				else
				{
					send(nstring("The server does not return any data to the browser"), 500, "Internal Server Error");
				}
			}

            void response::init(reqpack* rp)
			{
                this->m_reqpack = rp;
			}
			bool response::send(const char* buf, size_t buf_len, ushort stateNum, const nstring& stateDesc)
			{
				

				newobj::buffer send_data;
				send_data.__set((uchar*)buf, buf_len);
				send_data.deepcopy(false);
#if 1
                // 发送前回调
                {
                    auto sendbefore_callback = m_reqpack->website()->router()->m_callback_sendbefore;
					if (sendbefore_callback != nullptr)
					{
						if (buf_len != 0)
						{
							void* new_data = mem::malloc(buf_len);
							memcpy(new_data,buf, buf_len);
							send_data.__set((uchar*)new_data, buf_len);
							send_data.deepcopy(true);
						}
						sendbefore_callback(&send_data);
					}
                        
                }
#endif
				t_ret_f(m_response == true);
				m_response = true;
				/*合成THeader*/
				/*Content-Type*/
				{
					HEADER_SET("Content-Type", "text/html");
					HEADER_SET("Content-Length", nstring::from((uint64)send_data.length()));

					// 设置默认编码
					{
                        if (m_reqpack->website()->info()->default_codec != "")
                        {
							nstring value;
							auto iter = m_headers.find("Content-Type");
							if (iter == m_headers.end() && iter->second.empty())
								iter->second = "charset=" + m_reqpack->website()->info()->default_codec;
							else
							{
								if (value.find("charset") == -1)
									iter->second += "; charset=" + m_reqpack->website()->info()->default_codec;
							}
								
								
                        }
					}
					// 系统默认协议头
					{
//						auto m = m_server->get_default_response_header().to();
//						for_iter(iter, m)
//						{
//							if (m_headers.exist(iter->first) == false)
//								m_headers.set(iter->first, iter->second);
//						}
					}
				}


				{
					THeader* headers = new THeader[m_headers.size()];
					uint32 idx = 0;
					for_iter(iter, m_headers)
					{
						headers[idx].name = iter->first.c_str();
						headers[idx].value = iter->second.c_str();
						idx++;
					}
					this->m_reqpack->server()->qps()->response(send_data.length());
                    bool result = (bool)HPSERVER->SendResponse((CONNID)m_reqpack->connid(), stateNum, stateDesc.c_str(), headers, (int32)m_headers.size(), (const BYTE*)send_data.data(), send_data.length());
					delete[] headers;
					return result;
				}

			}

			bool response::send(const nstring& value, ushort stateNum, const nstring& stateDesc)
			{
                /*转码为UTF-8*/
				//nstring utf8str = GbkToUtf8(value.c_str());
				return send(value.c_str(), value.length(), stateNum, stateDesc);
			}

			bool response::send(const newobj::json& json, ushort stateNum, const nstring& stateDesc)
			{
				HEADER_SET("Content-Type", "application/json");
				return send(json.to_string(), stateNum, stateDesc);
			}

			bool response::send_file(const nstring& filepath, int32 downbaud)
			{
                auto filepath2 = m_reqpack->website()->info()->rootdir + filepath;
				t_ret_f(m_response == true);
				
				ushort stateCode = 0;
				nstring stateDesc;
				long filesize = 0;
				time_t last_modify_time = 0;

				/*取文件信息*/
				{
					struct stat statbuf;
					if (stat(filepath2.c_str(), &statbuf) != 0)
						return false;
					filesize = statbuf.st_size;
					last_modify_time = statbuf.st_mtime;
				}
				// 设置为已发送
				m_response = true;
				if (filesize != 0)
				{
					stateCode = 200;
					stateDesc = "OK";
				}
				else
				{
                    stateCode = 404;
                    stateDesc = "Not Found";
				}

				/*合成必要头*/
				nstring extName;
				nstring content_type;
				extName = file::ext(filepath2);
				newobj::network::tools::content_type(extName, content_type);
				//headers()->set("Content-Type", content_type,true);
				//时间
				{
					nstring gmt;
					time::now_gmt(gmt);
					HEADER_SET("Date", gmt);
				}
				

				/*判断缓存*/
				if (filecache(last_modify_time))
					return true;

                // 直接发送
                bool direct_send = filesize <= 1024;
                newobj::buffer send_data;

                /*计算断点续传*/
                long start = 0, len = 0;
                if(direct_send == false){
                    if (fileoffset(filesize, start, len))
                        stateCode = 206;
                    HEADER_SET("Content-Length", nstring::from((uint64)len));
                }else{
                    send_data = newobj::file::read(filepath2,OT_R);
                    /*GZIP*/
                    if(m_reqpack->website()->info()->gzip) {
                        send_data = codec::gzip(send_data);
                        HEADER_SET("Content-Encoding", "gzip");
                        HEADER_SET("Content-Length", nstring::from((int64)send_data.length()));
                    }else{
                        HEADER_SET("Content-Length", nstring::from((int64)filesize));
                    }
                }
				{
					THeader* headers = new THeader[m_headers.size()];
					uint32 idx = 0;
					for_iter(iter, m_headers)
					{
						headers[idx].name = iter->first.c_str();
						headers[idx].value = iter->second.c_str();
						idx++;
					}
                    if (!HPSERVER->SendResponse((CONNID)m_reqpack->connid(), stateCode, stateDesc.c_str(), headers, m_headers.size()))
					{
						delete[] headers;
						return false;
					}
					delete[] headers;
				}
				
                if(direct_send){
					if (send_data.length() != 0)
					{
						this->m_reqpack->server()->qps()->response(0);
						return HPSERVER->Send((CONNID)m_reqpack->connid(), (const BYTE*)send_data.data(), send_data.length());
					}
                        
                }
				this->m_reqpack->server()->qps()->response(0);

				/***************计算发送文件**************/
				//读取块大小
				long blocksize = 0;
				if (downbaud == -1)
				{
					downbaud = m_reqpack->website()->info()->download_maxbaud;
					if (downbaud <= 0)
						downbaud = -1;
				}
                    

				if (downbaud == -1)
					blocksize = 4096;
				else
				{
					if (downbaud > 4096)
						blocksize = 4096;
					else
						blocksize = downbaud;
				}
				if (blocksize > len)
					blocksize = len;
				FILE* pFile = nullptr;
#ifdef _WIN32
				fopen_s(&pFile, filepath2.c_str(), "rb");
#else
				pFile = fopen(filepath2.c_str(), "rb");
#endif
				if (pFile == NULL)
					return false;
				fseek(pFile, start, SEEK_SET);

				char* read_buffer = new char[blocksize];
				int readlen = 0;
				timestamp begin_sec = time::now_sec();
				long read_size = 0;

				while ((readlen = fread(read_buffer, 1, blocksize, pFile)) > 0)
				{
					read_size += blocksize;
                    if (HPSERVER->Send((CONNID)m_reqpack->connid(), (const BYTE*)read_buffer, readlen, 0) == false)
					{
						fclose(pFile);
						delete[] read_buffer;
						return false;
					}
					/*判断是否读够了*/
					if (read_size == len)
						break;
					if (len - read_size < blocksize)
					{
						blocksize = len - read_size;
					}
					/*等待，压缩带宽*/
					if (downbaud != -1) {
						while ((time::now_sec()-begin_sec)*downbaud < read_size) {
							system::sleep_msec(100);
						}
					}
				}
				fclose(pFile);
				delete[] read_buffer;

				return true;
			}
			std::map<nstring,nstring>* response::headers()
			{
				return &m_headers;
			}
			bool response::redirect(const nstring& filepath, bool MovedPermanently)
			{
				t_ret_f(m_response == true);
				HEADER_SET("Location", filepath);
				return send(NULL, 0, MovedPermanently == true ? 301 : 302, "Found");
			}

			bool response::forward(const nstring& filepath)
			{
                t_ret_f(m_response == true);
                m_response = true;
				newobj::buffer* newdata = new newobj::buffer;
				 *newdata = *m_reqpack->data();
				 network::http::reqpack* reqpack = network::http::reqpack::create();
				reqpack->init(m_reqpack->url(),m_reqpack->host(),newdata, m_reqpack->connid(), m_reqpack->server());
                reqpack->filepath(filepath);
				m_reqpack->website()->router()->push(reqpack);
                return true;
			}
			bool response::filecache(const uint64& last_modify_time)
			{
				nstring lastModified;
				newobj::time::to_gmt(last_modify_time, lastModified);
				/*对比缓存*/
				LPCSTR lpszValue = nullptr;
                if (HPSERVER->GetHeader((CONNID)m_reqpack->connid(), "If-Modified-Since",&lpszValue))
				{
                    if (lastModified == nstring(lpszValue).trim_end(';'))
					{
						//缓存一致 发送 304
                        return  (bool)HPSERVER->SendResponse((CONNID)m_reqpack->connid(), 304, "Not Modified", nullptr, 0);
					}
				}
				// 最后修改时间
				HEADER_SET("Last-Modified", lastModified);
				// 设置缓存类型
				HEADER_SET("Cache-Control", "no-cache");
				return false;
			}

            bool response::fileoffset(long filesize, long& start, long& len)
			{
				start = 0, len = filesize;

				LPCSTR lpszValue = nullptr;
                if (HPSERVER->GetHeader((CONNID)m_reqpack->connid(), "Range", &lpszValue) == FALSE)
					return false;
				nstring hValue = lpszValue;

				if (hValue.length() < 8)
					return false;
				hValue = hValue.substr(6, hValue.length() - 6);
				auto __arr = hValue.split("-");
				if (__arr.size() == 2)
				{
					start = atol(__arr[0].c_str());
					len = atol(__arr[1].c_str());
					if (len == 0)
						len = filesize - start;
				}
				else
				{
					if (hValue[0] == '-')
					{
						/*下载最后x字节*/
						len = atol(hValue.substr(1, hValue.length() - 2).c_str());
						if (len > filesize)
							return false;
						start = filesize - len;
					}
					else if (hValue[hValue.length() - 1] == '-')
					{
						/*从x字节开始下载*/
						start = atol(hValue.substr(0, hValue.length() - 2).c_str());
						if (start > filesize)
							return false;
						len = filesize - start;
					}
				}
				if (start + len > filesize)
					return false;
				if (len < 0)
					return false;
                HEADER_SET("Content-Range", nstring::from((int64)start) + "-" + nstring::from((int64)len) + "/" + nstring::from((int64)filesize));
				return true;
			}

		}
	}
}
#endif
