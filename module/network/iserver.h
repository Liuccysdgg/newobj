#pragma once
#include <string>
#include "public/base.h"
#include "public/buffer.h"
#include "inet.h"
#include "util/lock.h"
namespace newobj
{
	namespace network
	{
		

		/****************************************************************
		 * Class: [接口] 网络服务器
		 ****************************************************************/
		class MNETWORK_API iserver :public base,public inet
		{
		protected:
			/****************************************************************
			 * Fun: 启动
			 * Desc：监听地址，并启动TCP服务器。
			 * Param
			 *			bind_ipaddress		:		绑定IP地址
			 *			bind_port					:		绑定端口
			 *			sportMode				:		运动模式[default=true] 默认开启，尽可能的压榨单机性能
			 *			sport_policy				:		运动模式拒绝策略[default=0] ，高并发情况下出现的多包阻塞，计算机性能紧张情况下的处理方式
			 *																0		= 任然执行
			 *																>0	= 任务数超过该数量后丢弃，默认建议设置为 10000
			 ****************************************************************/
			virtual bool start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode = false,ushort sport_policy = 10000) = 0;
			/****************************************************************
			 * Fun：关闭
			 * Param
			 *			wait_msec				:		等待时间 默认=0为永不超时
			 ****************************************************************/
			virtual bool close(uint32 wait_msec = 0) = 0;
			
			/*取连接数量*/
			virtual uint32 conn_count() = 0;
			
		public:
			/****************************************************************
			 * Fun：取附加数据
			 * Param
			 *			connID						:		发送的客户ID
			 ****************************************************************/
			virtual ptr extra(uint64 connID) = 0;
			/****************************************************************
			 * Fun：置附加数据
			 * Param
			 *			connID						:		发送的客户ID
			 *			extra						:		附加数据
			 ****************************************************************/
			virtual bool extra(uint64 connID, ptr extra) = 0;
			/****************************************************************
			 * Fun：是否为有效连接
			 * Param
			 *			connID						:		发送的客户ID
			 ****************************************************************/
			virtual bool conned(uint64 connID) = 0;
			/****************************************************************
			 * Fun：发送数据
			 * Param
			 *			connID						:		发送的客户ID
			 *			pData						:		发送的数据指针
			 *			dataLen					:		发送的数据长度
			 ****************************************************************/
			virtual void send(uint64 connID, const char* pData, uint32 dataLen) = 0;
			virtual void send(uint64 connID, const buffer& data) = 0;
			/****************************************************************
			 * Fun：断开连接
			 * Param
			 *			connID						:		客户ID
			 ****************************************************************/
			virtual bool disconn(uint64 connID) = 0;
			/****************************************************************
			 * Fun：获取远程地址
			 * Param
			 *			connID						:		客户ID
			 *			ipaddress					:		IP地址
			 *			port							:		端口
			 ****************************************************************/
			virtual bool remote(uint64 connID,nstring& ipaddress,ushort &port) = 0;
			/*取等待队列大小*/
			virtual int32 wait_queue() = 0;
			/*取执行线程数*/
			virtual int32 proc_total() = 0;
		public:
			newobj::mutex m_mutex;
		};



		



		
		

	}
	

}