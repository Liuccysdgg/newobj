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
		 * Class: [�ӿ�] ���������
		 ****************************************************************/
		class MNETWORK_API iserver :public base,public inet
		{
		protected:
			/****************************************************************
			 * Fun: ����
			 * Desc��������ַ��������TCP��������
			 * Param
			 *			bind_ipaddress		:		��IP��ַ
			 *			bind_port					:		�󶨶˿�
			 *			sportMode				:		�˶�ģʽ[default=true] Ĭ�Ͽ����������ܵ�ѹե��������
			 *			sport_policy				:		�˶�ģʽ�ܾ�����[default=0] ���߲�������³��ֵĶ����������������ܽ�������µĴ���ʽ
			 *																0		= ��Ȼִ��
			 *																>0	= ����������������������Ĭ�Ͻ�������Ϊ 10000
			 ****************************************************************/
			virtual bool start(const nstring& bind_ipaddress, const ushort& bind_port, bool sportMode = false,ushort sport_policy = 10000) = 0;
			/****************************************************************
			 * Fun���ر�
			 * Param
			 *			wait_msec				:		�ȴ�ʱ�� Ĭ��=0Ϊ������ʱ
			 ****************************************************************/
			virtual bool close(uint32 wait_msec = 0) = 0;
			
			/*ȡ��������*/
			virtual uint32 conn_count() = 0;
			
		public:
			/****************************************************************
			 * Fun��ȡ��������
			 * Param
			 *			connID						:		���͵Ŀͻ�ID
			 ****************************************************************/
			virtual ptr extra(uint64 connID) = 0;
			/****************************************************************
			 * Fun���ø�������
			 * Param
			 *			connID						:		���͵Ŀͻ�ID
			 *			extra						:		��������
			 ****************************************************************/
			virtual bool extra(uint64 connID, ptr extra) = 0;
			/****************************************************************
			 * Fun���Ƿ�Ϊ��Ч����
			 * Param
			 *			connID						:		���͵Ŀͻ�ID
			 ****************************************************************/
			virtual bool conned(uint64 connID) = 0;
			/****************************************************************
			 * Fun����������
			 * Param
			 *			connID						:		���͵Ŀͻ�ID
			 *			pData						:		���͵�����ָ��
			 *			dataLen					:		���͵����ݳ���
			 ****************************************************************/
			virtual void send(uint64 connID, const char* pData, uint32 dataLen) = 0;
			virtual void send(uint64 connID, const buffer& data) = 0;
			/****************************************************************
			 * Fun���Ͽ�����
			 * Param
			 *			connID						:		�ͻ�ID
			 ****************************************************************/
			virtual bool disconn(uint64 connID) = 0;
			/****************************************************************
			 * Fun����ȡԶ�̵�ַ
			 * Param
			 *			connID						:		�ͻ�ID
			 *			ipaddress					:		IP��ַ
			 *			port							:		�˿�
			 ****************************************************************/
			virtual bool remote(uint64 connID,nstring& ipaddress,ushort &port) = 0;
			/*ȡ�ȴ����д�С*/
			virtual int32 wait_queue() = 0;
			/*ȡִ���߳���*/
			virtual int32 proc_total() = 0;
		public:
			newobj::mutex m_mutex;
		};



		



		
		

	}
	

}