/*
 * Copyright: JessMA Open Source (ldcsaa@gmail.com)
 *
 * Author	: Bruce Liang
 * Website	: https://github.com/ldcsaa
 * Project	: https://github.com/ldcsaa/HP-Socket
 * Blog		: http://www.cnblogs.com/ldcsaa
 * Wiki		: http://www.oschina.net/p/hp-socket
 * QQ Group	: 44636872, 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#pragma once

#include <ws2tcpip.h>
#include <mswsock.h>
#include <malloc.h>

#include <atlfile.h>

#include "windows/include/nossl/SocketInterface.h"
#include "Common/WaitFor.h"
#include "Common/SysHelper.h"
#include "Common/FuncHelper.h"
#include "Common/BufferPool.h"
#include "Common/RingBuffer.h"
#include "InternalDef.h"

#ifdef _ZLIB_SUPPORT
#include <zutil.h>
#endif

#ifdef _BROTLI_SUPPORT
#pragma warning(push)
#pragma warning(disable: 4005)
#include <brotli/decode.h>
#include <brotli/encode.h>
#pragma warning(pop)
#endif

/************************************************************************
���ƣ�Windows Socket �����ʼ����
�������Զ����غ�ж�� Windows Socket ���
************************************************************************/
class CInitSocket
{
public:
	CInitSocket(LPWSADATA lpWSAData = nullptr, BYTE minorVersion = 2, BYTE majorVersion = 2)
	{
		LPWSADATA lpTemp = lpWSAData;

		if(!lpTemp)
			lpTemp	= CreateLocalObject(WSADATA);

		m_iResult	= ::WSAStartup(MAKEWORD(majorVersion, minorVersion), lpTemp);
	}

	~CInitSocket()
	{
		if(IsValid())
			::WSACleanup();
	}

	int	 GetResult() const {return m_iResult;}
	BOOL IsValid()	 const {return m_iResult == 0;}

private:
	int m_iResult;
};

typedef struct hp_addr
{
	ADDRESS_FAMILY family;

	union
	{
		ULONG_PTR	addr;
		IN_ADDR		addr4;
		IN6_ADDR	addr6;
	};

	static const hp_addr ANY_ADDR4;
	static const hp_addr ANY_ADDR6;

	inline int AddrSize() const
	{
		return AddrSize(family);
	}

	inline static int AddrSize(ADDRESS_FAMILY f)
	{
		if(f == AF_INET)
			return sizeof(IN_ADDR);

		return sizeof(IN6_ADDR);
	}

	inline static const hp_addr& AnyAddr(ADDRESS_FAMILY f)
	{
		if(f == AF_INET)
			return ANY_ADDR4;

		return ANY_ADDR6;
	}

	inline const ULONG_PTR* Addr()	const	{return &addr;}
	inline ULONG_PTR* Addr()				{return &addr;}

	inline BOOL IsIPv4()			const	{return family == AF_INET;}
	inline BOOL IsIPv6()			const	{return family == AF_INET6;}
	inline BOOL IsSpecified()		const	{return IsIPv4() || IsIPv6();}
	inline void ZeroAddr()					{::ZeroMemory(&addr6, sizeof(addr6));}
	inline void Reset()						{::ZeroMemory(this, sizeof(*this));}

	inline hp_addr& Copy(hp_addr& other) const
	{
		if(this != &other)
			memcpy(&other, this, offsetof(hp_addr, addr) + AddrSize());

		return other;
	}

	hp_addr(ADDRESS_FAMILY f = AF_UNSPEC, BOOL bZeroAddr = FALSE)
	{
		family = f;

		if(bZeroAddr) ZeroAddr();
	}

} HP_ADDR, *HP_PADDR;

