#include "system.h"
#include "log/log_define.h"
#include "forstring.h"
#include "util/bytes.h"
#include "util/json.h"
#include <string.h>
#include "util/file.h"
#include "util/print.h"
#include "util/time.h"
#include <iostream>
#ifdef LIB_QT
#ifdef QT5
#include "QtGui/qscreen.h"
#endif
#endif
#ifdef _WIN32

#include <sys/stat.h>
#include <sys/timeb.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <lm.h>
#include <tlhelp32.h>
#include <locale>
#include "psapi.h"
#include <ShellAPI.h>
#include <ShlObj.h>
#include <DXGI.h>  
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "DXGL.lib")
//#pragma warning(disable:4091)
#else
#include <unistd.h>
#include <sys/time.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/route.h>
#endif
#include "util/codec.h"
#include <time.h>
#ifndef _WIN32
#ifdef LIB_QT
#include "QtWidgets/qapplication.h"
#endif
#endif
namespace newobj
{
    namespace system
    {
            void sleep_msec(unsigned int time)
            {
#ifdef _WIN32
                Sleep(time);
#else
                usleep(time * 1000);
#endif // _WIN32
            }

            nstring netcard::mac(nstring name)
            {
#ifdef WIN32
                auto GetAdapterInfo = [](int adapterNum, nstring& macOUT)->bool {
                    NCB Ncb;
                    memset(&Ncb, 0, sizeof(Ncb));
                    Ncb.ncb_command = NCBRESET; // �����������Ա����ǿ��Բ�ѯ
                    Ncb.ncb_lana_num = adapterNum;
                    if (Netbios(&Ncb) != NRC_GOODRET)
                        return false;
                    // ׼��ȡ�ýӿڿ���״̬��
                    memset(&Ncb, sizeof(Ncb), 0);
                    Ncb.ncb_command = NCBASTAT;
                    Ncb.ncb_lana_num = adapterNum;
                    strcpy((char*)Ncb.ncb_callname, "*");
                    struct ASTAT
                    {
                        ADAPTER_STATUS adapt;
                        NAME_BUFFER nameBuff[30];
                    }adapter;
                    memset(&adapter, sizeof(adapter), 0);
                    Ncb.ncb_buffer = (unsigned char*)&adapter;
                    Ncb.ncb_length = sizeof(adapter);
                    if (Netbios(&Ncb) != 0)
                        return false;

                    char acMAC[32];
                    sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
                        int(adapter.adapt.adapter_address[0]),
                        int(adapter.adapt.adapter_address[1]),
                        int(adapter.adapt.adapter_address[2]),
                        int(adapter.adapt.adapter_address[3]),
                        int(adapter.adapt.adapter_address[4]),
                        int(adapter.adapt.adapter_address[5]));
                    macOUT = acMAC;
                    return true;
                };
                    // ȡ�������б�
                    LANA_ENUM adapterList;
                    NCB Ncb;
                    memset(&Ncb, 0, sizeof(NCB));
                    Ncb.ncb_command = NCBENUM;
                    Ncb.ncb_buffer = (unsigned char*)&adapterList;
                    Ncb.ncb_length = sizeof(adapterList);
                    Netbios(&Ncb);

                    nstring result;
                    // ȡ��MAC
                    for (int i = 0; i < adapterList.length; ++i)
                    {
                        if (GetAdapterInfo(adapterList.lana[i], result))
                            return result;
                    }
                    return "";
#else
                struct ifreq ifreq;
                char mac[64];
                memset(mac, 0, 64);
                int sock = 0;
                if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                    return "";
                strcpy(ifreq.ifr_name, name.c_str());
                if (ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0)
                    return "";

