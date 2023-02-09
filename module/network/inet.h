#pragma once
#include "public/define.h"
#include "public/string.h"


namespace newobj
{
	namespace network
	{
		/*Tcp类型*/
		enum TcpType
		{
			TTGen,				//普通TCP
			TTPack,			//PACK模型
			TTPull				//PULL模型
		};

		class MNETWORK_API inet
		{
		public:
			inet();
			/****************************************************************
			 * Fun：获取HP指针
			 ****************************************************************/
			ptr hp();
			ptr hp_listener();
			/*设置与获取*/
			uint32 sport_poliy();
			bool sport_mode();
			void sport_poliy(uint32 sportPolicy);
			void sport_mode(bool sportMode);
			const nstring& name();
			void name(const nstring& name);
		protected:
			nstring m_bind_ipaddress;							// 绑定地址
			ushort m_bind_port;										// 绑定端口
			bool m_sportMode;								// 性能模式
			uint32 m_sport_policy;						// 性能模式拒绝策略
																					// 高并发情况下出现的多包阻塞，计算机性能紧张情况下的处理方式
																					// * 0 = 任然执行
																					// * > 0 = 任务数超过该数量后丢弃，默认建议设置为 10000 
			ptr m_hp_ptr;											// HP服务指针
			ptr m_hp_listener_ptr;								// HP服务监听器指针
			nstring m_name;											// 名称
		public:
			ptr m_temp_data[10]; 	// 附加数据，用户自定义填写
		};
	}
}
