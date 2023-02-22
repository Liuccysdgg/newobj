#pragma once
#include "basetype/string.h"
#include "public/define.h"
namespace newobj
{
	/*********************************************************[�Զ���ṹ��]*******************************************************/
	/*��Χ*/
	struct Range
	{
		Range() { min = 0; max = 0; }
		Range(int _in_min,int _in_max) { min = _in_min; max = _in_max; }
		uint32 min;
		uint32 max;
	};
	/*��С*/
	struct Size
	{
		Size(uint32 _in_width=0,uint32 _in_height = 0) { width = _in_width; height = _in_height; }
		uint32 width;
		uint32 height;
	};
	/*����*/
	struct Rect
	{
		Rect() { left = 0; top = 0; right = 0; bottom = 0; }
		uint32 left;
		uint32 top;
		uint32 right;
		uint32 bottom;
	};
	/*�����ַ*/
	struct NetAddr
	{
		NetAddr() { port = 0; }
		nstring ip;
		ushort port;
	};
	/*����*/
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
	//�ӽ��ܷ�ʽ
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
	// �Կ���Ϣ
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
		// ����
		nstring Description;
		// ��Ӧ��ID
		uint32 VendorId;
		// �豸ID
		uint32 DeviceId;
		// ��ϵͳID
		uint32 SubSysId;
		// �޶���
		uint32 Revision;
		size_t DedicatedVideoMemory;
		size_t DedicatedSystemMemory;
		size_t SharedSystemMemory;
		Luid AdapterLuid;
	};
	// CPU��Ϣ
	struct Cpu
	{
		Cpu()
		{
			number = 0;
		}
		// ���к�
		nstring serial_number;
		// CPU��������
		uint32 number;
	};
    /*��ֵ��*/
    struct Kv {
        nstring name;
        nstring value;
    };

}