                SPRINTF(mac, "%02x:%02x:%02x:%02x:%02x:%02x\n ",
                    (unsigned   char)ifreq.ifr_hwaddr.sa_data[0],
                    (unsigned   char)ifreq.ifr_hwaddr.sa_data[1],
                    (unsigned   char)ifreq.ifr_hwaddr.sa_data[2],
                    (unsigned   char)ifreq.ifr_hwaddr.sa_data[3],
                    (unsigned   char)ifreq.ifr_hwaddr.sa_data[4],
                    (unsigned   char)ifreq.ifr_hwaddr.sa_data[5]);
                return mac;
#endif
            }
            std::vector<NetCard> netcard::list()
            {
                std::vector<NetCard> netCardList;
#ifdef _WIN32
                return netCardList;
#else
                struct ifaddrs* addrs = nullptr;

                if (getifaddrs(&addrs) != 0)
                    return netCardList;
                for (struct ifaddrs* node = addrs; node != nullptr; node = node->ifa_next)
                {
                    if (node->ifa_addr != nullptr && node->ifa_addr->sa_family == AF_INET)
                    {
                        NetCard netCard;
                        struct sockaddr_in* sin = nullptr;
                        //������
                        netCard.name = node->ifa_name;
                        //IP��ַ
                        sin = (struct sockaddr_in*)node->ifa_addr;
                        netCard.ipaddress = inet_ntoa(sin->sin_addr);
                        //����
                        sin = (struct sockaddr_in*)node->ifa_dstaddr;
                        netCard.gateway = inet_ntoa(sin->sin_addr);
                        //��������
                        sin = (struct sockaddr_in*)node->ifa_netmask;
                        netCard.netmask = inet_ntoa(sin->sin_addr);
                        //������ַ
                        netCard.mac = system::netcard::mac(netCard.name);
                        netCardList.push_back(netCard);
                    }
                }
                freeifaddrs(addrs);
                return netCardList;
#endif
            }
            bool netcard::set(const NetCard& netCard)
            {
#ifdef _WIN32
                return false;
#else
                const int UP = 1;
                const int DOWN = 0;
                int fd = socket(AF_INET, SOCK_DGRAM, 0);
                if (fd < 0) {
                    //LOG_ERROR(0, strerror(errno));
                    return false;
                }

                do {
                    /*
                     * @ ����MAC
                     */
                    int flag = DOWN;
                    struct ifreq ifr;
                    ifr.ifr_addr.sa_family = AF_INET;
                    strncpy(ifr.ifr_name, netCard.name.c_str(), IFNAMSIZ - 1);

                    if ((ioctl(fd, SIOCGIFFLAGS, &ifr)) == 0) {
                        if (flag == DOWN)
                            ifr.ifr_flags &= ~IFF_UP;
                        else if (flag == UP)
                            ifr.ifr_flags |= IFF_UP;
                    }
                    if ((ioctl(fd, SIOCSIFFLAGS, &ifr)) != 0) {
                        //LOG_ERROR(0, strerror(errno));
                    }
                    //sleep(1);

                    /*��������MAC��ַ*/
                    /*unsigned char mac[6];
                    ifr.ifr_addr.sa_family = ARPHRD_ETHER;
                    memcpy((unsigned char*)ifr.ifr_hwaddr.sa_data, mac, 6);
                    if (ioctl(fd, SIOCSIFHWADDR, &ifr) < 0) {
                        LOG_ERROR(0, strerror(errno));
                        break;
                    }*/

                    sleep(1);
                    flag = UP;
                    // ifr.ifr_addr.sa_family = AF_INET;
                    // strncpy(ifr.ifr_name, networkCardName, IFNAMSIZ - 1 );

                    if ((ioctl(fd, SIOCGIFFLAGS, &ifr)) == 0) {
                        if (flag == DOWN)
                            ifr.ifr_flags &= ~IFF_UP;
                        else if (flag == UP)
                            ifr.ifr_flags |= IFF_UP;
            }
                    if ((ioctl(fd, SIOCSIFFLAGS, &ifr)) != 0) {
                        //LOG_ERROR(0, strerror(errno));
                    }
                    //LSInfo("eth0 up!\n");
                    sleep(1);

                    /*
                     * @ ������������
                     */
                     // struct ifreq ifr;
                    memset(&ifr, 0, sizeof(struct ifreq));
                    strcpy(ifr.ifr_name, netCard.name.c_str());
                    struct sockaddr_in* sin;
                    sin = (struct sockaddr_in*)&ifr.ifr_addr;
                    sin->sin_family = AF_INET;

                    /*
                     * @ ����IP
                     */
                    sin->sin_addr.s_addr = inet_addr(netCard.ipaddress.c_str());
                    if (ioctl(fd, SIOCSIFADDR, &ifr) < 0) {
                        //LOG_ERROR(0, strerror(errno));
                        break;
                    }

                    /*
                     * @ ������������
                     */
                    sin->sin_addr.s_addr = inet_addr(netCard.netmask.c_str());
                    if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0) {
                        //LOG_ERROR(0, strerror(errno));
                        break;
                    }

                    /*
                     * @ ��������
                     */
                    struct rtentry rt;
                    memset(&rt, 0, sizeof(struct rtentry));
                    memset(sin, 0, sizeof(struct sockaddr_in));
                    sin->sin_family = AF_INET;
                    sin->sin_port = 0;
                    sin->sin_addr.s_addr = inet_addr(netCard.gateway.c_str());
                    memcpy(&rt.rt_gateway, sin, sizeof(struct sockaddr_in));
                    ((struct sockaddr_in*)&rt.rt_dst)->sin_family = AF_INET;
                    ((struct sockaddr_in*)&rt.rt_genmask)->sin_family = AF_INET;
                    rt.rt_flags = RTF_GATEWAY;
                    ioctl(fd, SIOCADDRT, &rt);

                    ::close(fd);
                    return true;
    } while (0);
    ::close(fd);
    return false;
#endif
            }
            int32 shell(const nstring& cmd, nstring& result,const nstring& temp_filepath)
            {
                file::remove(temp_filepath);
                result  = shell(cmd+">> \""+ temp_filepath +"\" && echo ----%ERRORLEVEL%");
                
                size_t idx = result.rfind("----");
                int32 ret = false;
                if (idx == -1)
                    ret = -1;
                else
                    ret = 0;

                /*��ȡ�ļ�*/
                uint64 size = 0;
                newobj::file::size(temp_filepath, &size);
                file f;
                if (f.open(temp_filepath) == false)
                {
                    //LOG_ERROR(0, "open "+ temp_filepath +" failed");
                    file::remove(temp_filepath);
                    return ret;
                }
                if (size == 0)
                {
                    f.close();
                    file::remove(temp_filepath);
                    return ret;
                }
                    
                char* read_buf = new char[(unsigned int)size +1];
                read_buf[size] = '\0';
                f.read(0, size,read_buf);
                result = read_buf;
                f.close();
                file::remove(temp_filepath);
                return ret;
                    
                //result = result.substr(idx + 4, result.size() - idx + 4);
                //return 0;
                //return atoi(shell("echo %ERRORLEVEL%").c_str());
            }
            nstring shell(const nstring& cmd)
            {
#ifdef _WIN32
                //LOG_INFO(0, cmd);
                char buf[102400] = { 0 };
                FILE* pf = NULL;

                if ((pf = _popen(cmd.c_str(), "r")) == NULL)
                {
                    return "";
                }

                nstring strResult;
                while (fgets(buf, sizeof buf, pf))
                {
                    strResult += buf;
                }

                _pclose(pf);
                return strResult;
#else
                return "";
#endif
            }
            bool shell(const nstring& cmd, CALLBACK_SHELL callback,void* param)
            {
#ifdef _WIN32
                HANDLE hWrite = 0;
                HANDLE hRead = 0;
                SECURITY_ATTRIBUTES sa;
                PROCESS_INFORMATION proc_info;
                STARTUPINFO start = { sizeof(start) };

                DWORD len = 0;
                sa.nLength = 12;
                sa.lpSecurityDescriptor = 0;
                sa.bInheritHandle = 1;
                if (CreatePipe(&hRead, &hWrite, &sa, 0) == false)
                    return false;
                start.cb = 68;
                start.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
                start.wShowWindow = false;
                start.hStdOutput = hWrite;
                start.hStdError = hWrite;

                nstring std_str = nstring(cmd);
                CHAR c_str[2048];
                memset(c_str, 0, 2048);
                memcpy(c_str, std_str.c_str(), std_str.length());

                if (CreateProcessA(0, c_str, 0, 0, TRUE, 0, 0, 0, &start, &proc_info) == false)
                    return false;
                CloseHandle(hWrite);

                while (len != -1)
                {
                    if (PeekNamedPipe(hRead, 0, 0, 0, &len, 0) == false)
                        goto _GO_BREAK;
                    if (len != 0)
                    {
                        char read_buf[1024];
                        memset(read_buf, 0, 1024);
                        ReadFile(hRead, read_buf, 1039, &len, 0);
                        callback(nstring(read_buf), false, param);
                    }
                    else
                    {
                        DWORD EXIT;
                        GetExitCodeProcess(proc_info.hProcess, &EXIT);
                        if (EXIT != STILL_ACTIVE)
                            goto _GO_BREAK;
                    }
                    system::sleep_msec(500);
                    continue;
                _GO_BREAK:
                    callback("", true, param);
                    CloseHandle(hRead);
                    return true;
                }
                return false;
#else
                return false;
#endif
            }
            nstring current_dir()
            {
#ifdef WIN32
                char PATH[MAX_PATH];
                memset(PATH, 0, MAX_PATH);
                GetCurrentDirectoryA(MAX_PATH, PATH);
                return PATH;
#else
                return "";
#endif
            }
