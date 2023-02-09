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

#include "windows/include/nossl/HPTypeDef.h"

#ifdef _SSL_SUPPORT

#include "Common/BufferPool.h"

#pragma warning(push)
#pragma warning(disable: 4005)


#pragma warning(pop)

#define OPENSSL_VERSION_1_0_2	0x10002000L
#define OPENSSL_VERSION_1_1_0	0x10100000L

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	#define DEFAULT_CIPHER_LIST	_T("DEFAULT:!aNULL:!eNULL:!SSLv2")
#else
	#define DEFAULT_CIPHER_LIST	_T("DEFAULT:!aNULL:!eNULL:!SSLv2:!SSLv3")
#endif

/************************************************************************
���ƣ�SSL ȫ�ֳ���
���������� SSL ����Ĺ���ȫ�ֳ���
************************************************************************/

#define SSL_DOMAIN_SEP_CHAR		'.'

/************************************************************************
���ƣ�SSL ����״̬
��������ʶ��ǰ���ӵ� SSL ����״̬
************************************************************************/
enum EnSSLHandShakeStatus
{
	SSL_HSS_INIT	= 0,	// ��ʼ״̬
	SSL_HSS_PROC	= 1,	// ��������
	SSL_HSS_SUCC	= 2,	// ���ֳɹ�
};

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0

/* SSL CRYPTO DYNLOCK �ṹ */
typedef struct CRYPTO_dynlock_value
{
	CSimpleRWLock cs;
} DynamicLock;

#endif

class CSSLInitializer
{
public:

	static void CleanupThreadState(DWORD dwThreadID = 0);

private:

	CSSLInitializer();
	~CSSLInitializer();

private:

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	static void ssl_lock_callback(int mode, int n, const char *file, int line);
	static CRYPTO_dynlock_value* ssl_lock_dyn_create_callback(const char *file, int line);
	static void ssl_lock_dyn_callback(int mode, CRYPTO_dynlock_value* l, const char *file, int line);
	static void ssl_lock_dyn_destroy_callback(CRYPTO_dynlock_value* l, const char *file, int line);
#endif

private:

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	static int				sm_iLockNum;
	static CSimpleRWLock*	sm_pcsLocks;
#endif

	static CSSLInitializer sm_instance;

};

/************************************************************************
���ƣ�SSL Context
��������ʼ�������� SSL ���л���
************************************************************************/
class CSSLContext
{
	typedef unordered_map<CString, int, cstring_nc_hash_func::hash, cstring_nc_hash_func::equal_to> CServerNameMap;

public:

	/*
	* ���ƣ���ʼ�� SSL ��������
	* ������SSL �������������� SSL ͨ���������ǰ��ɳ�ʼ������������ʧ��
	*		
	* ������		enSessionMode			-- SSL ����ģʽ���ο� EnSSLSessionMode��
	*			iVerifyMode				-- SSL ��֤ģʽ���ο� EnSSLVerifyMode��
	*			lpPemCert				-- ֤���ļ����ͻ��˿�ѡ��
	*			lpPemKey				-- ˽Կ�ļ����ͻ��˿�ѡ��
	*			lpKeyPasswod			-- ˽Կ���루û��������Ϊ�գ�
	*			lpCAPemCert				-- CA ֤���ļ���Ŀ¼��������֤��ͻ��˿�ѡ��
	*			fnServerNameCallback	-- SNI �ص�����ָ�루��ѡ��ֻ���ڷ���ˣ����Ϊ nullptr ��ʹ�� SNI Ĭ�ϻص�������
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� ::GetLastError() ��ȡʧ��ԭ��
	*/
	BOOL Initialize(EnSSLSessionMode enSessionMode, int iVerifyMode = SSL_VM_NONE, BOOL bMemory = FALSE, LPVOID lpPemCert = nullptr, LPVOID lpPemKey = nullptr, LPVOID lpKeyPasswod = nullptr, LPVOID lpCAPemCert = nullptr, Fn_SNI_ServerNameCallback fnServerNameCallback = nullptr);

	/*
	* ���ƣ����� SNI ����֤�飨ֻ���ڷ���ˣ�
	* ������SSL ������� Initialize() �ɹ�����Ե��ñ��������Ӷ�� SNI ����֤��
	*		
	* ������		iVerifyMode				-- SSL ��֤ģʽ���ο� EnSSLVerifyMode��
	*			lpPemCert				-- ֤���ļ�
	*			lpPemKey				-- ˽Կ�ļ�
	*			lpKeyPasswod			-- ˽Կ���루û��������Ϊ�գ�
	*			lpCAPemCert				-- CA ֤���ļ���Ŀ¼��������֤��ѡ��
	*
	* ����ֵ��	����		-- �ɹ��������� SNI ����֤���Ӧ�������������������� SNI �ص������ж�λ SNI ����
	*			����		-- ʧ�ܣ���ͨ�� ::GetLastError() ��ȡʧ��ԭ��
	*/
	int AddServerContext(int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod = nullptr, LPVOID lpCAPemCert = nullptr);

