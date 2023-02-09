#include "tcp_client.h"
#ifdef USE_NET_TCP_CLIENT
#include "util/system.h"
#include "util/forstring.h"
#include "util/time.h"
#include <string.h>
#include "public/exception.h"
#include <hpsocket/hpsocket.h>
#include "tcp_client_lst.h"

using namespace newobj;





newobj::network::tcp::client::client(network::TcpType type)
{
    m_pfun_onaccept = nullptr;
    m_pfun_onclose = nullptr;
    m_pfun_onrecv = nullptr;
    m_pfun_onfilter = nullptr;
    m_type = type;
}

newobj::network::tcp::client::~client()
{
    close();
}

bool newobj::network::tcp::client::start(const nstring& address, uint32 port, bool reconnect, bool sportMode)
{
    if (start(reconnect, sportMode))
    {
        return connect(address, port, -1);
    }
    return false;
}

bool newobj::network::tcp::client::start(bool reconnect ,bool sportMode)
{
    ____lock____(this->m_mutex, true);
    //判断是否已创建
    if (this->m_hp_ptr != 0)
    {
        this->m_lastErrorDesc = "started";
        return false;
    }
    if (this->m_hp_ptr != 0)
    {
        this->m_lastErrorDesc = "started";
        return false;
    }
    this->m_reconn = reconnect;
    this->m_sportMode = sportMode;
    /*创建HPSocket*/
    tcp_client_lst* listener = new tcp_client_lst(this);
    void* tcpClient = nullptr;
    if (m_type == TTGen)
        tcpClient = HP_Create_TcpClient(listener);
    else if (m_type == TTPack)
        tcpClient = HP_Create_TcpPackClient(listener);
    else if (m_type == TTPull)
        tcpClient = HP_Create_TcpPullClient(listener);

    this->m_hp_listener_ptr = (ptr)listener;
    this->m_hp_ptr = (ptr)tcpClient;
    ::newobj::ithread::start();
    return true;
}

bool newobj::network::tcp::client::disconn()
{
    ____lock____(this->m_mutex,true);
    if (this->m_hp_ptr == 0)
        return false;
    return  (bool)to_ptr(ITcpClient*, this->m_hp_ptr)->Stop();
}

bool newobj::network::tcp::client::close(uint32 wait_msec)
{
    ____lock____(this->m_mutex,true);
    this->m_reconn = false;
    if (!::newobj::ithread::stop())
    {
        trw_str("thread stop failed");
    }
    ::newobj::ithread::wait();
    if (this->m_hp_ptr == 0)
        return true;
    IClient* tcpClient = nullptr;

    if (m_type == TTGen)
        tcpClient = to_ptr(ITcpClient*, this->m_hp_ptr);
    else if (m_type == TTPack)
        tcpClient = to_ptr(ITcpPackClient*, this->m_hp_ptr);
    else if (m_type == TTPull)
        tcpClient = to_ptr(ITcpPullClient*, this->m_hp_ptr);


    tcpClient->Stop();
    tcpClient->Wait();

    if (m_type == TTGen)
        HP_Destroy_TcpClient((ITcpClient*)tcpClient);
    else if (m_type == TTPack)
        HP_Destroy_TcpPackClient((ITcpPackClient*)tcpClient);
    else if (m_type == TTPull)
        HP_Destroy_TcpPullClient((ITcpPullClient*)tcpClient);

    
    delete to_ptr(tcp_client_lst*, this->m_hp_listener_ptr);
    this->m_hp_ptr = 0;
    this->m_hp_listener_ptr = 0;
    return true;
}

bool newobj::network::tcp::client::run()
{
    tcp_client_lst* listener = to_ptr(tcp_client_lst*, this->m_hp_listener_ptr);
    while (this->m_reconn)
    {
        if (listener->m_state == 2)
        {
            system::sleep_msec(this->m_reconn_time);
            continue;
        }
        /*重连*/
        for(uint32 i=0;i<this->m_reconn_num;i++)
        {
            if (to_ptr(tcp_client_lst*, this->m_hp_listener_ptr)->m_state != 2)
            {
                    
                if (connect("",0,-1))
                {
                    break;
                }
                else
                {
                    system::sleep_msec(this->m_reconn_time);
                }
            }
        }
    }
    return false;
}