#ifdef WIN32
            nstring current_filepath()
            {
                char exeFullPath[MAX_PATH]; // Full path 
                memset(exeFullPath, 0, MAX_PATH);
                GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
                return exeFullPath;
            }

            nstring  currentuser_path()
            {
                std::string path = "";
                const char* homeProfile = "USERPROFILE";
                char homePath[2048];
                memset(homePath, 0, 2048);
                unsigned int pathSize = GetEnvironmentVariable(homeProfile, homePath, 2048);
                if (pathSize < 2048 && pathSize != 0)
                    return homePath;
                return "";
            }
            
#endif
            int32 random(int32 min, int32 max, int32 rand_idx)
            {
                    int randNum = 0;
                    srand((unsigned int)time::now_msec()+ rand_idx);
                    //��������� 
                    randNum = rand() % (max - min + 1) + min;
                    return randNum;
            }
#ifdef _WIN32
            nstring desktop_path()
            {
                TCHAR sDesktopPath[MAX_PATH];
                memset(sDesktopPath, 0, MAX_PATH);
                SHGetSpecialFolderPathA(NULL, sDesktopPath, CSIDL_DESKTOPDIRECTORY, FALSE);
                return sDesktopPath;
            }
#endif
            nstring temp_path()
            {
#ifdef _WIN32
                char strTmpDir[MAX_PATH];
                memset(strTmpDir, 0, MAX_PATH);
                GetTempPathA(MAX_PATH, strTmpDir);
                return strTmpDir;
#else
                return "/temp/";
#endif
            }