typedef struct hp_sockaddr
{
	union
	{
		ADDRESS_FAMILY	family;
		SOCKADDR		addr;
		SOCKADDR_IN		addr4;
		SOCKADDR_IN6	addr6;
	};

	inline int AddrSize() const
	{
		return AddrSize(family);
	}

	inline static int AddrSize(ADDRESS_FAMILY f)
	{
		if(f == AF_INET)
			return sizeof(SOCKADDR_IN);

		return sizeof(SOCKADDR_IN6);
	}

	inline int EffectAddrSize() const
	{
		return EffectAddrSize(family);
	}

	inline static int EffectAddrSize(ADDRESS_FAMILY f)
	{
		return (f == AF_INET) ? offsetof(SOCKADDR_IN, sin_zero) : sizeof(SOCKADDR_IN6);
	}

	inline static const hp_sockaddr& AnyAddr(ADDRESS_FAMILY f)
	{
		static const hp_sockaddr s_any_addr4(AF_INET, TRUE);
		static const hp_sockaddr s_any_addr6(AF_INET6, TRUE);

		if(f == AF_INET)
			return s_any_addr4;

		return s_any_addr6;
	}

	inline static int AddrMinStrLength(ADDRESS_FAMILY f)
	{
		if(f == AF_INET)
			return 16;

		return 46;
	}

	inline BOOL IsIPv4()			const	{return family == AF_INET;}
	inline BOOL IsIPv6()			const	{return family == AF_INET6;}
	inline BOOL IsSpecified()		const	{return IsIPv4() || IsIPv6();}
	inline USHORT Port()			const	{return ntohs(addr4.sin_port);}
	inline void SetPort(USHORT usPort)		{addr4.sin_port = htons(usPort);}
	inline void* SinAddr()			const	{return IsIPv4() ? (void*)&addr4.sin_addr : (void*)&addr6.sin6_addr;}
	inline void* SinAddr()					{return IsIPv4() ? (void*)&addr4.sin_addr : (void*)&addr6.sin6_addr;}

	inline const SOCKADDR* Addr()	const	{return &addr;}
	inline SOCKADDR* Addr()					{return &addr;}
	inline void ZeroAddr()					{::ZeroMemory(((char*)this) + sizeof(family), sizeof(*this) - sizeof(family));}
	inline void Reset()						{::ZeroMemory(this, sizeof(*this));}

	inline hp_sockaddr& Copy(hp_sockaddr& other) const
	{
		if(this != &other)
			memcpy(&other, this, AddrSize());

		return other;
	}

	size_t Hash() const
	{
		ASSERT(IsSpecified());

		size_t _Val		  = 2166136261U;
		const int size	  = EffectAddrSize();
		const BYTE* pAddr = (const BYTE*)Addr();

		for(int i = 0; i < size; i++)
			_Val = 16777619U * _Val ^ (size_t)pAddr[i];

		return (_Val);
	}

	bool EqualTo(const hp_sockaddr& other) const
	{
		ASSERT(IsSpecified() && other.IsSpecified());

		return EqualMemory(this, &other, EffectAddrSize());
	}

	hp_sockaddr(ADDRESS_FAMILY f = AF_UNSPEC, BOOL bZeroAddr = FALSE)
	{
		family = f;

		if(bZeroAddr) ZeroAddr();
	}

} HP_SOCKADDR, *HP_PSOCKADDR;

/* Server ����� Agent ����ڲ�ʹ�õ��¼������������ */

// �����ѹر�
#define HR_CLOSED	0xFF

/* �ر����ӱ�ʶ */
enum EnSocketCloseFlag
{
	SCF_NONE		= 0,	// �������¼�
	SCF_CLOSE		= 1,	// ���� �����ر� OnClose �¼�
	SCF_ERROR		= 2		// ���� �쳣�ر� OnClose �¼�
};

/* ���ݻ����������ṹ */
template<class T> struct TBufferObjBase
{
	WSAOVERLAPPED		ov;
	CPrivateHeap&		heap;

	EnSocketOperation	operation;
	WSABUF				buff;

	int					capacity;
	volatile LONG		sndCounter;

	T* next;
	T* last;

	static T* Construct(CPrivateHeap& heap, DWORD dwCapacity)
	{
		T* pBufferObj = (T*)heap.Alloc(sizeof(T) + dwCapacity);
		ASSERT(pBufferObj);

		pBufferObj->TBufferObjBase::TBufferObjBase(heap, dwCapacity);
		pBufferObj->buff.buf = ((char*)pBufferObj) + sizeof(T);

		return pBufferObj;
	}

	static void Destruct(T* pBufferObj)
	{
		ASSERT(pBufferObj);
		pBufferObj->heap.Free(pBufferObj);
	}

	void ResetSendCounter()
	{
		sndCounter = 2;
	}

	LONG ReleaseSendCounter()
	{
		return ::InterlockedDecrement(&sndCounter);
	}

	TBufferObjBase(CPrivateHeap& hp, DWORD dwCapacity)
	: heap(hp)
	, capacity((int)dwCapacity)
	{
		ASSERT(capacity > 0);
	}

	int Cat(const BYTE* pData, int length)
	{
		ASSERT(pData != nullptr && length >= 0);

		int cat = min(Remain(), length);

		if(cat > 0)
		{
			memcpy(buff.buf + buff.len, pData, cat);
			buff.len += cat;
		}

		return cat;
	}

	void ResetOV()	{::ZeroMemory(&ov, sizeof(ov));}
	void Reset()	{ResetOV(); buff.len = 0;}
	int Remain()	{return capacity - buff.len;}
	BOOL IsFull()	{return Remain() == 0;}
};

/* ���ݻ������ṹ */
struct TBufferObj : public TBufferObjBase<TBufferObj>
{
	SOCKET client;
};

/* UDP ���ݻ������ṹ */
struct TUdpBufferObj : public TBufferObjBase<TUdpBufferObj>
{
	HP_SOCKADDR	remoteAddr;
	int			addrLen;
};

