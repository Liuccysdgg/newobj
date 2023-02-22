#pragma once
#include "inet.h"
#include "public/base.h"
#include "public/buffer.h"
namespace newobj
{
	namespace network
	{
        /****************************************************************
        * Class: TCP�ͻ��˽ӿ�
        ****************************************************************/
        class MNETWORK_API iclient :public base, public inet
        {
        public:
            iclient()
            {
                m_server_port = 0;
                m_reconn_time = 500;
                m_reconn_num = 10;
                m_reconn = false;
            }
            ~iclient()
            {

            }
            /*****************************************************************
             * Class������
             * Desc�����������ӷ�����
             * Param��
             *      address             ��       ������IP��ַ
             *      port                   ��       �������˿�
             *      reconnect          ��       ������Ĭ��10�Ρ����500ms�������׶�״̬Ϊ����
             * Return��
             *      TRUE=���ӳɹ� FALSE=����ʧ��
             *****************************************************************/
            virtual bool start(const nstring& address, uint32 port, bool reconnect = false, bool sportMode = false) = 0;
            /*****************************************************************
             * Class���Ͽ�
             *****************************************************************/
            virtual bool disconn() = 0;
            /*****************************************************************
             * Class��ֹͣ
             *****************************************************************/
            virtual bool close(uint32 wait_msec = 0) = 0;
            /*****************************************************************
             * Class������
             * Desc�����ڵ����ֶ�����
             *****************************************************************/
            virtual bool connect(const nstring& address = "", uint32 port = 0) = 0;
            /*****************************************************************
             * Class���Ƿ�������
             *****************************************************************/
            virtual bool conned() = 0;
            /*****************************************************************
             * Class������Զ����Ϣ������Ϣ
             * Param:
             *      address         :           Զ�̷����ַ
             *      port               :          Զ�̶˿ڵ�ַ
             *      reconn          ��         �Ƿ�����
             *****************************************************************/
            virtual void remote(const nstring& address, uint32 port,bool reconn = true) = 0;
            /*****************************************************************
             * Class������
             *****************************************************************/
            virtual bool send(const char* buff,uint32 len) = 0;
            virtual void send(const buffer& data) = 0;
        protected:
            /*��������Ϣ*/
            nstring m_server_ipaddress; 
            ushort m_server_port;
            /****************������Ϣ****************/
           //�������ʱ��
            uint32 m_reconn_time;
            //��������
            uint32 m_reconn_num;
            //����
            bool m_reconn;
        };


	}
}