#ifdef _WIN32
            void clipboard(const nstring& value)
            {
                if (OpenClipboard(NULL))//�򿪼�����
                {
                    HANDLE hClip;
                    char* pBuf = nullptr;
                    EmptyClipboard();//��ռ�����
                    hClip = GlobalAlloc(GMEM_MOVEABLE, strlen(value.c_str()) + 1);
                    pBuf = (char*)GlobalLock(hClip);
                    strcpy(pBuf, value.c_str());
                    GlobalUnlock(hClip);//����
                    SetClipboardData(CF_TEXT, pBuf);//д���µļ���������
                    CloseClipboard();
                }
            }

            void serial_number(nstring& sn1,nstring& sn2,nstring& sn3)
            {
                auto graphicscard = hardware::graphicscard();
                nstring all_str;
                nstring gh_str;
                nstring cpu_str =
                    hardware::cpu().serial_number +
                    nstring::from(hardware::cpu().number);
                all_str = cpu_str;
                for (uint32 i = 0; i < graphicscard.size(); i++)
                {
                    gh_str +=
                        codec::md5(codec::des::en(nstring::from((int64)graphicscard[i].AdapterLuid.HighPart),nstring::from(i))) +
                        codec::md5(codec::des::en(nstring::from(graphicscard[i].AdapterLuid.LowPart),nstring::from(i))) +
                        codec::md5(codec::des::en(nstring::from(graphicscard[i].Description),nstring::from(i))) +
                        codec::md5(codec::des::en(nstring::from(graphicscard[i].DeviceId),nstring::from(i))) +
                        codec::md5(codec::des::en(nstring::from(graphicscard[i].Revision),nstring::from(i))) +
                        codec::md5(codec::des::en(nstring::from(graphicscard[i].VendorId),nstring::from(i)));
                }
                all_str = cpu_str + gh_str;
                // ȫ��У�� ���仯�ʸ�
                sn1 = codec::md5(codec::des::en(all_str, all_str.left(5) + "   "));
                // CPUУ��
                sn2 = codec::md5(codec::des::en(cpu_str, cpu_str.left(5) + "   "));
                // �Կ�У��
                sn3 = codec::md5(codec::des::en(gh_str, gh_str.left(5) + "   "));
            }
            std::vector<nstring> disk_list()
            {
                std::vector<nstring> result;
#ifdef _WIN32
                for (char a = 'C'; a <= 'Z'; a++) //��ȡ����Ӳ���̷���
                {
                    char diskName[3] = { a, ':' ,0 };
                    //type����� 1:���ƶ����̣�2:���̣�3:����Ӳ�̣�4:������̣�5:CD-ROM��6:RAM����
                    auto type = GetDriveTypeA(diskName);
                    if (type == DRIVE_FIXED)
                        result.push_back(diskName);
                }
#else
                result.push_back("~");
#endif
                return result;
            }
            bool read_res(uint32 resname, const nstring& type, newobj::buffer& data)
            {
                data.clear();
                char strTmpDir[MAX_PATH];
                memset(strTmpDir, 0, MAX_PATH);
                GetTempPathA(MAX_PATH, strTmpDir);
                //����Ŀ����Դ
                HRSRC hResource = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resname), type.c_str());
                if (hResource)
                {
                    //������Դ
                    HGLOBAL hg = LoadResource(GetModuleHandle(NULL), hResource);
                    if (hg)
                    {
                        //������Դ
                        LPVOID pData = LockResource(hg);
                        if (pData)
                        {
                            //��ȡ��Դ��С
                            DWORD dwSize = SizeofResource(GetModuleHandle(NULL), hResource);
                            data.append((const char*)pData, dwSize);
                            return true;
                        }
                    }
                }
                return false;
            }