/* ���ݻ���������ģ�� */
template<class T> struct TBufferObjListT : public TSimpleList<T>
{
public:
	int Cat(const BYTE* pData, int length)
	{
		ASSERT(pData != nullptr && length >= 0);

		int remain = length;

		while(remain > 0)
		{
			T* pItem = Back();

			if(pItem == nullptr || pItem->IsFull())
				pItem = PushBack(bfPool.PickFreeItem());

			int cat  = pItem->Cat(pData, remain);

			pData	+= cat;
			remain	-= cat;
		}

		return length;
	}

	T* PushTail(const BYTE* pData, int length)
	{
		ASSERT(pData != nullptr && length >= 0 && length <= (int)bfPool.GetItemCapacity());

		T* pItem = PushBack(bfPool.PickFreeItem());
		pItem->Cat(pData, length);

		return pItem;
	}

	void Release()
	{
		bfPool.PutFreeItem(*this);
	}

public:
	TBufferObjListT(CNodePoolT<T>& pool) : bfPool(pool)
	{
	}

private:
	CNodePoolT<T>& bfPool;
};

/* ���ݻ���������� */
typedef CNodePoolT<TBufferObj>			CBufferObjPool;
/* UDP ���ݻ���������� */
typedef CNodePoolT<TUdpBufferObj>		CUdpBufferObjPool;
/* ���ݻ���������ģ�� */
typedef TBufferObjListT<TBufferObj>		TBufferObjList;
/* UDP ���ݻ���������ģ�� */
typedef TBufferObjListT<TUdpBufferObj>	TUdpBufferObjList;

/* TBufferObj ����ָ�� */
typedef TItemPtrT<TBufferObj>			TBufferObjPtr;
/* TUdpBufferObj ����ָ�� */
typedef TItemPtrT<TUdpBufferObj>		TUdpBufferObjPtr;

/* Socket �����������ṹ */
struct TSocketObjBase
{
	CPrivateHeap& heap;

	CONNID		connID;
	HP_SOCKADDR	remoteAddr;
	PVOID		extra;
	PVOID		reserved;
	PVOID		reserved2;
	BOOL		valid;

	union
	{
		DWORD	freeTime;
		DWORD	connTime;
	};

	DWORD		activeTime;

	volatile BOOL	smooth;
	volatile long	pending;
	volatile long	sndCount;

	volatile BOOL	connected;
	volatile BOOL	paused;
	volatile BOOL	recving;

	TSocketObjBase(CPrivateHeap& hp) : heap(hp) {}

	static BOOL IsExist(TSocketObjBase* pSocketObj)
		{return pSocketObj != nullptr;}

	static BOOL IsValid(TSocketObjBase* pSocketObj)
		{return pSocketObj != nullptr && pSocketObj->valid;}

	static void Invalid(TSocketObjBase* pSocketObj)
		{ASSERT(IsExist(pSocketObj)); pSocketObj->valid = FALSE;}

	static void Release(TSocketObjBase* pSocketObj)
		{ASSERT(IsExist(pSocketObj)); pSocketObj->freeTime = ::TimeGetTime();}

	DWORD GetConnTime	()	const	{return connTime;}
	DWORD GetFreeTime	()	const	{return freeTime;}
	DWORD GetActiveTime	()	const	{return activeTime;}
	BOOL IsPaused		()	const	{return paused;}

	long Pending()		{return pending;}
	BOOL IsPending()	{return pending > 0;}
	BOOL IsSmooth()		{return smooth;}
	void TurnOnSmooth()	{smooth = TRUE;}

	BOOL TurnOffSmooth()
		{return ::InterlockedCompareExchange((volatile long*)&smooth, FALSE, TRUE) == TRUE;}
	
	BOOL HasConnected()							{return connected;}
	void SetConnected(BOOL bConnected = TRUE)	{connected = bConnected;}

	void Reset(CONNID dwConnID)
	{
		connID		= dwConnID;
		connected	= FALSE;
		valid		= TRUE;
		smooth		= TRUE;
		paused		= FALSE;
		recving		= FALSE;
		pending		= 0;
		sndCount	= 0;
		extra		= nullptr;
		reserved	= nullptr;
		reserved2	= nullptr;
	}
};

/* ���ݻ������ṹ */
struct TSocketObj : public TSocketObjBase
{
	CCriSec			csRecv;
	CCriSec			csSend;
	CSpinGuard		sgPause;

	SOCKET			socket;
	CStringA		host;
	TBufferObjList	sndBuff;

	BOOL IsCanSend() {return sndCount <= GetSendBufferSize();}

	long GetSendBufferSize()
	{
		long lSize;
		int len	= (int)(sizeof(lSize));
		int rs	= getsockopt(socket, SOL_SOCKET, SO_SNDBUF, (CHAR*)&lSize, &len);

		if(rs == SOCKET_ERROR || lSize <= 0)
			lSize = DEFAULT_SOCKET_SNDBUFF_SIZE;

		return lSize;
	}