	/*
	* ���ƣ��� SNI ��������
	* ������SSL ������� AddServerContext() �ɹ�����Ե��ñ����������������� SNI ����֤��
	*		
	* ������		lpszServerName		-- ��������
	*			iContextIndex		-- SNI ����֤���Ӧ������
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� ::GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL BindServerName(LPCTSTR lpszServerName, int iContextIndex);

	/*
	* ���ƣ����� SSL ���л���
	* ���������� SSL ���л��������� SSL ����ڴ�
	*		1��CSSLContext �������������Զ����ñ�����
	*		2����Ҫ�������� SSL ��������ʱ����Ҫ�ȵ��ñ���������ԭ�ȵĻ�������
	*		
	* ������	��
	* 
	* ����ֵ����
	*/
	void Cleanup();

	/* ��ȡ SSL ���л��� SSL_CTX ���� */
	SSL_CTX* GetContext				(int i) const;
	/* ��ȡ SSL ���л���Ĭ�� SSL_CTX ���� */
	SSL_CTX* GetDefaultContext		()		const	{return m_sslCtx;}
	/* ��ȡ SSL ���л���������ģʽ������ģʽ�ο���EnSSLSessionMode */
	EnSSLSessionMode GetSessionMode	()		const	{return m_enSessionMode;}
	/* ��� SSL ���л����Ƿ��ʼ����� */
	BOOL IsValid					()		const	{return m_sslCtx != nullptr;}

	/* ���� SSL �����㷨�б� */
	void SetCipherList(LPCTSTR lpszCipherList)		{m_strCipherList = lpszCipherList;}
	/* ��ȡ SSL �����㷨�б� */
	LPCTSTR GetCipherList()							{return m_strCipherList;}

public:
	
	/*
	* ���ƣ������ֲ߳̾����� SSL ��Դ
	* �������κ�һ������ SSL ���̣߳���ͨ�Ž���ʱ����Ҫ�����ֲ߳̾����� SSL ��Դ
	*		1�����̺߳� HP-Socket �����߳���ͨ�Ž���ʱ���Զ������ֲ߳̾����� SSL ��Դ����ˣ�һ������²����ֹ����ñ�����
	*		2����������£����Զ����̲߳��� HP-Socket ͨ�Ų�������鵽 SSL �ڴ�й©ʱ������ÿ��ͨ�Ž���ʱ�Զ����̵߳��ñ�����
	*		
	* ������		dwThreadID	-- �߳� ID��0����ǰ�̣߳�
	* 
	* ����ֵ����
	*/
	static void RemoveThreadLocalState(DWORD dwThreadID = 0)	{CSSLInitializer::CleanupThreadState(dwThreadID);}

public:

	CSSLContext()
	: m_strCipherList		(DEFAULT_CIPHER_LIST)
	, m_enSessionMode		(SSL_SM_SERVER)
	, m_sslCtx				(nullptr)
	, m_fnServerNameCallback(nullptr)
	{

	}

	~CSSLContext() {}

private:

	void SetServerNameCallback(Fn_SNI_ServerNameCallback fn);
	int AddContext(int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod, LPVOID lpCAPemCert);
	BOOL LoadCertAndKey(SSL_CTX* sslCtx, int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod, LPVOID lpCAPemCert);
	BOOL LoadCertAndKeyByFile(SSL_CTX* sslCtx, int iVerifyMode, LPCTSTR lpszPemCertFile, LPCTSTR lpszPemKeyFile, LPCTSTR lpszKeyPassword, LPCTSTR lpszCAPemCertFileOrPath);
	BOOL LoadCertAndKeyByMemory(SSL_CTX* sslCtx, int iVerifyMode, LPCSTR lpszPemCert, LPCSTR lpszPemKey, LPCSTR lpszKeyPassword, LPCSTR lpszCAPemCert);
	BOOL LoadCAPemCertByMemory(SSL_CTX* sslCtx, int iVerifyMode, LPCSTR lpszCAPemCert);
	BOOL LoadPemCertAndKeyByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert, LPCSTR lpszPemKey, LPCSTR lpszKeyPassword);
	BOOL AddCAPemCertToStoreByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert);
	BOOL SetClientCAListByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert);
	BOOL SetPrivateKeyByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemKey);
	BOOL SetCertChainByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert);

