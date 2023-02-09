#if 0
#include "bigfile_server.h"
#include "util/bytes.h"
#include "hpsocket/hpsocket.h"
#include "util/codec.h"
#include "util/system.h"

void bigfile_server_OnAccept(newobj::network::iserver* pServer, uint64 connID)
{

}
void bigfile_server_OnClose(newobj::network::iserver* pServer, uint64 connID)
{

}
void bigfile_server_OnRecv(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)
{

}
buffer bigfile_server_OnFilter(newobj::network::iserver* pServer, uint64 connID, const char* buf, uint32 buf_len)
{
    if (buf_len < 9)
        return buffer();
    auto pull = ((ITcpPullServer*)pServer->hp());
	while (true)
	{
        newobj::buffer temp;
        temp.setsize(9);
        if (pull->Peek(connID, (BYTE*)temp.data(), temp.length()) != FR_OK)
            break;
        //std::cout <<"[server][recv] => "<< temp.to_hex() << std::endl;
        if (temp[0] != 0x54 ||
            temp[1] != 0xC0 ||
            temp[2] != 0xBF ||
            temp[3] != 0xAA)
            break;

        auto reqType = (network::ReqType)temp[4];
        int32 data_len = 0;
        bytes::to_int(data_len,temp.data()+5);
        temp.setsize(9+data_len);
        if (pull->Fetch(connID, (BYTE*)temp.data(), data_len + 9) != FR_OK)
            break;
        network::bigfile_server_handle handle(((network::bigfile_server*)pServer->m_temp_data[1]),reqType, temp.right(temp.length() - 9),connID);
	}
    return buffer();
}
void bigfile_server_OnShutdown(newobj::network::iserver* pServer)
{

}


newobj::network::bigfile_server::bigfile_server()
{
    m_server.on_accept(bigfile_server_OnAccept);
    m_server.on_close(bigfile_server_OnClose);
    m_server.on_filter(bigfile_server_OnFilter);
    m_server.on_recv(bigfile_server_OnRecv);
    m_server.on_shutdown(bigfile_server_OnShutdown);

    m_server.m_temp_data[1] = (ptr)this;
}

newobj::network::bigfile_server::~bigfile_server()
{

}

bool newobj::network::bigfile_server::start(const nstring& ipaddress, uint32 port)
{
    return m_server.start(ipaddress,port);
}

bool newobj::network::bigfile_server::append(const nstring& filepath, uint32& filecode)
{
    ____lock____(m_map_mutex, true);
    filecode = m_maker.create();

    FileConf conf;
    conf.file = new file();
    f_ret_f(conf.file->open(filepath));
    conf.path = filepath;
    return m_map.add(filecode, conf);
}

newobj::network::bigfile_server_handle::bigfile_server_handle(bigfile_server* server, ReqType type, const newobj::buffer& data, uint64 connID)
{
    m_server = server;
    m_type = type;
    m_data = data;
    m_connID = connID;

    switch (type)
    {
    case newobj::network::RT_NONE:
        break;
    case newobj::network::RT_GET_FILEINFO:
        fileinfo();
        break;
    case newobj::network::RT_DPACKS:
        dpakcs();
        break;
    default:
        break;
    }
}

newobj::network::bigfile_server_handle::~bigfile_server_handle()
{
}

void newobj::network::bigfile_server_handle::fileinfo()
{
    newobj::json data;
    newobj::json send;
    f_ret(data.parse(m_data.to_string()));
    nstring encode =  codec::des::de(newobj::buffer::from_hex(data["encode"].to<nstring>()), BIGFILE_DES_KEY).to_string();
    uint32 filecode = data["filecode"].to<uint32>();

    ____lock____(m_server->m_map_mutex, true);
    network::bigfile_server::FileConf conf;

    if (m_server->m_map.get(filecode, conf) == false)
    {
        send["result"] = false;
        send["errorcode"] = 1; // 文件不存在
    }
    else
    {
        send["result"] = true;
        send["name"] = "none";
        send["size"] = conf.file->size();
        send["packsize"] = BIGFILE_PACKSIZE;
    }
    reply(send);
}