	static TSocketObj* Construct(CPrivateHeap& hp, CBufferObjPool& bfPool)
	{
		TSocketObj* pSocketObj = (TSocketObj*)hp.Alloc(sizeof(TSocketObj));
		ASSERT(pSocketObj);

		pSocketObj->TSocketObj::TSocketObj(hp, bfPool);

		return pSocketObj;
	}

	static void Destruct(TSocketObj* pSocketObj)
	{
		ASSERT(pSocketObj);

		CPrivateHeap& heap = pSocketObj->heap;
		pSocketObj->TSocketObj::~TSocketObj();
		heap.Free(pSocketObj);
	}
	
	TSocketObj(CPrivateHeap& hp, CBufferObjPool& bfPool)
	: TSocketObjBase(hp), sndBuff(bfPool)
	{

	}

	static BOOL InvalidSocketObj(TSocketObj* pSocketObj)
	{
		BOOL bDone = FALSE;

		if(TSocketObj::IsValid(pSocketObj))
		{
			pSocketObj->SetConnected(FALSE);

			CCriSecLock locallock(pSocketObj->csRecv);
			CCriSecLock locallock2(pSocketObj->csSend);

			if(TSocketObjBase::IsValid(pSocketObj))
			{
				TSocketObjBase::Invalid(pSocketObj);
				bDone = TRUE;
			}
		}

		return bDone;
	}

	static void Release(TSocketObj* pSocketObj)
	{
		__super::Release(pSocketObj);

		pSocketObj->sndBuff.Release();
	}

	void Reset(CONNID dwConnID, SOCKET soClient)
	{
		__super::Reset(dwConnID);
		
		host.Empty();

		socket = soClient;
	}

	BOOL GetRemoteHost(LPCSTR* lpszHost, USHORT* pusPort = nullptr)
	{
		*lpszHost = host;

		if(pusPort)
			*pusPort = remoteAddr.Port();

		return (*lpszHost != nullptr && (*lpszHost)[0] != 0);
	}
};

/* UDP ���ݻ������ṹ */
struct TUdpSocketObj : public TSocketObjBase
{
	PVOID				pHolder;
	HANDLE				hTimer;

	CRWLock				csRecv;
	CCriSec				csSend;

	TUdpBufferObjList	sndBuff;
	volatile DWORD		detectFails;

	BOOL IsCanSend			() {return sndCount <= GetSendBufferSize();}
	long GetSendBufferSize	() {return (4 * DEFAULT_SOCKET_SNDBUFF_SIZE);}

	static TUdpSocketObj* Construct(CPrivateHeap& hp, CUdpBufferObjPool& bfPool)
	{
		TUdpSocketObj* pSocketObj = (TUdpSocketObj*)hp.Alloc(sizeof(TUdpSocketObj));
		ASSERT(pSocketObj);

		pSocketObj->TUdpSocketObj::TUdpSocketObj(hp, bfPool);

		return pSocketObj;
	}

	static void Destruct(TUdpSocketObj* pSocketObj)
	{
		ASSERT(pSocketObj);

		CPrivateHeap& heap = pSocketObj->heap;
		pSocketObj->TUdpSocketObj::~TUdpSocketObj();
		heap.Free(pSocketObj);
	}
	
	TUdpSocketObj(CPrivateHeap& hp, CUdpBufferObjPool& bfPool)
	: TSocketObjBase(hp), sndBuff(bfPool)
	{

	}

	static BOOL InvalidSocketObj(TUdpSocketObj* pSocketObj)
	{
		BOOL bDone = FALSE;

		if(TUdpSocketObj::IsValid(pSocketObj))
		{
			pSocketObj->SetConnected(FALSE);

			CReentrantWriteLock	locallock(pSocketObj->csRecv);
			CCriSecLock			locallock2(pSocketObj->csSend);

			if(TSocketObjBase::IsValid(pSocketObj))
			{
				TSocketObjBase::Invalid(pSocketObj);
				bDone = TRUE;
			}
		}

		return bDone;
	}

	static void Release(TUdpSocketObj* pSocketObj)
	{
		__super::Release(pSocketObj);

		pSocketObj->sndBuff.Release();
	}

	void Reset(CONNID dwConnID)
	{
		__super::Reset(dwConnID);

		pHolder		= nullptr;
		hTimer		= nullptr;
		detectFails	= 0;
	}
};

/* ��Ч TSocketObj ���� */
typedef CRingCache2<TSocketObj, CONNID, true>		TSocketObjPtrPool;
/* ʧЧ TSocketObj ���� */
typedef CRingPool<TSocketObj>						TSocketObjPtrList;
/* ʧЧ TSocketObj �������սṹ���� */
typedef CCASQueue<TSocketObj>						TSocketObjPtrQueue;

/* ��Ч TUdpSocketObj ���� */
typedef CRingCache2<TUdpSocketObj, CONNID, true>	TUdpSocketObjPtrPool;
/* ʧЧ TUdpSocketObj ���� */
typedef CRingPool<TUdpSocketObj>					TUdpSocketObjPtrList;
/* ʧЧ TUdpSocketObj �������սṹ���� */
typedef CCASQueue<TUdpSocketObj>					TUdpSocketObjPtrQueue;