#endif
#ifdef MSVC_2010
            bool link(const nstring& srcpath, const nstring&dstpath)
            {
                HRESULT hr = CoInitialize(NULL);
                if (SUCCEEDED(hr))
                {
                    IShellLink* pShellLink;
                    hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
                    if (SUCCEEDED(hr))
                    {
                        pShellLink->SetPath(dstpath.c_str());
                        std::string strTmp = dstpath.c_str();
                        int nStart = strTmp.find_last_of(TEXT("/\\"));
                        pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
                        pShellLink->SetArguments("");
                        //if (szIconPath)
                        //{
                        //    pShellLink->SetIconLocation(szIconPath, 0);
                        //}

                        IPersistFile* pPersistFile;
                        hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
                        if (SUCCEEDED(hr))
                        {
                            hr = pPersistFile->Save(forstring::to_wstring(dstpath).c_str(), FALSE);
                            if (SUCCEEDED(hr))
                            {
                                return true;
                            }
                            pPersistFile->Release();
                        }
                        pShellLink->Release();
                    }
                    CoUninitialize();
                }
                return false;
            }
#endif
    }
}
#ifdef _WIN32
bool newobj::system::tool::mstsc(const nstring& savepath,const nstring& ipaddress, const nstring& username, const nstring& password, uint32 port, bool mod, uint32 color_15_16_24_32, uint32 width, uint32 height, bool disk, bool compression, bool displayconnectionbar, bool wallpaper, bool themes, bool autoreconnection)
{
    
    nstring cmd;
    cmd += "screen mode id:i:" + nstring::from(mod)+"\r\n";
    cmd += "desktopwidth:i:" + nstring::from(width) + "\r\n";
    cmd += "desktopheight:i:" + nstring::from(height) + "\r\n";
    cmd += "session bpp:i:" + nstring::from(color_15_16_24_32) + "\r\n";
    cmd += "compression:i:" + nstring::from(compression) + "\r\n";
    cmd += "displayconnectionbar:i:" + nstring::from(displayconnectionbar) + "\r\n";
    cmd += "disable wallpaper:i:" + nstring::from(wallpaper) + "\r\n";
    cmd += "disable themes:i:" + nstring::from(themes) + "\r\n";
    cmd += "full address:s:" + ipaddress +":"+nstring::from(port) + "\r\n";
    if(disk)
        cmd += "drivestoredirect:s:*\r\ndevicestoredirect : s:*\r\nredirectsmartcards : i:1\r\n";
    cmd += "autoreconnection enabled:i:" + nstring::from(autoreconnection) + "\r\n";
    cmd += "winposstr:s:2,3,0,0,800,600\r\n";
    cmd += "redirectclipboard:i:1\r\n";
    cmd += "username:s:"+username+"\r\n";

    auto GetCryptPassword = [&]()->nstring {
        auto unPwd = bytes::to_unicode(password);
        unPwd.append(0x00);
        unPwd.append(0x00);

        DATA_BLOB in;
        DATA_BLOB out;
        out.cbData = 0;
        out.pbData = nullptr;
        in.pbData = (BYTE*)unPwd.data();
        in.cbData = 512;

        print::print_hex(unPwd.data(),(uint32)unPwd.length());

        const WCHAR code[8] = {112,0,115, 0, 119, 0, 0, 0 };
        CryptProtectData(&in,code,0,0,0,0,&out);
        if (out.cbData == 0)
            return "";
        nstring result;
        for (uint32 i = 0; i < out.cbData; i++)
        {
            auto temp = newobj::bytes::to_hexstring((int32)out.pbData[i]).right(2);
            result += temp;
        }
        return result + "0";
    };
    cmd += "password 51:b:"+GetCryptPassword()+"\r\n";

    f_ret_f(file::write(savepath, cmd));
    return true;
}
#endif