void newobj::network::tcp::client::on_recv(std::function<void(::newobj::network::iclient*, const char*, uint32)> cb)
{
    this->m_pfun_onrecv = cb;
}

void newobj::network::tcp::client::on_accept(std::function<void(::newobj::network::iclient*)> cb)
{
    this->m_pfun_onaccept = cb;
}

void newobj::network::tcp::client::on_close(std::function<void(::newobj::network::iclient*)> cb)
{
    this->m_pfun_onclose = cb;
}

void newobj::network::tcp::client::on_filter(std::function<void(::newobj::network::iclient*, uint32)> cb)
{
    this->m_pfun_onfilter = cb;
}

void newobj::network::tcp::client::remote(const nstring& address, uint32 port, bool reconn)
{
    this->m_server_ipaddress = address;
    this->m_server_port = port;
    if (reconn)
    {
        disconn();
    }
}

bool newobj::network::tcp::client::send(const char* buff, uint32 len)
{
    if (buff == nullptr || len == 0)
        return false;
    if (this->m_hp_ptr == 0)
        return false;
    if(m_type == TTGen)
        return to_ptr(ITcpClient*, this->m_hp_ptr)->Send((const BYTE*)buff, len, 0);
    else if (m_type == TTPack)
        return to_ptr(ITcpPackClient*, this->m_hp_ptr)->Send((const BYTE*)buff, len, 0);
    else if (m_type == TTPull)
        return to_ptr(ITcpPullClient*, this->m_hp_ptr)->Send((const BYTE*)buff, len, 0);
    return false;
}

void newobj::network::tcp::client::send(const buffer& data)
{
    if (!send(data.data(), data.length()))
    {
        trw_str("send failed");
    }
}

bool newobj::network::tcp::client::conned()
{
    //____lock____(this->m_mutex,true);
    if (this->m_hp_ptr == 0)
        return false;
    tcp_client_lst* listener = to_ptr(tcp_client_lst*, this->m_hp_listener_ptr);
    //return to_ptr(ITcpPackClient*, this->m_hp_ptr)->IsConnected();
    return listener->m_state == 2;
}

bool newobj::network::tcp::client::connect(const nstring& address, uint32 port)
{
    return connect(address, port, -1);
}

bool newobj::network::tcp::client::connect(const nstring& address, uint32 port, int32 wait_msec)
{
    //____lock____(this->m_mutex,true);
    if (address != "")
        this->m_server_ipaddress = address;
    if (port != 0)
        this->m_server_port = port;

    IClient* tcpClient = nullptr;
    if (m_type == TTGen)
        tcpClient = to_ptr(ITcpClient*, this->m_hp_ptr);
    else if (m_type == TTPack)
        tcpClient = to_ptr(ITcpPackClient*, this->m_hp_ptr);
    else if (m_type == TTPull)
        tcpClient = to_ptr(ITcpPullClient*, this->m_hp_ptr);


    tcp_client_lst* listener = to_ptr(tcp_client_lst*, this->m_hp_listener_ptr);
    if (tcpClient->GetState() != SS_STOPPED)
    {
        this->m_lastErrorDesc = "not stopped";
        return false;
    }
    /*连接*/
    if (tcpClient->Start(m_server_ipaddress.c_str(), m_server_port, TRUE) == false)
    {
        this->m_lastErrorDesc = tcpClient->GetLastErrorDesc();
        return false;
    }
    timestamp start_time = time::now_msec();
    while (listener->m_state == 0)
    {
        if (wait_msec != -1)
        {
            if (time::now_msec() - start_time > wait_msec)
                break;
        }
        newobj::system::sleep_msec(10);
    }
    return listener->m_state == 2;
}

#endif