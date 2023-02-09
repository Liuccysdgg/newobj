#pragma once
#include "public/string.h"
#include "public/struct.h"
#include "public/buffer.h"
#include <vector>
namespace newobj
{
	namespace system
	{
		/*网卡信息*/
		struct NetCard
		{
			//网卡名
			nstring name; 
			//IP地址
			nstring ipaddress;
			//子网掩码
			nstring netmask;
			//网关
			nstring gateway;
			//Mac
			nstring mac;
		};

		/*睡眠*/
		void  NEWOBJ_API sleep_msec(unsigned int time);


		/*执行shell/cmd*/
		int32 NEWOBJ_API shell(const nstring& cmd, nstring& result, const nstring& temp_filepath);
		/*执行shell/cmd*/
		nstring NEWOBJ_API shell(const nstring& cmd);

		typedef void(*CALLBACK_SHELL)(nstring callback, bool close, void* param);
		/*执行shell/cmd*/
		bool NEWOBJ_API shell(const nstring& cmd, CALLBACK_SHELL callback, void* param);
		/*取运行目录*/
		nstring NEWOBJ_API current_dir();
		/*取运行路径*/
		nstring NEWOBJ_API current_filepath();
		// 取用户目录
		nstring NEWOBJ_API currentuser_path();
		/*获取目录下文件*/
		//void getFiles(const nstring& rootPath, std::vector<nstring>& ret, std::vector<nstring>& name)
		/*取范围随机数*/
		int32 NEWOBJ_API random(int32 min, int32 max, int32 rand_idx = 0);
		/*生成快捷方式*/
		bool  NEWOBJ_API link(const nstring& srcpath, const nstring& dstpath);
#ifdef _WIN32
		/*取桌面路径*/
		nstring NEWOBJ_API desktop_path();
#endif
#ifdef _WIN32
		/*置剪辑版文本*/
		void NEWOBJ_API clipboard(const nstring& value);
#endif
		/******************************************[ 屏幕相关 ]************************************************/
		namespace screen
		{
			/*取真实屏幕设备大小*/
			Size NEWOBJ_API device_size();
			/*
				取工作区大小
				Desc：不包含任务栏
			*/
			Size NEWOBJ_API work_size();
			/*设置分辨率*/
			bool NEWOBJ_API set(const Size &size);

		}
		/******************************************[ 网卡相关 ]************************************************/
		namespace netcard
		{
			/*获取网卡地址*/
			nstring  NEWOBJ_API mac(nstring name = "");
			/*获取网卡列表*/
			std::vector<NetCard>  NEWOBJ_API list();
			/*设置网卡*/
			bool  NEWOBJ_API set(const NetCard& netCard);
		}
		/******************************************[ CPU相关 ]************************************************/
		namespace cpu
		{
			
		}
		/******************************************[ 硬件相关 ]************************************************/
		namespace hardware
		{
			// 显卡信息
			std::vector<GraphicsCard> NEWOBJ_API graphicscard();
			// CPU信息
			Cpu  NEWOBJ_API cpu();
		}
		/******************************************[ 系统工具 ]************************************************/
		namespace tool
		{
			/*
				创建远程桌面文件
				@savepath									：			保存地址
				@ipaddress									：			IP地址
				@username									：			账号
				@password									：			密码
				@port											：			端口
				@mod											：			窗口化
				@color_15_16_24_32					：			色彩
				@width										：			宽度
				@height										：			高度
				@disk											：			使用硬盘
				@compression								：			压缩数据
				@displayconnectionbar				：			全屏显示连接栏
				@wallpaper									：			显示桌面背景
				@themes										：			允许主题
				@autoreconnection						：			自动连接
			*/
			bool NEWOBJ_API mstsc(
				const nstring& savepath,
				const nstring& ipaddress,
				const nstring& username,
				const nstring& password,
				uint32 port,
				bool mod,
				uint32 color_15_16_24_32,
				uint32 width,
				uint32 height,
				bool disk = true,
				bool compression = true,
				bool displayconnectionbar = true,
				bool wallpaper = true,
				bool themes = true,
				bool autoreconnection = true
			);
		}



		namespace os
		{
			/*取系统版本ID*/
			nstring NEWOBJ_API version();
		}
		/*取临时目录*/
		nstring NEWOBJ_API temp_path();

		/*生成本机唯一序列号*/
		void NEWOBJ_API serial_number(nstring& sn1, nstring& sn2, nstring& sn3);

		/*磁盘列表*/
		std::vector<nstring> NEWOBJ_API disk_list();

		/*读编译资源*/
		bool NEWOBJ_API read_res(uint32 resname,const nstring& type,newobj::buffer& data);
	}
}
