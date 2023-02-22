#pragma once
#include "basetype/string.h"
#include "public/define.h"
namespace newobj
{
	/*********************************************************[自定义结构体]*******************************************************/
	/*范围*/
	struct Range
	{
		Range() { min = 0; max = 0; }
		Range(int _in_min,int _in_max) { min = _in_min; max = _in_max; }
		uint32 min;
		uint32 max;
	};
	/*大小*/
	struct Size
	{
		Size(uint32 _in_width=0,uint32 _in_height = 0) { width = _in_width; height = _in_height; }
		uint32 width;
		uint32 height;
	};
	/*矩形*/
	struct Rect
	{
		Rect() { left = 0; top = 0; right = 0; bottom = 0; }
		uint32 left;
		uint32 top;
		uint32 right;
		uint32 bottom;
	};
	/*网络地址*/
	struct NetAddr
	{
		NetAddr() { port = 0; }
		nstring ip;
		ushort port;
	};
	/*几何*/
	struct Geometry
	{
		Geometry()
		{
			x = 0;
			y = 0;
			width = 0;
			height = 0;
		}
		int32 left()
		{
			return x;
		}
		int32 right()
		{
			return width;
		}
		int32 top()
		{
			return y;
		}
		int32 bottom()
		{
			return height;
		}
		int32 x;
		int32 y;
		uint32 width;
		uint32 height;
	};
	//加解密方式
	enum Endecrypt
	{
		ED_NONE,
		ED_AES,
		ED_DES3
	};
	// LUID
	struct Luid
	{
		Luid()
		{
			LowPart = 0;
			HighPart = 0;
		}
		size_t LowPart;
		long HighPart;
	};
	// 显卡信息
	struct GraphicsCard
    {
		GraphicsCard()
		{
			VendorId = 0;
			DeviceId = 0;
			SubSysId = 0;
			Revision = 0;
			DedicatedVideoMemory = 0;
			DedicatedSystemMemory = 0;
			SharedSystemMemory = 0;
		}
		// 描述
		nstring Description;
		// 供应商ID
		uint32 VendorId;
		// 设备ID
		uint32 DeviceId;
		// 子系统ID
		uint32 SubSysId;
		// 修订号
		uint32 Revision;
		size_t DedicatedVideoMemory;
		size_t DedicatedSystemMemory;
		size_t SharedSystemMemory;
		Luid AdapterLuid;
	};
	// CPU信息
	struct Cpu
	{
		Cpu()
		{
			number = 0;
		}
		// 序列号
		nstring serial_number;
		// CPU核心数量
		uint32 number;
	};
    /*键值对*/
    struct Kv {
        nstring name;
        nstring value;
    };

}