Size newobj::system::screen::device_size()
{
    Size result;
#ifdef WIN32
    result.width = GetSystemMetrics(SM_CXSCREEN);
    result.height = GetSystemMetrics(SM_CYSCREEN);
#else
#ifdef LIB_QT
    QScreen* screen = QApplication::primaryScreen();
    result.width = screen->size().width();
    result.height = screen->size().height();
#endif

#endif
    return result;
}

Size newobj::system::screen::work_size()
{
    Size result;
#ifdef WIN32
    result.width = GetSystemMetrics(SM_CXFULLSCREEN);
    result.height = GetSystemMetrics(SM_CXFULLSCREEN);
#endif
    return result;
}
#ifdef _WIN32
bool newobj::system::screen::set(const Size& size)
{
    DEVMODE lpDevMode;
    lpDevMode.dmBitsPerPel = 32;
    lpDevMode.dmPelsWidth = size.width;
    lpDevMode.dmPelsHeight = size.height;
    lpDevMode.dmSize = sizeof(lpDevMode);
    lpDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    LONG result;
    result = ChangeDisplaySettings(&lpDevMode, 0);
    if (result == DISP_CHANGE_SUCCESSFUL)
    {
        ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);
    }
    else
    {
        ChangeDisplaySettings(NULL, 0);
    }
    return result == DISP_CHANGE_SUCCESSFUL;
}
nstring newobj::system::os::version()
{
    auto GetWinMajorMinorVersion = [](DWORD& major, DWORD& minor)->bool
    {
        bool bRetCode = false;
        LPBYTE pinfoRawData = 0;
        if (NERR_Success == NetWkstaGetInfo(NULL, 100, &pinfoRawData))
        {
            WKSTA_INFO_100* pworkstationInfo = (WKSTA_INFO_100*)pinfoRawData;
            major = pworkstationInfo->wki100_ver_major;
            minor = pworkstationInfo->wki100_ver_minor;
            ::NetApiBufferFree(pinfoRawData);
            bRetCode = true;
        }
        return bRetCode;
    };

    nstring     winver;
    OSVERSIONINFOEX osver;
    SYSTEM_INFO     sysInfo;
    typedef void(__stdcall* GETSYSTEMINFO) (LPSYSTEM_INFO);

    __pragma(warning(push))
        __pragma(warning(disable:4996))
        memset(&osver, 0, sizeof(osver));
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx((LPOSVERSIONINFO)&osver);
    __pragma(warning(pop))
        DWORD major = 0;
    DWORD minor = 0;
    if (GetWinMajorMinorVersion(major, minor))
    {
        osver.dwMajorVersion = major;
        osver.dwMinorVersion = minor;
    }
    else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
    {
        OSVERSIONINFOEXW osvi;
        ULONGLONG cm = 0;
        cm = VerSetConditionMask(cm, VER_MINORVERSION, VER_EQUAL);
        ZeroMemory(&osvi, sizeof(osvi));
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        osvi.dwMinorVersion = 3;
        if (VerifyVersionInfoW(&osvi, VER_MINORVERSION, cm))
        {
            osver.dwMinorVersion = 3;
        }
    }

    GETSYSTEMINFO getSysInfo = (GETSYSTEMINFO)GetProcAddress(GetModuleHandle("kernel32.dll"), "GetNativeSystemInfo");
    if (getSysInfo == NULL)  getSysInfo = ::GetSystemInfo;
    getSysInfo(&sysInfo);

    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows 10 Server";
    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 10";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2012 R2";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 8.1";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2012";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 8";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2008 R2";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 7";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2008";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows Vista";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION
        && sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)  winver = "Windows XP x64";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)   winver = "Windows Server 2003";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)   winver = "Windows XP";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)   winver = "Windows 2000";
    if (osver.dwMajorVersion < 5)   winver = "unknown";

    if (osver.wServicePackMajor != 0)
    {
        std::string sp;
        char buf[128] = { 0 };
        sp = " Service Pack ";
        sprintf_s(buf, sizeof(buf), "%hd", osver.wServicePackMajor);
        sp.append(buf);
        winver += sp;
    }

    return winver;
}