void newobj::network::bigfile_server_handle::dpakcs()
{
#define SEND_PACK(LENGTH)                                                       \
    sendData.setsize(LENGTH + 6);                                                                       \
    bytes::to_char((char*)sendData.data(), (int32)packIdx);                                         \
    bytes::to_char((char*)sendData.data()+4, (short)LENGTH);                                  \
    if (!conf.file->read(readIdx, LENGTH, (char*)sendData.data() + 6))              \
    {                                                                                                                       \
        reply(RT_ABORT, {});                                                                                    \
        return;                                                                                                         \
    }                                                                                                                       \
    else                                                                                                                  \
    {                                                                                                                       \
        reply(RT_DPACKS, sendData);                                                                       \
    }   



    if (m_data.length() % 4 != 0)
        return;
    if (m_data.length() < 8)
        return;
    int32 filecode = 0;
    int32 packIdx = 0;
    uint32 maxPackIdx = 0;
    network::bigfile_server::FileConf conf;
    bytes::to_int(filecode,m_data.data());

    ____lock____(m_server->m_map_mutex, true);
    f_ret(m_server->m_map.get((uint32)filecode,conf));
    uint32 temp = conf.file->size() / BIGFILE_PACKSIZE;
    uint32 lastLength = conf.file->size() % BIGFILE_PACKSIZE;
    maxPackIdx = lastLength == 0 ? temp : temp + 1;
    
    uint32 loop = m_data.length() / 4 - 1;
    std::cout << "[SERVER][RQ] =====>" << loop << std::endl;
    for (uint32 i = 0; i < loop ; i++)
    {
        bytes::to_int(packIdx, m_data.data()+4+(i*4));
        if (packIdx<0 || packIdx + 1>maxPackIdx)
            return;


        int64 readIdx = packIdx*BIGFILE_PACKSIZE;
        //conf.file->jump(readIdx);
        newobj::buffer sendData;
        if (packIdx == maxPackIdx - 1 && lastLength != 0)
        {
            SEND_PACK(lastLength);
            //system::sleep_msec(1000);
            //reply(RT_DPACKS_OK, {});
        }
        else
        {

            sendData.setsize(BIGFILE_PACKSIZE + 6);                                                                       
            bytes::to_char((char*)sendData.data(), (int32)packIdx );                                         
            bytes::to_char((char*)sendData.data() + 4, (short)BIGFILE_PACKSIZE);
            if (!conf.file->read(readIdx, BIGFILE_PACKSIZE, (char*)sendData.data() + 6))
            {                                                                                                                       
                reply(RT_ABORT, {});                                                                                    
                return;                                                                                                         
            }                                                                                                                       
            else                                                                                                                  
            {                                                                                                                       
                reply(RT_DPACKS, sendData);                                                                       
            }


            //SEND_PACK(BIGFILE_PACKSIZE);
        }
        
    }



}

bool newobj::network::bigfile_server_handle::reply(const newobj::json& data)
{
    return reply(newobj::buffer(data.to_string()));
}

bool newobj::network::bigfile_server_handle::reply(const newobj::buffer& data)
{
    return reply(m_type, data);
}

bool newobj::network::bigfile_server_handle::reply(network::ReqType type, const newobj::buffer& data)
{
    newobj::buffer reqData;
    newobj::buffer lenData;
    lenData.setsize(4);
    bytes::to_char((char*)lenData.data(), (int32)data.length());
    reqData.append({ 0x54,0xC0,0xBF,0xAA });
    reqData.append((uchar)type);
    reqData.append(lenData);
    reqData.append(data);

    try
    {
        //std::cout << "[server][send] => " << reqData.to_hex() << std::endl;
        m_server->m_server.send(m_connID, reqData);
    }
    catch (const newobj::exception& e)
    {
        return false;
    }
    return true;
}
#endif