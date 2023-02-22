//#pragma once
//
//
//
////*************MYSQL***************
////#define ON_DB_MYSQL
//
////*************REDIS***************
////#define ON_DB_REDIS
//
////*************HTTP SERVER***************
////#define ON_HTTP_SERVER
//
////*************TCP SERVER***************
////#define ON_TCP_SERVER
//
////*************TCP CLIENT***************
////#define ON_TCP_CLIENT
//
////*************LOG***************
////#define ON_LOG
//
//
//#include <string>
//#include <mutex>
//#include <map>
//
//#include "public/define.h"
//#include "public/base.h"
//#include "public/exception.h"
//
//#include "util/json.h"
//#include "util/file.h"
//#include "util/system.h"
//#include "util/forstring.h"
//
//#ifdef ON_HTTP_SERVER
//#include "network/HttpDef.h"
//#include "network/HttpServer.h"
//#endif
//#ifdef ON_LOG
//#include "log/log_define.h"
//#endif
//#ifdef ON_TCP_CLIENT
//#include "network/TcpClient.h"
//#endif
//#ifdef ON_TCP_SERVER
//#include "network/TcpServer.h"
//#endif
//#ifdef ON_DB_MYSQL
//#include "db/db_mysql.h"
//#endif
//#ifdef ON_DB_REDIS
//#include "db/db_redis.h"
//#endif
//
///*设置错误描述后返回FALSE*/
//#define SET_ERRORDESC_RETF(errordesc)   this->m_lastErrorDesc =errordesc;return false;
///*执行启动停止前后函数*/
//#define EXEC_SSPLFUN(fun_ptr)                                                                  \
//        {ResultStatus rs = fun_ptr;                                                                   \
//        if (!rs.state) { this->m_lastErrorDesc = rs.errorDesc; return false; }}    
//
//namespace callback
//{
//#ifdef ON_TCP_CLIENT
//	namespace tcpclient
//	{
//		void OnAccept(newobj::net::IClient* pClient);
//		void OnClose(newobj::net::IClient* pClient);
//		void OnRecv(newobj::net::IClient* pClient, const char* buf, uint32 buf_len);
//	}
//#endif
//#ifdef ON_TCP_SERVER
//	namespace tcpserver
//	{
//		void OnAccept(newobj::net::IServer* pServer, uint64 connID);
//		void OnClose(newobj::net::IServer* pClient, uint64 connID);
//		void OnHandShake(newobj::net::IServer* pServer, uint64 connID);
//		void OnRecv(newobj::net::IServer* pServer, uint64 connID, const char* buf, uint32 buf_len);
//		void OnDisCard(newobj::net::IServer* pServer, uint64 connID, const char* buf, uint32 buf_len);
//		void OnSend(newobj::net::IServer* pServer, uint64 connID, const char* buf, uint32 buf_len);
//		void OnShutDown(newobj::net::IServer* pServer);
//		Buf OnFilter(newobj::net::IServer* pServer, uint64 connID, const char* buf, uint32 buf_len);
//	}
//#endif
//#ifdef ON_HTTP_SERVER
//	namespace httpserver
//	{
//		bool OnMsgComplete(newobj::net::IServer* pServer, uint64 connID, newobj::net::http::Method method, const nstring& filepath);
//		bool OnDisCard(newobj::net::IServer* pServer, uint64 connID, newobj::net::http::Method method, const nstring& filepath);
//	}
//#endif
//
//}
//
//
//namespace newobj
//{
//	namespace util
//	{
//
//	}
//	namespace net
//	{
//#ifdef ON_HTTP_SERVER
//		class HttpServer;
//#endif
//#ifdef ON_TCP_SERVER
//		class TcpServer;
//#endif
//#ifdef ON_TCP_CLIENT
//		class TcpClient;
//#endif
//		class IClient;
//		class IServer;
//	}
//	namespace db
//	{
//#ifdef ON_DB_MYSQL
//		namespace mysql
//		{
//			class Pool;
//		}
//#endif
//#ifdef ON_DB_REDIS
//		namespace redis
//		{
//			class Pool;
//		}
//#endif
//	}
//	namespace other
//	{
//		class Core;
//		/*返回情况*/
//		struct ResultStatus
//		{
//			//状态
//			bool state = false;
//			//错误描述,一般当state=false时赋值该值
//			nstring errorDesc;
//		};
//		/*启动停止前后调用函数指针*/
//		typedef ResultStatus(Core::* SSPL_FUN)();
//
//
//
//		/**************************************************************
//		 * Class：核心类
//		 * Desc：用于新项目快速构建结构
//		 **************************************************************/
//		class Core :public Base
//		{
//		public:
//			/**************************************************************
//			 * Class：启动
//			 * Desc
//			 *			启动顺序
//			 *					1、加载配置	2、Prev()	3、启动配置	4、Last()
//			 * Param
//			 *			config_filepath			：			配置文件路径。参照：or_core_config_json_template.json
//			 * Return
//			 *			返回false可通过 GetLastErrorDesc 获取最后错误信息
//			 **************************************************************/
//			bool start(const nstring& config_filepath)
//			{
//				f_ret_f(openConfig(config_filepath));
//
//				EXEC_SSPLFUN(this->onPrev());
//
//#ifdef ON_LOG
//				f_ret_f(StartupLog());
//#endif
//#ifdef ON_DB_MYSQL
//				f_ret_f(StartupMysqlPools());
//#endif
//#ifdef ON_DB_REDIS
//				f_ret_f(StartupRedisPools());
//#endif
//#ifdef ON_TCP_SERVER
//				f_ret_f(StartupTcpServers());
//#endif
//#ifdef ON_HTTP_SERVER
//				f_ret_f(StartupHttpServers());
//#endif
//#ifdef ON_TCP_CLIENT
//				f_ret_f(StartupTcpClients());
//#endif
//
//				EXEC_SSPLFUN(this->onLast());
//
//				return true;
//			}
//			/**************************************************************
//			 * Class：停止
//			 * Return
//			 *			返回false可通过 GetLastErrorDesc 获取最后错误信息
//			 **************************************************************/
//			bool stop()
//			{
//#ifdef ON_LOG
//				StopLog();
//#endif
//#ifdef ON_TCP_SERVER
//				StopTcpServers();
//#endif
//#ifdef ON_HTTP_SERVER
//				StopHttpServers();
//#endif
//#ifdef ON_DB_MYSQL
//				
//#endif
//				return false;
//			}
//			/*前调用*/
//			virtual newobj::other::ResultStatus onPrev() = 0;
//			/*后调用*/
//			virtual newobj::other::ResultStatus onLast() = 0;
//			/*取配置JSON*/
//			json& config()
//			{
//				return this->m_config_json;
//			}
//			/*保存配置JSON*/
//			bool saveConfig()
//			{
//				//system("rm -rf res/config.json");
//				this->m_config_file.close();
//				remove(this->m_config_file.filepath().c_str());
//				nstring config_str = this->m_config_json.toFormattedString();
//				return file::write(this->m_config_file.filepath(),config_str.c_str(), config_str.length());
//			}
//#ifdef ON_HTTP_SERVER
//			/**************************************************************
//			 * Class：初始化HTTP服务器控制器
//			 * Desc：请在该函数下编写Http服务器控制器函数
//			 * Param
//			 *			name							：			名称
//			 *			httpServer					：			HTTP服务指针
//			 * Return
//			 *			返回false可通过 GetLastErrorDesc 获取最后错误信息
//			 **************************************************************/
//			virtual void OnHttpServerInitCTL(const nstring& name, newobj::net::HttpServer* httpServer) = 0;
//#endif
//		private:
//#ifdef ON_DB_MYSQL
//			bool StartupMysqlPools()
//			{
//				nstring name;
//				nstring ipaddress;
//				nstring username;
//				nstring password;
//				uint32 port = 0;
//				nstring database;
//				uint32 poolsize = 0;
//				nstring log;
//				bool ssl = false;
//				bool reconnect = true;
//				for(int i = 0;i < this->m_config_json["mysql"].GetArraySize();i++)
//				{
//					if (!this->m_config_json["mysql"][i].Get("name", name))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->name not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("ipaddress", ipaddress))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->ipaddress not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("password", password))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->password not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("database", database))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->database not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("port", port))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->port not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("username", username))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->username not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("ssl", ssl))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->ssl not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("reconnect", reconnect))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->reconnect not found");
//					}
//					if (!this->m_config_json["mysql"][i].Get("poolsize", poolsize))
//					{
//						SET_ERRORDESC_RETF("config mysql[" + nstring::from(i) + "]->poolsize not found");
//					}
//
//					db::mysql::Pool* pool = new db::mysql::Pool;
//					if (pool->Init(ipaddress, username, password, database, port, poolsize) == false)
//					{
//						SET_ERRORDESC_RETF("mysqlpool start failed");
//					}
//#ifdef ON_LOG
//					if (this->m_config_json["mysql"][i].Get("log", log))
//						pool->SetLogPtr(this->m_logs[log]);
//					else
//					{
//						LOG_WARN(0, "No log configuration found for MySQL");
//					}
//#endif
//					this->m_mysql_pools[name] = pool;
//
//				}
//				return true;
//			}
//			bool StopMysqlPools()
//			{
//				for (auto iter : this->m_mysql_pools)
//				{
//					iter.second->Close();
//					delete iter.second;
//				}
//				this->m_mysql_pools.clear();
//				return true;
//			}
//#endif
//#ifdef ON_DB_REDIS
//			bool StartupRedisPools()
//			{
//				nstring name;
//				nstring ipaddress;
//				nstring password;
//				uint32 port = 0;
//				uint32 poolsize = 0;
//				uint32 timout_connect_sec;
//				uint32 timeout_io_sec;
//				for_l(i, this->m_config_json["redis"].GetArraySize())
//				{
//					if (!this->m_config_json["redis"][i].Get("name", name))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->name not found");
//					}
//					if (!this->m_config_json["redis"][i].Get("ipaddress", ipaddress))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->ipaddress not found");
//					}
//					if (!this->m_config_json["redis"][i].Get("password", password))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->password not found");
//					}
//					if (!this->m_config_json["redis"][i].Get("port", port))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->port not found");
//					}
//					if (!this->m_config_json["redis"][i].Get("poolsize", poolsize))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->poolsize not found");
//					}
//					if (!this->m_config_json["redis"][i].Get("timout_connect_sec", timout_connect_sec))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->timout_connect_sec not found");
//					}
//					if (!this->m_config_json["redis"][i].Get("timeout_io_sec", timeout_io_sec))
//					{
//						SET_ERRORDESC_RETF("config redis[" + nstring::from(i) + "]->timeout_io_sec not found");
//					}
//					db::redis::Pool* pool = new db::redis::Pool;
//					if (pool->Init(ipaddress, port, password, poolsize, timout_connect_sec, timeout_io_sec) == false)
//					{
//						SET_ERRORDESC_RETF("redis pool init failed");
//					}
//					// this->m_mysqlpool->SetOverCB(gencb::sql_conn_free);
//					this->m_redis_pools[name] = pool;
//				}
//				return true;
//			}
//			bool StopRedisPools()
//			{
//				for (auto iter : this->m_redis_pools)
//				{
//					delete iter.second;
//				}
//				this->m_redis_pools.clear();
//				return true;
//			}
//#endif
//#ifdef ON_TCP_SERVER
//			bool StartupTcpServers()
//			{
//				for (int i = 0; i < this->m_config_json["server"]["tcp"].GetArraySize();i++)
//				{
//					nstring name;
//					nstring bind_ipaddress;
//					uint32 bind_port = 0;
//					nstring type;
//					bool sport;
//					if (!this->m_config_json["server"]["tcp"][i].Get("type", type))
//					{
//						SET_ERRORDESC_RETF("config server->tcp[" + nstring::from(i) + "]->type not found");
//					}
//					if (!this->m_config_json["server"]["tcp"][i].Get("name", name))
//					{
//						SET_ERRORDESC_RETF("config server->tcp[" + nstring::from(i) + "]->name not found");
//					}
//					if (!this->m_config_json["server"]["tcp"][i].Get("ipaddress", bind_ipaddress))
//					{
//						SET_ERRORDESC_RETF("config server->tcp[" + nstring::from(i) + "]->ipaddress not found");
//					}
//					if (!this->m_config_json["server"]["tcp"][i].Get("port", bind_port))
//					{
//						SET_ERRORDESC_RETF("config server->tcp[" + nstring::from(i) + "]->port not found")
//					}
//					if (!this->m_config_json["server"]["tcp"][i].Get("sport", sport))
//					{
//						SET_ERRORDESC_RETF("config server->tcp[" + nstring::from(i) + "]->sport not found")
//					}
//					newobj::net::TcpServer* tcpServer = nullptr;
//					if (type == "pull")
//					{
//						tcpServer = new newobj::net::TcpServer(newobj::net::TTPull);
//					}
//					else if (type == "pack")
//					{
//						tcpServer = new newobj::net::TcpServer(newobj::net::TTPack);
//					}
//					else if (type == "gen")
//					{
//						tcpServer = new newobj::net::TcpServer(newobj::net::TTGen);
//					}
//					else
//					{
//						SET_ERRORDESC_RETF("config server->tcp[" + nstring::from(i) + "]->type Unknown value");
//					}
//					tcpServer->OnAccept(callback::tcpserver::OnAccept);
//					tcpServer->OnClose(callback::tcpserver::OnClose);
//					tcpServer->OnHandShake(callback::tcpserver::OnHandShake);
//					tcpServer->OnRecv(callback::tcpserver::OnRecv);
//					tcpServer->OnDisCard(callback::tcpserver::OnDisCard);
//					tcpServer->OnSend(callback::tcpserver::OnSend);
//					tcpServer->OnShutDown(callback::tcpserver::OnShutDown);
//					tcpServer->OnFilter(callback::tcpserver::OnFilter);
//
//					if (!tcpServer->Start(bind_ipaddress, bind_port, sport))
//					{
//						this->m_lastErrorDesc = tcpServer->GetLastErrorDesc();
//						return false;
//					}
//					this->m_tcpservers[name] = tcpServer;
//				}
//				return true;
//			}
//			bool StopTcpServers()
//			{
//				for (auto iter : this->m_tcpservers)
//				{
//					iter.second->Close();
//					delete iter.second;
//				}
//				this->m_tcpservers.clear();
//				return true;
//			}
//#endif
//#ifdef ON_HTTP_SERVER
//			bool StartupHttpServers()
//			{
//				for (int i = 0; i < this->m_config_json["server"]["http"].GetArraySize();i++)
//				{
//					nstring name;
//					nstring bind_ipaddress;
//					nstring wwwroot;
//					nstring default_page;
//					uint32 bind_port = 0;
//					net::HttpServer* httpServer = new net::HttpServer();
//
//					if (!this->m_config_json["server"]["http"][i].Get("name", name))
//					{
//						SET_ERRORDESC_RETF("config server->http[" + nstring::from(i) + "]->name not found");
//					}
//					if (!this->m_config_json["server"]["http"][i].Get("ipaddress", bind_ipaddress))
//					{
//						SET_ERRORDESC_RETF("config server->http[" + nstring::from(i) + "]->ipaddress not found");
//					}
//					if (!this->m_config_json["server"]["http"][i].Get("port", bind_port))
//					{
//						SET_ERRORDESC_RETF("config server->http[" + nstring::from(i) + "]->port not found");
//					}
//					if (!this->m_config_json["server"]["http"][i].Get("wwwroot", httpServer->m_wwwroot))
//					{
//						SET_ERRORDESC_RETF("config server->http[" + nstring::from(i) + "]->wwwroot not found");
//					}
//					if (!this->m_config_json["server"]["http"][i]["session"].Get("defaultTimeoutSec", httpServer->m_session_default_timeout))
//					{
//						SET_ERRORDESC_RETF("config server->http[" + nstring::from(i) + "]->session->defaultTimeoutSec not found");
//					}
//
//					/*默认页*/
//					default_page = "";
//					this->m_config_json["server"]["http"][i]["default"].Get("404", default_page);
//					httpServer->AddDefaultPage("404", default_page);
//					default_page = "";
//					this->m_config_json["server"]["http"][i]["default"].Get("def", default_page);
//					httpServer->AddDefaultPage("def", default_page);
//					/*回调初始化控制器*/
//					OnHttpServerInitCTL(name, httpServer);
//					httpServer->OnMsgComplete(callback::httpserver::OnMsgComplete, false);
//					httpServer->OnDisCard(callback::httpserver::OnDisCard);
//					
//					if (httpServer->Start(bind_ipaddress, bind_port, true) == false)
//					{
//						SET_ERRORDESC_RETF(httpServer->GetLastErrorDesc());
//						return false;
//					}
//					this->m_httpservers[name] = httpServer;
//				}
//				return true;
//			}
//			bool StopHttpServers()
//			{
//				for (auto iter : this->m_httpservers)
//				{
//					iter.second->Close();
//					delete iter.second;
//				}
//				this->m_httpservers.clear();
//				return true;
//			}
//#endif
//#ifdef ON_TCP_CLIENT
//			bool StartupTcpClients()
//			{
//				return true;
//			}
//			bool StopTcpClients()
//			{
//				return true;
//			}
//#endif
//#ifdef ON_LOG
//			bool StartupLog()
//			{
//				for (int i = 0; i < this->m_config_json["log"].GetArraySize();i++)
//				{
//					nstring filepath;
//					nstring name;
//					bool everyday = false;
//					if (!this->m_config_json["log"][i].Get("name", name))
//					{
//						this->m_lastErrorDesc = "config log[" + nstring::from(i) + "]->name not found";
//						return false;
//					}
//					if (!this->m_config_json["log"][i].Get("filepath", filepath))
//					{
//						this->m_lastErrorDesc = "config log[" + nstring::from(i) + "]->filepath not found";
//						return false;
//					}
//					//if (!this->m_config_json["log"][i].Get("everyday", everyday))
//					//{
//					//	this->m_lastErrorDesc = "config log[" + nstring::from(i) + "]->everyday not found";
//					//	return false;
//					//}
//					try
//					{
//						ptr log_ptr = newobj::log::Create(filepath, name, false);
//						this->m_logs[name] = log_ptr;
//					}
//					catch (newobj::Exception& e)
//					{
//						e.Print();
//						this->m_lastErrorDesc = "Failed to open the log \"" + filepath + "\", possibly due to insufficient permissions";
//						return false;
//					}
//				}
//
//				return true;
//			}
//			bool StopLog()
//			{
//				for (auto iter : this->m_logs)
//				{
//					newobj::log::Destroy(iter.second);
//				}
//				this->m_logs.clear();
//				return true;
//			}
//#endif
//			/*打开配置JSON*/
//			bool openConfig(const nstring& filepath)
//			{
//				m_config_file.close();
//				/*load config file*/
//				if (m_config_file.open(filepath) == false)
//				{
//					this->m_lastErrorDesc = "load \"" + filepath + "\" failed";
//					return false;
//				}
//				if (m_config_file.size() <= 0)
//				{
//					this->m_lastErrorDesc = "\"" + filepath + "\" content is null";
//					return false;
//				}
//				char* temp_buf = new char[m_config_file.size()];
//				m_config_file.read(0, m_config_file.size(), temp_buf);
//				m_config_file.close();
//				if (!this->m_config_json.parse(temp_buf))
//				{
//					this->m_lastErrorDesc = "\"" + filepath + "\" content not json";
//					delete[] temp_buf;
//					return false;
//				}
//				delete[] temp_buf;
//				return true;
//			}
//			
//
//		private:
//			/*配置文件路径*/
//			File m_config_file;
//			/*配置JSON*/
//			json m_config_json;
//		public:
//#ifdef ON_TCP_CLIENT
//			/*TCP客户组*/
//			std::map<nstring, newobj::net::TcpClient*> m_tcpclients;
//#endif
//#ifdef ON_TCP_SERVER
//			/*TCP服务组*/
//			std::map<nstring, newobj::net::TcpServer*> m_tcpservers;
//#endif
//#ifdef ON_HTTP_SERVER
//			/*HTTP服务组*/
//			std::map<nstring, newobj::net::HttpServer*> m_httpservers;
//#endif
//#ifdef ON_DB_MYSQL
//			/*MYSQL连接池*/
//			std::map<nstring, newobj::db::mysql::Pool*> m_mysql_pools;
//#endif
//#ifdef ON_DB_REDIS
//			/*REDIS连接池*/
//			std::map<nstring, newobj::db::redis::Pool*> m_redis_pools;
//#endif
//#ifdef ON_LOG
//			/*日志*/
//			std::map<nstring, ptr> m_logs;
//#endif
//		};
//
//	}
//}