#endif
std::vector<GraphicsCard>  newobj::system::hardware::graphicscard()
{
    std::vector<GraphicsCard> result;
#ifdef _WIN32
    // ��������  
    IDXGIFactory* pFactory = NULL;
    IDXGIAdapter* pAdapter;
    std::vector <IDXGIAdapter*> vAdapters;            // �Կ�  
    // �Կ�������  
    int iAdapterNum = 0;
    // ����һ��DXGI����  
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
    if (FAILED(hr))
        return result;
    // ö��������  
    while (pFactory->EnumAdapters(iAdapterNum, &pAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        vAdapters.push_back(pAdapter);
        ++iAdapterNum;
    }
    for (size_t i = 0; i < vAdapters.size(); i++)
    {
        GraphicsCard grap;
        // ��ȡ��Ϣ  
        DXGI_ADAPTER_DESC adapterDesc;
        vAdapters[i]->GetDesc(&adapterDesc);
        grap.Description = forstring::to_string(adapterDesc.Description);
        grap.AdapterLuid.HighPart = adapterDesc.AdapterLuid.HighPart;
        grap.AdapterLuid.LowPart = adapterDesc.AdapterLuid.LowPart;
        grap.DedicatedSystemMemory = adapterDesc.DedicatedSystemMemory;
        grap.DedicatedVideoMemory = adapterDesc.DedicatedVideoMemory;
        grap.DeviceId = adapterDesc.DeviceId;
        grap.Revision = adapterDesc.Revision;
        grap.SharedSystemMemory = adapterDesc.SharedSystemMemory;
        grap.SubSysId = adapterDesc.SubSysId;
        grap.VendorId = adapterDesc.VendorId;
        result.push_back(grap);
        //// ����豸  
        //IDXGIOutput* pOutput;
        //std::vector<IDXGIOutput*> vOutputs;
        //// ����豸����  
        //int iOutputNum = 0;
        //while (vAdapters[i]->EnumOutputs(iOutputNum, &pOutput) != DXGI_ERROR_NOT_FOUND)
        //{
        //    vOutputs.push_back(pOutput);
        //    iOutputNum++;
        //}

        //cout << "-----------------------------------------" << endl;
        //cout << "��ȡ��" << iOutputNum << "����ʾ�豸:" << endl;
        //cout << endl;

        //for (size_t n = 0; n < vOutputs.size(); n++)
        //{
        //    // ��ȡ��ʾ�豸��Ϣ  
        //    DXGI_OUTPUT_DESC outputDesc;
        //    vOutputs[n]->GetDesc(&outputDesc);

        //    // ��ȡ�豸֧��  
        //    UINT uModeNum = 0;
        //    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //    UINT flags = DXGI_ENUM_MODES_INTERLACED;

        //    vOutputs[n]->GetDisplayModeList(format, flags, &uModeNum, 0);
        //    DXGI_MODE_DESC* pModeDescs = new DXGI_MODE_DESC[uModeNum];
        //    vOutputs[n]->GetDisplayModeList(format, flags, &uModeNum, pModeDescs);

        //    cout << "��ʾ�豸����:" << outputDesc.DeviceName << endl;
        //    cout << "��ʾ�豸��ǰ�ֱ���:" << outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left << "*" << outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top << endl;
        //    cout << endl;

        //    // ��֧�ֵķֱ�����Ϣ  
        //    cout << "�ֱ�����Ϣ:" << endl;
        //    for (UINT m = 0; m < uModeNum; m++)
        //    {
        //        cout << "== �ֱ���:" << pModeDescs[m].Width << "*" << pModeDescs[m].Height << "     ˢ����" << (pModeDescs[m].RefreshRate.Numerator) / (pModeDescs[m].RefreshRate.Denominator) << endl;
        //    }
        //}
        //vOutputs.clear();

    }
    vAdapters.clear();

#endif
    return result;
}

Cpu  newobj::system::hardware::cpu()
{
    Cpu cpu_config;
    /*������*/
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    cpu_config.number = (int)info.dwNumberOfProcessors;
    //CPU����
    const long MAX_COMMAND_SIZE = 10000; // ��������������С
    char szFetCmd[] = "wmic cpu get processorid"; // ��ȡCPU���к�������
    const std::string strEnSearch = "ProcessorId"; // CPU���кŵ�ǰ����Ϣ
    BOOL   bret = FALSE;
    HANDLE hReadPipe = NULL; //��ȡ�ܵ�
    HANDLE hWritePipe = NULL; //д��ܵ�
    PROCESS_INFORMATION pi;   //������Ϣ
    STARTUPINFO         si;   //���������д�����Ϣ
    SECURITY_ATTRIBUTES sa;   //��ȫ����

    char            szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
    std::string          strBuffer;
    unsigned long   count = 0;
    long            ipos = 0;

    memset(&pi, 0, sizeof(pi));
    memset(&si, 0, sizeof(si));
    memset(&sa, 0, sizeof(sa));
    int index = 0;
    //modify here
    //ȥ���м�Ŀո� \r \n
    char temp[512];
    memset(temp, 0, sizeof(temp));

    pi.hProcess = NULL;
    pi.hThread = NULL;
    si.cb = sizeof(STARTUPINFO);
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    //1.0 �����ܵ�
    bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    if (!bret)
    {
        goto END;
    }

    //2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
    GetStartupInfo(&si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.wShowWindow = SW_HIDE; //���������д���
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    //3.0 ������ȡ�����еĽ���
    bret = CreateProcessA(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if (!bret)
    {
        goto END;
    }



    //4.0 ��ȡ���ص�����
    WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
    bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
    if (!bret)
    {
        goto END;
    }

    //5.0 ����CPU���к�
    bret = FALSE;
    strBuffer = szBuffer;
    ipos = strBuffer.find(strEnSearch);

    if (ipos < 0) // û���ҵ�
    {
        goto END;
    }
    else
    {
        strBuffer = strBuffer.substr(ipos + strEnSearch.length());
    }

    memset(szBuffer, 0x00, sizeof(szBuffer));
    strcpy_s(szBuffer, strBuffer.c_str());




    for (size_t i = 0; i < strBuffer.size(); i++)
    {
        if (strBuffer[i] != ' ' && strBuffer[i] != '\n' && strBuffer[i] != '\r')
        {
            temp[index] = strBuffer[i];
            index++;
        }
    }
    cpu_config.serial_number = temp;

    bret = TRUE;

END:
    //�ر����еľ��
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#else
    cpu_config.number = 0;
#endif


    return cpu_config;

}
