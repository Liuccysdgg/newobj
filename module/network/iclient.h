#pragma once
#include "inet.h"
#include "public/base.h"
#include "public/buffer.h"
namespace newobj
{
	namespace network
	{
        /****************************************************************
        * Class: TCP客户端接口
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
             * Class：连接
             * Desc：启动并连接服务器
             * Param：
             *      address             ：       服务器IP地址
             *      port                   ：       服务器端口
             *      reconnect          ：       重连【默认10次、间隔500ms】重连阶段状态为在线
             * Return：
             *      TRUE=连接成功 FALSE=连接失败
             *****************************************************************/
            virtual bool start(const nstring& address, uint32 port, bool reconnect = false, bool sportMode = false) = 0;
            /*****************************************************************
             * Class：断开
             *****************************************************************/
            virtual bool disconn() = 0;
            /*****************************************************************
             * Class：停止
             *****************************************************************/
            virtual bool close(uint32 wait_msec = 0) = 0;
            /*****************************************************************
             * Class：连接
             * Desc：用于掉线手动连接
             *****************************************************************/
            virtual bool connect(const nstring& address = "", uint32 port = 0) = 0;
            /*****************************************************************
             * Class：是否已连接
             *****************************************************************/
            virtual bool conned() = 0;
            /*****************************************************************
             * Class：设置远程信息连接信息
             * Param:
             *      address         :           远程服务地址
             *      port               :          远程端口地址
             *      reconn          ：         是否重连
             *****************************************************************/
            virtual void remote(const nstring& address, uint32 port,bool reconn = true) = 0;
            /*****************************************************************
             * Class：发送
             *****************************************************************/
            virtual bool send(const char* buff,uint32 len) = 0;
            virtual void send(const buffer& data) = 0;
        protected:
            /*服务器信息*/
            nstring m_server_ipaddress; 
            ushort m_server_port;
            /****************重连信息****************/
           //重连间隔时间
            uint32 m_reconn_time;
            //重连次数
            uint32 m_reconn_num;
            //重连
            bool m_reconn;
        };


	}
}