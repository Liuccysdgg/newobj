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
			 * Function��У������
			 * Desc�����سɹ�����ִ�У����������ͣ��
			 ****************************************************************/
			virtual bool checkdata(const char* recv_buf, uint32 recv_len) = 0;
			/****************************************************************
			 * Function��ת��Ϊ�ڴ�����
			 ****************************************************************/
			virtual bool to(const char* recv_buf, uint32 recv_len, CONTAINER& contaniner) = 0;
			/****************************************************************
			 * Function��ѡ����
			 ****************************************************************/
			virtual void select(CONTAINER& contaniner) = 0;

			
		private:
			// �ͻ�ID
			uint64 m_connID = 0;
			// ����ָ��
			net::INet* m_net = nullptr;
			// ��������
			CONTAINER m_contaniner;
		};
		

		

	}

}