/* HP_SOCKADDR �Ƚ��� */
struct hp_sockaddr_func
{
	struct hash
	{
		size_t operator() (const HP_SOCKADDR* pA) const
		{
			return pA->Hash();
		}
	};

	struct equal_to
	{
		bool operator () (const HP_SOCKADDR* pA, const HP_SOCKADDR* pB) const
		{
			return pA->EqualTo(*pB);
		}
	};

};

/* ��ַ-���� ID ��ϣ�� */
typedef unordered_map<const HP_SOCKADDR*, CONNID, hp_sockaddr_func::hash, hp_sockaddr_func::equal_to>
										TSockAddrMap;
/* ��ַ-���� ID ��ϣ�������� */
typedef TSockAddrMap::iterator			TSockAddrMapI;
/* ��ַ-���� ID ��ϣ�� const ������ */
typedef TSockAddrMap::const_iterator	TSockAddrMapCI;

/* IClient ����ر������� */
struct TClientCloseContext
{
	BOOL bFireOnClose;
	EnSocketOperation enOperation;
	int iErrorCode;
	BOOL bNotify;

	TClientCloseContext(BOOL bFire = TRUE, EnSocketOperation enOp = SO_CLOSE, int iCode = SE_OK, BOOL bNtf = TRUE)
	{
		Reset(bFire, enOp, iCode, bNtf);
	}

	void Reset(BOOL bFire = TRUE, EnSocketOperation enOp = SO_CLOSE, int iCode = SE_OK, BOOL bNtf = TRUE)
	{
		bFireOnClose = bFire;
		enOperation	 = enOp;
		iErrorCode	 = iCode;
		bNotify		 = bNtf;
	}

};

/*****************************************************************************************************/
/******************************************** ������������ ********************************************/
/*****************************************************************************************************/

/* ��ȡ���������ı� */
LPCTSTR GetSocketErrorDesc(EnSocketError enCode);
/* ȷ����ַ�� */
ADDRESS_FAMILY DetermineAddrFamily(LPCTSTR lpszAddress);
/* ��ַ�ַ�����ַת��Ϊ HP_ADDR */
BOOL GetInAddr(LPCTSTR lpszAddress, __out HP_ADDR& addr);
/* ��ַ�ַ�����ַת��Ϊ HP_SOCKADDR */
BOOL GetSockAddr(LPCTSTR lpszAddress, USHORT usPort, __inout HP_SOCKADDR& addr);
/* ����ַ����Ƿ���� IP ��ַ��ʽ */
BOOL IsIPAddress(LPCTSTR lpszAddress, __out EnIPAddrType* penType = nullptr);
/* ͨ����������ȡ IP ��ַ */
BOOL GetIPAddress(LPCTSTR lpszHost, __out LPTSTR lpszIP, __inout int& iIPLenth, __out EnIPAddrType& enType);
/* ͨ����������ȡ HP_SOCKADDR */
BOOL GetSockAddrByHostName(LPCTSTR lpszHost, USHORT usPort, __out HP_SOCKADDR& addr);
/* ͨ����������ȡ HP_SOCKADDR */
BOOL GetSockAddrByHostNameDirectly(LPCTSTR lpszHost, USHORT usPort, HP_SOCKADDR &addr);
/* ö������ IP ��ַ */
BOOL EnumHostIPAddresses(LPCTSTR lpszHost, EnIPAddrType enType, __out LPTIPAddr** lpppIPAddr, __out int& iIPAddrCount);
/* ��� LPTIPAddr* */
BOOL RetrieveSockAddrIPAddresses(const vector<HP_PSOCKADDR>& vt, __out LPTIPAddr** lpppIPAddr, __out int& iIPAddrCount);
/* �ͷ� LPTIPAddr* */
BOOL FreeHostIPAddresses(LPTIPAddr* lppIPAddr);
/* �� HP_SOCKADDR �ṹת��Ϊ��ַ�ַ��� */
BOOL sockaddr_IN_2_A(const HP_SOCKADDR& addr, __out ADDRESS_FAMILY& usFamily, __out LPTSTR lpszAddress, __inout int& iAddressLen, __out USHORT& usPort);
/* �ѵ�ַ�ַ���ת��Ϊ HP_SOCKADDR �ṹ */
BOOL sockaddr_A_2_IN(LPCTSTR lpszAddress, USHORT usPort, __out HP_SOCKADDR& addr);
/* ��ȡ Socket �ı��ػ�Զ�̵�ַ��Ϣ */
BOOL GetSocketAddress(SOCKET socket, __out LPTSTR lpszAddress, __inout int& iAddressLen, __out USHORT& usPort, BOOL bLocal = TRUE);
/* ��ȡ Socket �ı��ص�ַ��Ϣ */
BOOL GetSocketLocalAddress(SOCKET socket, __out LPTSTR lpszAddress, __inout int& iAddressLen, __out USHORT& usPort);
/* ��ȡ Socket ��Զ�̵�ַ��Ϣ */
BOOL GetSocketRemoteAddress(SOCKET socket, __out LPTSTR lpszAddress, __inout int& iAddressLen, __out USHORT& usPort);

