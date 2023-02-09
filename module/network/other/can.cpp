//#ifndef _WIN32
//#include "Can.h"
//#include <sys/socket.h>
//#include <linux/can.h>
//#include <netinet/ether.h>
//#include <stdio.h>
//#include "util/system.h"
//#include <net/if.h>  
//#include <net/if_arp.h>  
//#include <arpa/inet.h>  
//#include <sys/ioctl.h>  
//
//#include<stdlib.h>
//#include<errno.h>
//#include<string.h>
//#include<sys/types.h>
//#include<netinet/in.h>
//#include "util/system.h"
//extern "C" {
//
//    int write(int, void*, int);
//
//    int read(int, void*, int);
//
//    int close(int fd);
//};
//newobj::net::Can::Can()
//{
//}
//newobj::net::Can::~Can()
//{
//    StopThread();
//    close(m_fd);
//}
//bool newobj::net::Can::Init(const nstring& can_name, uint32 canID)
//{
//    //CAN ID
//    this->m_id = canID;
//
//
//    struct sockaddr_can addr;
//    struct ifreq ifr;
//    m_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
//    if (m_fd == 0)
//        return false;
//    strcpy(ifr.ifr_name, can_name.c_str());
//    if (ioctl(m_fd, SIOCGIFINDEX, &ifr) != 0) {
//        return false;
//    }
//
//    addr.can_family = AF_CAN;
//    addr.can_ifindex = ifr.ifr_ifindex;
//    if (bind(m_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
//        return false;
//    }
//    return StartThread();
//}
//bool newobj::net::Can::Send(const char* buff, ushort size, uint32 can_flag)
//{
//    if (buff == nullptr || size == 0 || size > 8)
//        return false;
//    struct can_frame frame;
//    frame.can_id = this->m_id | can_flag;
//    frame.can_dlc = size;
//    memcpy(frame.data, buff, size);
//    int sendLen = write(m_fd, (void*)&frame, sizeof(frame));
//    if (sendLen != size)
//        return false;
//    return true;
//}
//bool newobj::net::Can::RunThread()
//{
//    if (this->m_recv_cb == NULL)
//    {
//        sys::SleepMsec(100);
//        return true;
//    }
//    struct can_frame frame;
//    int readByte = read(this->m_fd, &frame, sizeof(frame));
//    if (readByte <= 0)
//        return true;
//    (this->m_recv_cb_class_ptr->*m_recv_cb)(frame.can_id, (const char*)frame.data, frame.can_dlc);
//    return true;
//
//}
//void newobj::net::Can::OnRecv(NewObj* class_ptr, newobj::network::CALLBACK_CAN_RECV recv_cb)
//{
//    this->m_recv_cb_class_ptr = class_ptr;
//    this->m_recv_cb = recv_cb;
//}
//
//#endif