private:

	static int InternalServerNameCallback(SSL* ssl, int* ad, void* arg);

public:

	/*
	* ���ƣ�SNI Ĭ�ϻص�����
	* ������Initialize �����������ָ�� SNI �ص�������ʹ�ô� SNI Ĭ�ϻص�����
	*		
	* ������		lpszServerName	-- ��������
	*			pContext		-- SSL Context ����
	* 
	* ����ֵ��SNI ����֤���Ӧ������
	*/
	static int __HP_CALL DefaultServerNameCallback(LPCTSTR lpszServerName, PVOID pContext);

private:

	CString				m_strCipherList;
	EnSSLSessionMode	m_enSessionMode;
	CServerNameMap		m_sslServerNames;
	vector<SSL_CTX*>	m_lsSslCtxs;
	SSL_CTX*			m_sslCtx;

	Fn_SNI_ServerNameCallback m_fnServerNameCallback;
};

class CSSLSession
{
public:

	BOOL WriteRecvChannel(const BYTE* pData, int iLength);
	BOOL ReadRecvChannel();

	BOOL WriteSendChannel(const BYTE* pData, int iLength);
	BOOL WriteSendChannel(const WSABUF pBuffers[], int iCount);
	BOOL ReadSendChannel();

	const WSABUF& GetRecvBuffer()	const	{return m_bufRecv;}
	const WSABUF& GetSendBuffer()	const	{return m_bufSend;}

	CSSLSession*			Renew(const CSSLContext& sslCtx, LPCSTR lpszHostName = nullptr);
	BOOL					Reset();
	BOOL					IsValid()		const	{return GetStatus() != SSL_HSS_INIT;}
	BOOL					IsHandShaking()	const	{return GetStatus() == SSL_HSS_PROC;}
	BOOL					IsReady()		const	{return GetStatus() == SSL_HSS_SUCC;}
	EnSSLHandShakeStatus	GetStatus()		const	{return m_enStatus;}
	DWORD					GetFreeTime()	const	{return m_dwFreeTime;}
	CCriSec&				GetSendLock()			{return m_csSend;}
	BOOL					GetSessionInfo(EnSSLSessionInfo enInfo, LPVOID* lppInfo);

private:

	BOOL IsFatalError(int iBytes);

public:

	CSSLSession(CItemPool& itPool)
	: m_enStatus(SSL_HSS_INIT)
	, m_itPool	(itPool)
	, m_ssl		(nullptr)
	, m_bioSend	(nullptr)
	, m_bioRecv	(nullptr)
	, m_pitSend	(nullptr)
	, m_pitRecv	(nullptr)
	{

	}

	~CSSLSession()
	{
		Reset();
	}

	static CSSLSession* Construct(CItemPool& itPool)
		{return new CSSLSession(itPool);}

	static void Destruct(CSSLSession* pSession)
		{if(pSession) delete pSession;}

private:
	CItemPool&				m_itPool;
	CCriSec					m_csSend;

	DWORD					m_dwFreeTime;
	EnSSLHandShakeStatus	m_enStatus;

	SSL* m_ssl;
	BIO* m_bioSend;
	BIO* m_bioRecv;

	TItem*		m_pitSend;
	TItem*		m_pitRecv;
	WSABUF		m_bufSend;
	WSABUF		m_bufRecv;
};

class CSSLSessionPool
{
	typedef CRingPool<CSSLSession>	TSSLSessionList;
	typedef CCASQueue<CSSLSession>	TSSLSessionQueue;

public:
	CSSLSession*	PickFreeSession	(LPCSTR lpszHostName = nullptr);
	void			PutFreeSession	(CSSLSession* pSession);

	void			Prepare			();
	void			Clear			();

private:
	void ReleaseGCSession	(BOOL bForce = FALSE);

public:
	void SetItemCapacity	(DWORD dwItemCapacity)		{m_itPool.SetItemCapacity(dwItemCapacity);}
	void SetItemPoolSize	(DWORD dwItemPoolSize)		{m_itPool.SetPoolSize(dwItemPoolSize);}
	void SetItemPoolHold	(DWORD dwItemPoolHold)		{m_itPool.SetPoolHold(dwItemPoolHold);}

	void SetSessionLockTime	(DWORD dwSessionLockTime)	{m_dwSessionLockTime = dwSessionLockTime;}
	void SetSessionPoolSize	(DWORD dwSessionPoolSize)	{m_dwSessionPoolSize = dwSessionPoolSize;}
	void SetSessionPoolHold	(DWORD dwSessionPoolHold)	{m_dwSessionPoolHold = dwSessionPoolHold;}

