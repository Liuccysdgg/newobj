#pragma once
#include "public/define.h"
#include "public/string.h"


namespace newobj
{
	namespace network
	{
		/*Tcp����*/
		enum TcpType
		{
			TTGen,				//��ͨTCP
			TTPack,			//PACKģ��
			TTPull				//PULLģ��
		};

		class MNETWORK_API inet
		{
		public:
			inet();
			/****************************************************************
			 * Fun����ȡHPָ��
			 ****************************************************************/
			ptr hp();
			ptr hp_listener();
			/*�������ȡ*/
			uint32 sport_poliy();
			bool sport_mode();
			void sport_poliy(uint32 sportPolicy);
			void sport_mode(bool sportMode);
			const nstring& name();
			void name(const nstring& name);
		protected:
			nstring m_bind_ipaddress;							// �󶨵�ַ
			ushort m_bind_port;										// �󶨶˿�
			bool m_sportMode;								// ����ģʽ
			uint32 m_sport_policy;						// ����ģʽ�ܾ�����
																					// �߲�������³��ֵĶ����������������ܽ�������µĴ���ʽ
																					// * 0 = ��Ȼִ��
																					// * > 0 = ����������������������Ĭ�Ͻ�������Ϊ 10000 
			ptr m_hp_ptr;											// HP����ָ��
			ptr m_hp_listener_ptr;								// HP���������ָ��
			nstring m_name;											// ����
		public:
			ptr m_temp_data[10]; 	// �������ݣ��û��Զ�����д
		};
	}
}
