#include "tcp_client_lst.h"
#ifdef USE_NET_TCP_CLIENT
#include "tcp_client.h"
#include "public/exception.h"
newobj::network::tcp::tcp_client_lst::tcp_client_lst(client* tcpClient)
{
    m_threadpool = nullptr;
    m_state = 0;
    this->m_tcpClient = tcpClient;
    /*是否为性能模式，=true 则开启线程池*/
    if (this->m_tcpClient->sport_mode())
    {
        m_state = 0;
        m_threadpool = NULL;

        //创建Socket任务线程池
        this->m_threadpool = HP_Create_ThreadPool();
        if (this->m_threadpool->Start(
            0,
            this->m_tcpClient->sport_poliy(),
            TRP_WAIT_FOR, 0) == false)
        {
            //销毁Socket任务线程池
            HP_Destroy_ThreadPool(this->m_threadpool);
            this->m_threadpool = nullptr;
            trw_str("Threadpool Init failed");
        }
    }
}
newobj::network::tcp::tcp_client_lst::~tcp_client_lst()
{
    if (this->m_threadpool != NULL)
    {
        if (!this->m_threadpool->Stop(60000))
        {
            //("ThreadPool Stop failed");
        }
        HP_Destroy_ThreadPool(this->m_threadpool);
    }
}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket)
{

    return HR_OK;

}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
    return HR_OK;

}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
    //if (this->m_tcpClient->m_pfun_onrecv != nullptr)
    //    this->m_tcpClient->m_pfun_onrecv(this->m_tcpClient,(const char*)pData,iLength);

    if (iLength <= 0)
        return HR_OK;
    /*回调*/
    if (m_tcpClient->m_pfun_onrecv != NULL)
    {
        /*是否为性能模式，选择是否使用线程*/
        if (this->m_tcpClient->sport_mode())
        {

            TcpClientRecvSt* tsrs = new TcpClientRecvSt;
            if (pData != NULL)
            {
                tsrs->pData = new char[iLength];
                memcpy(tsrs->pData, (const char*)pData, iLength);
            }
            tsrs->iLength = iLength;

            tsrs->pClient = (iclient*)this->m_tcpClient;
            tsrs->callback = this->m_tcpClient->m_pfun_onrecv;
            /*启动线程池*/
            if (this->m_threadpool->Submit(&callback_thread_recv_tcpclient, (PVOID)tsrs) == false)
            {
                //失败释放资源
                if (tsrs->pData != NULL)
                    delete[] tsrs->pData;
                delete tsrs;
                trw_str("threadpol submit failed!");
            }
        }
        else
        {
            m_tcpClient->m_pfun_onrecv(this->m_tcpClient, (const char*)pData, iLength);
        }
    }
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
    this->m_state = 1;
    if (this->m_tcpClient->m_pfun_onclose != nullptr)
        this->m_tcpClient->m_pfun_onclose(this->m_tcpClient);
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength)
{
    if (this->m_tcpClient->m_pfun_onfilter != nullptr)
        this->m_tcpClient->m_pfun_onfilter(this->m_tcpClient, iLength);
    return HR_OK;
}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnConnect(ITcpClient* pSender, CONNID dwConnID)
{
    this->m_state = 2;
    if (this->m_tcpClient->m_pfun_onaccept != nullptr)
        this->m_tcpClient->m_pfun_onaccept(this->m_tcpClient);
    return HR_OK;

}
EnHandleResult newobj::network::tcp::tcp_client_lst::OnHandShake(ITcpClient* pSender, CONNID dwConnID)
{

    return HR_OK;
}
void __HP_CALL newobj::network::tcp::callback_thread_recv_tcpclient(PVOID param)
{
    TcpClientRecvSt* tsrs = (TcpClientRecvSt*)param;
    tsrs->callback(tsrs->pClient, tsrs->pData, tsrs->iLength);
    if (tsrs->pData != NULL)
        delete[] tsrs->pData;
    delete tsrs;
}
#endif