/* 64 λ�����ֽ���ת�����ֽ��� */
ULONGLONG NToH64(ULONGLONG value);
/* 64 λ�����ֽ���ת�����ֽ��� */
ULONGLONG HToN64(ULONGLONG value);

/* �����͸ߵ��ֽڽ��� */
#define ENDIAN_SWAP_16(A)	((USHORT)((((USHORT)(A) & 0xff00) >> 8) | (((USHORT)(A) & 0x00ff) << 8)))
/* �����͸ߵ��ֽڽ��� */
#define ENDIAN_SWAP_32(A)	((((DWORD)(A) & 0xff000000) >> 24) | \
							(((DWORD)(A) & 0x00ff0000) >>  8)  | \
							(((DWORD)(A) & 0x0000ff00) <<  8)  | \
							(((DWORD)(A) & 0x000000ff) << 24)	 )

/* ����Ƿ�С���ֽ��� */
BOOL IsLittleEndian();
/* �����������ֽ���תС���ֽ��� */
USHORT HToLE16(USHORT value);
/* �����������ֽ���ת����ֽ��� */
USHORT HToBE16(USHORT value);
/* �����������ֽ���תС���ֽ��� */
DWORD HToLE32(DWORD value);
/* �����������ֽ���ת����ֽ��� */
DWORD HToBE32(DWORD value);

/* ��ȡ Socket ��ĳ����չ������ָ�� */
PVOID GetExtensionFuncPtr					(SOCKET sock, GUID guid);
/* ��ȡ AcceptEx ��չ����ָ�� */
LPFN_ACCEPTEX Get_AcceptEx_FuncPtr			(SOCKET sock);
/* ��ȡ GetAcceptExSockaddrs ��չ����ָ�� */
LPFN_GETACCEPTEXSOCKADDRS Get_GetAcceptExSockaddrs_FuncPtr(SOCKET sock);
/* ��ȡ ConnectEx ��չ����ָ�� */
LPFN_CONNECTEX Get_ConnectEx_FuncPtr		(SOCKET sock);
/* ��ȡ TransmitFile ��չ����ָ�� */
LPFN_TRANSMITFILE Get_TransmitFile_FuncPtr	(SOCKET sock);
/* ��ȡ DisconnectEx ��չ����ָ�� */
LPFN_DISCONNECTEX Get_DisconnectEx_FuncPtr	(SOCKET sock);

HRESULT ReadSmallFile(LPCTSTR lpszFileName, CAtlFile& file, CAtlFileMapping<>& fmap, DWORD dwMaxFileSize = MAX_SMALL_FILE_SIZE);
HRESULT MakeSmallFilePackage(LPCTSTR lpszFileName, CAtlFile& file, CAtlFileMapping<>& fmap, WSABUF szBuf[3], const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr);

/************************************************************************
���ƣ�IOCP ָ��Ͷ�ݰ�������
�������� IOCP ָ��Ͷ��
************************************************************************/

/* IOCP ���� */
enum EnIocpCommand
{
	IOCP_CMD_EXIT		= 0x00000000,	// �˳�����
	IOCP_CMD_ACCEPT		= 0xFFFFFFF1,	// ��������
	IOCP_CMD_DISCONNECT	= 0xFFFFFFF2,	// �Ͽ�����
	IOCP_CMD_SEND		= 0xFFFFFFF3,	// ��������
	IOCP_CMD_UNPAUSE	= 0xFFFFFFF4,	// ȡ����ͣ
	IOCP_CMD_TIMEOUT	= 0xFFFFFFF5	// ���ʱ
};

/* IOCP ��������� */
enum EnIocpAction
{
	IOCP_ACT_GOON		= 0,	// ����ִ��
	IOCP_ACT_CONTINUE	= 1,	// ����ִ��
	IOCP_ACT_BREAK		= 2		// �ж�ִ��
};

BOOL PostIocpCommand(HANDLE hIOCP, EnIocpCommand enCmd, ULONG_PTR ulParam);
BOOL PostIocpExit(HANDLE hIOCP);
BOOL PostIocpAccept(HANDLE hIOCP);
BOOL PostIocpDisconnect(HANDLE hIOCP, CONNID dwConnID);
BOOL PostIocpSend(HANDLE hIOCP, CONNID dwConnID);
BOOL PostIocpUnpause(HANDLE hIOCP, CONNID dwConnID);
BOOL PostIocpTimeout(HANDLE hIOCP, CONNID dwConnID);
BOOL PostIocpClose(HANDLE hIOCP, CONNID dwConnID, int iErrorCode);

