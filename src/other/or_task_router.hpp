#pragma once
#include "public/define.h"
#include "network/INet.h"
namespace newobj
{
	namespace other
	{


		template<typename CONTAINER>
		class irouter
		{
		public:
			void post(const char * recv_buf,uint32 recv_len,uint64 connID,net::INet* net)
			{
				this->m_net = net;
				this->m_connID = connID;
				if (checkdata(recv_buf, recv_len))
				{
					if (To(recv_buf, recv_len, m_contaniner))
					{
						select(m_contaniner);
					}
				}
			}
			CONTAINER& get()
			{
				return m_contaniner;
			}
			net::INet* inet()
			{
				return m_net;
			}
			uint64 connid()
			{
				return m_connID;
			}
		protected:
			/****************************************************************
			 * Function：校验数据
			 * Desc：返回成功继续执行，否则截至暂停。
			 ****************************************************************/
			virtual bool checkdata(const char* recv_buf, uint32 recv_len) = 0;
			/****************************************************************
			 * Function：转换为期待类型
			 ****************************************************************/
			virtual bool to(const char* recv_buf, uint32 recv_len, CONTAINER& contaniner) = 0;
			/****************************************************************
			 * Function：选择处理
			 ****************************************************************/
			virtual void select(CONTAINER& contaniner) = 0;

			
		private:
			// 客户ID
			uint64 m_connID = 0;
			// 服务指针
			net::INet* m_net = nullptr;
			// 数据容器
			CONTAINER m_contaniner;
		};
		

		

	}

}
