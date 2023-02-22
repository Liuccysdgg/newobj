#include "http_util.h"
#if USE_NET_HTTP_UTIL
#include "util/forstring.h"
#include "hpsocket/hpsocket.h"
#include <regex>

#include "hpsocket/hpsocket.h"
#ifdef _WIN32
#include <sys/stat.h>
#include <sys/timeb.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <locale>
#include "psapi.h"
#include <tcpmib.h>
#include <IPHlpApi.h>
#include <WinInet.h>
#else
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h>
#endif
#include "network/http/http_client.h"
#include <cmath>
#include "util/math.h"
#include <string.h>
void newobj::network::tools::content_type(const nstring& extName, nstring& type)
{
    {
        if (extName == "html")
        {
            type = "text/html";
        }
        else if (extName == "htm")
        {
            type = "text/html";
        }
        else if (extName == "css")
        {
            type = "text/css";
        }
        else if (extName == "text")
        {
            type = "text/plain";
        }
        else if (extName == "icon")
        {
            type = "image/x-icon";
        }
        else if (extName == "jpeg")
        {
            type = "image/jpeg";
        }
        else if (extName == "mp3")
        {
            type = "audio/mpeg";
        }
        else if (extName == "pdf")
        {
            type = "application/pdf";
        }
        else if (extName == "swf")
        {
            type = "application/x-shockwave-flash";
        }
        else if (extName == "wav")
        {
            type = "audio/x-wav";
        }
        else if (extName == "zip")
        {
            type = "application/zip";
        }
        else if (extName == "mp4")
        {
            type = "video/mpeg4";
        }
        else if (extName == "png")
        {
            type = "image/png";
        }
        else if (extName == "rmvb")
        {
            type = "application/vnd.rn-realmedia-vbr";
        }
        else if (extName == "js")
        {
            type = "application/x-javascript";
        }
        else if (extName == "ttf")
        {
            type = "application/octet-stream";
        }
        else if (extName == "woff")
        {
            type = "application/x-font-woff";
        }
        else if (extName == "woff2")
        {
            type = "application/x-font-woff";
        }
        else
        {
            type = extName;
        }


    }
}

bool newobj::network::tools::parse_url(const nstring& url, nstring& httpType,nstring&host, nstring& ipaddress, ushort& port, nstring& urlField)
{
    nstring temp;
    if (url.length() < 10)
        return false;
    if (url.substr(0, 7) == "http://")
    {
        httpType = "http://";
        temp = url.substr(7, url.length() - 7);
    }
    else if (url.substr(0, 8) == "https://")
    {
        httpType = "https://";
        temp = url.substr(8, url.length() - 8);
    }
    else
    {
        temp = url;
        httpType = "http://";
    }

    std::vector<nstring_view> __arr = temp.split("/");
    if (__arr.size() == 2)
    {
        temp = __arr[0];
        urlField = __arr[1];
    }
    __arr = temp.split(":");
    if (__arr.size() == 2)
    {
        host = __arr[0];
        ipaddress = newobj::network::tools::to_ip(__arr[0]);
        port = atoi(__arr[1].c_str());
    }
    else
    {
        host = temp;
        ipaddress = temp;
        if(httpType == "https://")
            port = 443;
        else
            port = 80;
    }
    return true;
}
nstring newobj::network::tools::to_ip(const nstring& url)
{
#ifdef WIN32
	WORD wVersionRequested = 0;
	WSADATA wsaData = {};
	int err = 0;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return "";
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return "";
	}
	struct hostent* p = gethostbyname(url.c_str());//根据主机名得到主机信息
	if (p == NULL)
	{
		return "";
	}
	struct in_addr myaddr;
	memcpy(&myaddr.s_addr, p->h_addr, sizeof(p->h_addr));
    WSACleanup();
	return inet_ntoa(myaddr);
#else
    struct hostent *hptr;
    hptr = gethostbyname(url.c_str());
    if (hptr == nullptr)
        hstrerror(h_errno);
        return "";
    if (hptr->h_addrtype != AF_INET)
        return "";
    char **pptr = hptr->h_addr_list;
    if (*pptr == nullptr)
        return "";
    char str[INET_ADDRSTRLEN+1];
    memset(str,0,INET_ADDRSTRLEN+1);
    inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
    return str;
#endif	
}

bool newobj::network::tools::tcp_status(ushort port, ulong& status)
{
#ifdef _WIN32
	MIB_TCPTABLE TcpTable[10240];
	DWORD nSize = sizeof(TcpTable);
	auto ret = GetTcpTable(&TcpTable[0], &nSize, TRUE);
	if (NO_ERROR == ret)
	{
		DWORD nCount = TcpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for (DWORD i = 0; i < nCount; i++)
			{
				MIB_TCPROW TcpRow = TcpTable[0].table[i];
				DWORD temp1 = TcpRow.dwLocalPort;
				int temp2 = temp1 / 256 + (temp1 % 256) * 256;
				if (temp2 == port)
				{
					status = TcpRow.dwState;
					return true;
				}
			}
		}
		return false;
	}
	return false;