/************************************************************************
���ƣ�setsockopt() ��������
�������򻯳��õ� setsockopt() ����
************************************************************************/

int SSO_SetSocketOption		(SOCKET sock, int level, int name, LPVOID val, int len);
int SSO_GetSocketOption		(SOCKET sock, int level, int name, LPVOID val, int* len);
int SSO_IoctlSocket			(SOCKET sock, long cmd, u_long* arg);
int SSO_WSAIoctl			(SOCKET sock, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned);

int SSO_UpdateAcceptContext	(SOCKET soClient, SOCKET soBind);
int SSO_UpdateConnectContext(SOCKET soClient, int iOption);
int SSO_NoBlock				(SOCKET sock, BOOL bNoBlock = TRUE);
int SSO_NoDelay				(SOCKET sock, BOOL bNoDelay = TRUE);
int SSO_DontLinger			(SOCKET sock, BOOL bDont = TRUE);
int SSO_Linger				(SOCKET sock, USHORT l_onoff, USHORT l_linger);
int SSO_KeepAlive			(SOCKET sock, BOOL bKeepAlive = TRUE);
int SSO_KeepAliveVals		(SOCKET sock, u_long onoff, u_long time, u_long interval);
int SSO_RecvBuffSize		(SOCKET sock, int size);
int SSO_SendBuffSize		(SOCKET sock, int size);
int SSO_RecvTimeOut			(SOCKET sock, int ms);
int SSO_SendTimeOut			(SOCKET sock, int ms);
int SSO_ReuseAddress		(SOCKET sock, EnReuseAddressPolicy opt);
int SSO_ExclusiveAddressUse	(SOCKET sock, BOOL bExclusive = TRUE);
int SSO_UDP_ConnReset		(SOCKET sock, BOOL bNewBehavior = TRUE);

/************************************************************************
���ƣ�Socket ��������
������Socket ������װ����
************************************************************************/

/* ��� IOCP ��������ֵ��NO_ERROR �򷵻� TRUE */
#define IOCP_NO_ERROR(rs)		((rs) == NO_ERROR)
/* ��� IOCP ��������ֵ��WSA_IO_PENDING �򷵻� TRUE */
#define IOCP_PENDING(rs)		((rs) == WSA_IO_PENDING)
/* ��� IOCP ��������ֵ��NO_ERROR �� WSA_IO_PENDING �򷵻� TRUE */
#define IOCP_SUCCESS(rs)		(IOCP_NO_ERROR(rs) || IOCP_PENDING(rs))

/* ����Ƿ� UDP RESET ���� */
#define IS_UDP_RESET_ERROR(rs)	((rs) == WSAENETRESET || (rs) == WSAECONNRESET)