	DWORD GetItemCapacity	()	{return m_itPool.GetItemCapacity();}
	DWORD GetItemPoolSize	()	{return m_itPool.GetPoolSize();}
	DWORD GetItemPoolHold	()	{return m_itPool.GetPoolHold();}

	DWORD GetSessionLockTime()	{return m_dwSessionLockTime;}
	DWORD GetSessionPoolSize()	{return m_dwSessionPoolSize;}
	DWORD GetSessionPoolHold()	{return m_dwSessionPoolHold;}

public:
	CSSLSessionPool(const CSSLContext& sslCtx,
					DWORD dwPoolSize = DEFAULT_SESSION_POOL_SIZE,
					DWORD dwPoolHold = DEFAULT_SESSION_POOL_HOLD,
					DWORD dwLockTime = DEFAULT_SESSION_LOCK_TIME)
	: m_sslCtx(sslCtx)
	, m_dwSessionPoolSize(dwPoolSize)
	, m_dwSessionPoolHold(dwPoolHold)
	, m_dwSessionLockTime(dwLockTime)
	{

	}

	~CSSLSessionPool()	{Clear();}

	DECLARE_NO_COPY_CLASS(CSSLSessionPool)

public:
	static const DWORD DEFAULT_ITEM_CAPACITY;
	static const DWORD DEFAULT_ITEM_POOL_SIZE;
	static const DWORD DEFAULT_ITEM_POOL_HOLD;
	static const DWORD DEFAULT_SESSION_LOCK_TIME;
	static const DWORD DEFAULT_SESSION_POOL_SIZE;
	static const DWORD DEFAULT_SESSION_POOL_HOLD;

private:
	CItemPool			m_itPool;
	const CSSLContext&	m_sslCtx;

	DWORD				m_dwSessionLockTime;
	DWORD				m_dwSessionPoolSize;
	DWORD				m_dwSessionPoolHold;

	TSSLSessionList		m_lsFreeSession;
	TSSLSessionQueue	m_lsGCSession;
};

template<class T, class S> EnHandleResult ProcessHandShake(T* pThis, S* pSocketObj, CSSLSession* pSession)
{
	EnHandleResult result = HR_OK;

	CCriSecLock locallock(pSession->GetSendLock());

	while(TRUE)
	{
		ENSURE(pSession->ReadSendChannel());
		const WSABUF& buffer = pSession->GetSendBuffer();

		if(buffer.len == 0)
			break;

		if(!pThis->DoSendPackets(pSocketObj, &buffer, 1))
		{
			result = HR_ERROR;
			break;
		}
	}

	return result;
}

template<class T, class S> EnHandleResult ProcessReceive(T* pThis, S* pSocketObj, CSSLSession* pSession, const BYTE* pData, int iLength)
{
	if(!pSession->WriteRecvChannel(pData, iLength))
		return HR_ERROR;

	EnHandleResult result			= HR_OK;
	EnSSLHandShakeStatus enStatus	= pSession->GetStatus();

	while(TRUE)
	{
		if(!pSession->ReadRecvChannel())
			return HR_ERROR;

		if(enStatus == SSL_HSS_PROC && pSession->IsReady())
		{
			result = ProcessHandShake(pThis, pSocketObj, pSession);

			if(result == HR_ERROR)
				break;

			enStatus = SSL_HSS_SUCC;
			result	 = pThis->DoFireHandShake(pSocketObj);

			if(result == HR_ERROR)
				break;
		}

		const WSABUF& buffer = pSession->GetRecvBuffer();

		if(buffer.len == 0)
			break;

		result = pThis->DoFireReceive(pSocketObj, (const BYTE*)buffer.buf, buffer.len);

		if(result == HR_ERROR)
			break;
	}

	if(result != HR_ERROR && pSession->IsHandShaking())
		result = ::ProcessHandShake(pThis, pSocketObj, pSession);

	return result;
}

template<class T, class S> BOOL ProcessSend(T* pThis, S* pSocketObj, CSSLSession* pSession, const WSABUF * pBuffers, int iCount)
{
	if(pSession == nullptr || !pSession->IsReady())
	{
		::SetLastError(ERROR_INVALID_STATE);
		return FALSE;
	}

	CCriSecLock locallock(pSession->GetSendLock());

	if(!pSession->IsReady())
	{
		::SetLastError(ERROR_INVALID_STATE);
		return FALSE;
	}

	ENSURE(pSession->WriteSendChannel(pBuffers, iCount));

	while(TRUE)
	{
		ENSURE(pSession->ReadSendChannel());
		const WSABUF& buffer = pSession->GetSendBuffer();

		if(buffer.len == 0)
			break;

		if(!pThis->DoSendPackets(pSocketObj, &buffer, 1))
			return FALSE;
	}

	return TRUE;
}

#endif