#else
	return false;
#endif
}

void newobj::network::tools::tcp_list(std::list<TcpConf>& list)
{
#ifdef _WIN32
	auto ToPort = [](DWORD param)
	{
		return  param / 256 + (param % 256) * 256;
	};
	MIB_TCPTABLE TcpTable[10240];
	DWORD nSize = sizeof(TcpTable);
	auto ret = GetTcpTable(&TcpTable[0], &nSize, TRUE);
	if (NO_ERROR == ret)
	{
		DWORD nCount = TcpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for (DWORD i = 0; i < nCount; i++)
			{
				MIB_TCPROW TcpRow = TcpTable[0].table[i];
				DWORD temp1 = TcpRow.dwLocalPort;

				TcpConf tc;

				tc.__local_ipaddress = TcpRow.dwLocalAddr;
				tc.local_port = ToPort(TcpRow.dwLocalPort);
				tc.__remote_ipaddress = TcpRow.dwRemoteAddr;
				tc.remote_port = ToPort(TcpRow.dwRemotePort);

				list.push_back(tc);
			}
		}
	}
#endif

}
void newobj::network::tools::to_string(uint32 int_ip, nstring& ipaddress)
{
	unsigned char* pch = (unsigned char*)&int_ip;
	char szIPAddr[64] = { 0 };

	int index = 0;
	char number[4];
	for (uint32 i = 0; i < 4; i++)
	{
		memset(number, 0, 4);

		sprintf(number, "%d", *(pch + i));
		memcpy(szIPAddr + index, number, strlen(number));
		if (i == 3)
			break;
		index += strlen(number);
		szIPAddr[index] = '.';
		index++;
	}
	ipaddress = szIPAddr;
}
std::vector<nstring> newobj::network::tools::iplist()
{
	std::vector<nstring> vdata;
#ifdef _WIN32

	WSADATA wsaData;
	int iResult;
	DWORD dwRetval;
	struct addrinfo* result = nullptr;
	struct addrinfo* ptr = nullptr;
	struct addrinfo hints;
	struct addrinfo* cur = nullptr;
	struct sockaddr_in* pSockaddr;
	int temp = 0;

	//初始化Winsock
	memset(&hints, 0, sizeof(struct addrinfo));
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
		return vdata;

	//设置hints信息
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;

	//调用getaddrinfo()如果成功返回地址信息列表
	char hostname[50];
	gethostname(hostname, 50);  //获得主机名称
	dwRetval = getaddrinfo(hostname, nullptr, &hints, &result);  //利用主机名称获取本地地址

	for (cur = result; cur != NULL; cur = cur->ai_next)
	{
		//将本地地址转换成字符串显示
		pSockaddr = (sockaddr_in*)cur->ai_addr;
		const char* pIP = inet_ntoa(pSockaddr->sin_addr);
		vdata.push_back(pIP);
	}

	//清理
	freeaddrinfo(result);
	WSACleanup();
	return vdata;
#else
	return vdata;
#endif
}

nstring newobj::network::tools::size_name(double size, uint32 fixe)
{
	double num = 0.00;
	nstring unit;
	if (size <= 1024)
	{
		unit = "B";
		num = size;
	}
	else if (size <= pow((double)1024, (double)2))
	{
		num = size / 1024;
		unit = "KB";
	}
	else if (size <= pow((double)1024, (double)3))
	{
		num = size / pow((double)1024, (double)2);
		unit = "MB";
	}
	else if (size <= pow((double)1024, (double)4))
	{
		num = size / pow((double)1024, (double)3);
		unit = "GB";
	}
	else if (size <= pow((double)1024, (double)5))
	{
		num = size / pow((double)1024, (double)4);
		unit = "TB";
	}
	else
		return "max";

	return nstring::from(math::to_fixed(num, fixe)) + unit;
}


bool network::tools::parse_url_host(const nstring &url, nstring &host)
{
    nstring temp;
    if (url.length() < 10)
        return false;
    if (url.substr(0, 7) == "http://")
        temp = url.substr(7, url.length() - 7);
    else if (url.substr(0, 8) == "https://")
        temp = url.substr(8, url.length() - 8);

    std::vector<nstring_view> __arr = temp.split("/");
    if (__arr.size() >= 1)
    {
        temp = __arr[0];
    }
    __arr = temp.split(":");
    if (__arr.size() == 2)
        host = __arr[0];
    else
        host = temp;
    return true;
}

#endif