/* ���� Connection ID */
CONNID GenerateConnectionID	();
/* ��� UDP ���ӹر�֪ͨ */
int IsUdpCloseNotify		(const BYTE* pData, int iLength);
/* ���� UDP ���ӹر�֪ͨ */
int SendUdpCloseNotify		(SOCKET sock);
/* ���� UDP ���ӹر�֪ͨ */
int SendUdpCloseNotify		(SOCKET sock, const HP_SOCKADDR& remoteAddr);
/* �ر� Socket */
int ManualCloseSocket		(SOCKET sock, int iShutdownFlag = 0xFF, BOOL bGraceful = TRUE);
/* Ͷ�� AccceptEx()������ WSA_IO_PENDING ת��Ϊ NO_ERROR */
int PostAccept				(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj, ADDRESS_FAMILY usFamily);
/* Ͷ�� AccceptEx() */
int PostAcceptNotCheck		(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj, ADDRESS_FAMILY usFamily);
/* Ͷ�� ConnectEx()������ WSA_IO_PENDING ת��Ϊ NO_ERROR */
int PostConnect				(LPFN_CONNECTEX pfnConnectEx, SOCKET soClient, const HP_SOCKADDR& sockAddr, TBufferObj* pBufferObj);
/* Ͷ�� ConnectEx() */
int PostConnectNotCheck		(LPFN_CONNECTEX pfnConnectEx, SOCKET soClient, const HP_SOCKADDR& sockAddr, TBufferObj* pBufferObj);
/* Ͷ�� WSASend()������ WSA_IO_PENDING ת��Ϊ NO_ERROR */
int PostSend				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* Ͷ�� WSASend() */
int PostSendNotCheck		(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* Ͷ�� WSARecv()������ WSA_IO_PENDING ת��Ϊ NO_ERROR */
int PostReceive				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* Ͷ�� WSARecv() */
int PostReceiveNotCheck		(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* Ͷ�� WSASendTo()������ WSA_IO_PENDING ת��Ϊ NO_ERROR */
int PostSendTo				(SOCKET sock, TUdpBufferObj* pBufferObj);
/* Ͷ�� WSASendTo() */
int PostSendToNotCheck		(SOCKET sock, TUdpBufferObj* pBufferObj);
/* Ͷ�� WSARecvFrom()������ WSA_IO_PENDING ת��Ϊ NO_ERROR */
int PostReceiveFrom			(SOCKET sock, TUdpBufferObj* pBufferObj);
/* Ͷ�� WSARecvFrom() */
int PostReceiveFromNotCheck	(SOCKET sock, TUdpBufferObj* pBufferObj);
/* ִ�з����� WSARecv()������ WSAEWOULDBLOCK ת��Ϊ NO_ERROR */
int NoBlockReceive(TBufferObj* pBufferObj);
/* ִ�з����� WSARecv() */
int NoBlockReceiveNotCheck(TBufferObj* pBufferObj);
/* ִ�з����� WSARecvFrom()������ WSAEWOULDBLOCK ת��Ϊ NO_ERROR */
int NoBlockReceiveFrom(SOCKET sock, TUdpBufferObj* pBufferObj);
/* ִ�з����� WSARecvFrom() */
int NoBlockReceiveFromNotCheck(SOCKET sock, TUdpBufferObj* pBufferObj);
/* �����鲥ѡ�� */
BOOL SetMultiCastSocketOptions(SOCKET sock, const HP_SOCKADDR& bindAddr, const HP_SOCKADDR& castAddr, int iMCTtl, BOOL bMCLoop);

// CP_XXX -> UNICODE
BOOL CodePageToUnicode(int iCodePage, const char szSrc[], WCHAR szDest[], int& iDestLength);
// UNICODE -> CP_XXX
BOOL UnicodeToCodePage(int iCodePage, const WCHAR szSrc[], char szDest[], int& iDestLength);
// GBK -> UNICODE
BOOL GbkToUnicode(const char szSrc[], WCHAR szDest[], int& iDestLength);
// UNICODE -> GBK
BOOL UnicodeToGbk(const WCHAR szSrc[], char szDest[], int& iDestLength);
// UTF8 -> UNICODE
BOOL Utf8ToUnicode(const char szSrc[], WCHAR szDest[], int& iDestLength);
// UNICODE -> UTF8
BOOL UnicodeToUtf8(const WCHAR szSrc[], char szDest[], int& iDestLength);
// GBK -> UTF8
BOOL GbkToUtf8(const char szSrc[], char szDest[], int& iDestLength);
// UTF8 -> GBK
BOOL Utf8ToGbk(const char szSrc[], char szDest[], int& iDestLength);

// ���� Base64 ����󳤶�
DWORD GuessBase64EncodeBound(DWORD dwSrcLen);
// ���� Base64 ����󳤶�
DWORD GuessBase64DecodeBound(const BYTE* lpszSrc, DWORD dwSrcLen);
// Base64 ���루����ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int Base64Encode(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// Base64 ���루����ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int Base64Decode(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);

// ���� URL ����󳤶�
DWORD GuessUrlEncodeBound(const BYTE* lpszSrc, DWORD dwSrcLen);
// ���� URL ����󳤶�
DWORD GuessUrlDecodeBound(const BYTE* lpszSrc, DWORD dwSrcLen);
// URL ���루����ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int UrlEncode(BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// URL ���루����ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int UrlDecode(BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);

#ifdef _ZLIB_SUPPORT

// ��ͨѹ��������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int Compress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// �߼�ѹ��������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int CompressEx(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen, int iLevel = Z_DEFAULT_COMPRESSION, int iMethod = Z_DEFLATED, int iWindowBits = DEF_WBITS, int iMemLevel = DEF_MEM_LEVEL, int iStrategy = Z_DEFAULT_STRATEGY);
// ��ͨ��ѹ������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int Uncompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// �߼���ѹ������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int UncompressEx(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen, int iWindowBits = DEF_WBITS);
// �Ʋ�ѹ���������
DWORD GuessCompressBound(DWORD dwSrcLen, BOOL bGZip = FALSE);

// Gzip ѹ��������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int GZipCompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// Gzip ��ѹ������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int GZipUncompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// �Ʋ� Gzip ��ѹ������ȣ�������� 0 �򲻺���ֵ��˵���������ݲ�����Ч�� Gzip ��ʽ��
DWORD GZipGuessUncompressBound(const BYTE* lpszSrc, DWORD dwSrcLen);

#endif

#ifdef _BROTLI_SUPPORT

// Brotli ѹ��������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int BrotliCompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// Brotli �߼�ѹ��������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int BrotliCompressEx(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen, int iQuality = BROTLI_DEFAULT_QUALITY, int iWindow = BROTLI_DEFAULT_WINDOW, BrotliEncoderMode enMode = BROTLI_DEFAULT_MODE);
// Brotli ��ѹ������ֵ��0 -> �ɹ���-3 -> �������ݲ���ȷ��-5 -> ������������㣩
int BrotliUncompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD& dwDestLen);
// Brotli �Ʋ�ѹ���������
DWORD BrotliGuessCompressBound(DWORD dwSrcLen);

#endif