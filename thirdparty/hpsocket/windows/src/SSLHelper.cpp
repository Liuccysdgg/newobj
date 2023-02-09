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
 
#include "Common/GeneralHelper.h"
#include "SSLHelper.h"

#ifdef _SSL_SUPPORT

#include "SocketHelper.h"
#include "Common/WaitFor.h"

#include <atlpath.h>

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/engine.h"
#include "openssl/x509v3.h"

/*
#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	#pragma comment(lib, "libeay32")
	#pragma comment(lib, "ssleay32")
#else
	#pragma comment(lib, "libssl")
	#pragma comment(lib, "libcrypto")
	#pragma comment(lib, "crypt32")
#endif
*/

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	int CSSLInitializer::sm_iLockNum			= 0;
	CSimpleRWLock* CSSLInitializer::sm_pcsLocks	= nullptr;
#endif

CSSLInitializer CSSLInitializer::sm_instance;

const DWORD CSSLSessionPool::DEFAULT_ITEM_CAPACITY		= CItemPool::DEFAULT_ITEM_CAPACITY;
const DWORD CSSLSessionPool::DEFAULT_ITEM_POOL_SIZE		= CItemPool::DEFAULT_POOL_SIZE;
const DWORD CSSLSessionPool::DEFAULT_ITEM_POOL_HOLD		= CItemPool::DEFAULT_POOL_HOLD;
const DWORD CSSLSessionPool::DEFAULT_SESSION_LOCK_TIME	= DEFAULT_OBJECT_CACHE_LOCK_TIME;
const DWORD CSSLSessionPool::DEFAULT_SESSION_POOL_SIZE	= DEFAULT_OBJECT_CACHE_POOL_SIZE;
const DWORD CSSLSessionPool::DEFAULT_SESSION_POOL_HOLD	= DEFAULT_OBJECT_CACHE_POOL_HOLD;

CSSLInitializer::CSSLInitializer()
{
#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	sm_iLockNum = CRYPTO_num_locks();

	if(sm_iLockNum > 0)
		sm_pcsLocks = new CSimpleRWLock[sm_iLockNum];
/*
#ifdef _DEBUG
	CRYPTO_malloc_debug_init();
	CRYPTO_dbg_set_options(V_CRYPTO_MDEBUG_ALL);
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
#endif
*/
	CRYPTO_set_locking_callback			(&ssl_lock_callback);
	CRYPTO_set_dynlock_create_callback	(&ssl_lock_dyn_create_callback);
	CRYPTO_set_dynlock_destroy_callback	(&ssl_lock_dyn_destroy_callback);
	CRYPTO_set_dynlock_lock_callback	(&ssl_lock_dyn_callback);

	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
#else
	OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, nullptr);
#endif
}

CSSLInitializer::~CSSLInitializer()
{
	//CleanupThreadState();

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	CONF_modules_free();
	ENGINE_cleanup();
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
#if OPENSSL_VERSION_NUMBER >= OPENSSL_VERSION_1_0_2
	SSL_COMP_free_compression_methods();
#endif

	CRYPTO_set_locking_callback			(nullptr);
	CRYPTO_set_dynlock_create_callback	(nullptr);
	CRYPTO_set_dynlock_destroy_callback	(nullptr);
	CRYPTO_set_dynlock_lock_callback	(nullptr);

	if(sm_iLockNum > 0)
	{
		delete[] sm_pcsLocks;

		sm_pcsLocks = nullptr;
		sm_iLockNum = 0;
	}
#endif
}

void CSSLInitializer::CleanupThreadState(DWORD dwThreadID)
{
#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	CRYPTO_THREADID tid = {nullptr, dwThreadID};
	
	CRYPTO_THREADID_current(&tid);
	ERR_remove_thread_state(&tid);
#else
	OPENSSL_thread_stop();
#endif
}

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0

void CSSLInitializer::ssl_lock_callback(int mode, int n, const char *file, int line)
{
	mode & CRYPTO_LOCK
	? (mode & CRYPTO_READ
		? sm_pcsLocks[n].WaitToRead()
		: sm_pcsLocks[n].WaitToWrite())
	: (mode & CRYPTO_READ
		? sm_pcsLocks[n].ReadDone()
		: sm_pcsLocks[n].WriteDone());
}

CRYPTO_dynlock_value* CSSLInitializer::ssl_lock_dyn_create_callback(const char *file, int line)
 {
	 return new DynamicLock;
 }

void CSSLInitializer::ssl_lock_dyn_callback(int mode, CRYPTO_dynlock_value* l, const char *file, int line)
{
	mode & CRYPTO_LOCK
	? (mode & CRYPTO_READ
		? l->cs.WaitToRead()
		: l->cs.WaitToWrite())
	: (mode & CRYPTO_READ
		? l->cs.ReadDone()
		: l->cs.WriteDone());
}

void CSSLInitializer::ssl_lock_dyn_destroy_callback(CRYPTO_dynlock_value* l, const char *file, int line)
{
	delete l;
}

#endif

BOOL CSSLContext::Initialize(EnSSLSessionMode enSessionMode, int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod, LPVOID lpCAPemCert, HP_Fn_SNI_ServerNameCallback fnServerNameCallback)
{
	ASSERT(!IsValid());

	if(IsValid())
	{
		::SetLastError(ERROR_INVALID_STATE);
		return FALSE;
	}

	m_enSessionMode	= enSessionMode;

	if(AddContext(iVerifyMode, bMemory, lpPemCert, lpPemKey, lpKeyPasswod, lpCAPemCert) == 0)
		m_sslCtx = GetContext(0);
	else
	{
		//EXECUTE_RESTORE_ERROR(Cleanup());
		return FALSE;
	}

	//SetServerNameCallback(fnServerNameCallback);

	return TRUE;
}

int CSSLContext::AddServerContext(int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod, LPVOID lpCAPemCert)
{
	ASSERT(IsValid());

	if(!IsValid())
	{
		::SetLastError(ERROR_INVALID_STATE);
		return FALSE;
	}

	if(m_enSessionMode != SSL_SM_SERVER)
	{
		::SetLastError(ERROR_INVALID_OPERATION);
		return FALSE;
	}

	return AddContext(iVerifyMode, bMemory, lpPemCert, lpPemKey, lpKeyPasswod, lpCAPemCert);
}

BOOL CSSLContext::BindServerName(LPCTSTR lpszServerName, int iContextIndex)
{
	ASSERT(lpszServerName && iContextIndex >= 0 && !::IsIPAddress(lpszServerName));

	if(!lpszServerName || iContextIndex < 0 || ::IsIPAddress(lpszServerName))
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	int iLen		= lstrlen(lpszServerName);
	LPCTSTR lpszSep	= ::StrChr(lpszServerName, SSL_DOMAIN_SEP_CHAR);

	if(lpszSep == nullptr || lpszSep == lpszServerName || lpszSep == (lpszServerName + iLen - 1))
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	int iSize = (int)m_lsSslCtxs.size();

	if(iSize <= iContextIndex)
	{
		::SetLastError(ERROR_INVALID_INDEX);
		return FALSE;
	}
	
	m_sslServerNames[lpszServerName] = iContextIndex;

	return TRUE;
}

int CSSLContext::AddContext(int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod, LPVOID lpCAPemCert)
{
	USES_CONVERSION;

	int iIndex = -1;

#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
	SSL_CTX* sslCtx	= SSL_CTX_new(SSLv23_method());
#else
	SSL_CTX* sslCtx	= SSL_CTX_new(TLS_method());
#endif

	SSL_CTX_set_quiet_shutdown(sslCtx, 1);
	SSL_CTX_set_verify(sslCtx, iVerifyMode, nullptr);

	if(!SSL_CTX_set_cipher_list(sslCtx, T2CA(m_strCipherList)))
		::SetLastError(ERROR_EMPTY);
	else
	{
		if(m_enSessionMode == SSL_SM_SERVER)
		{
			static volatile ULONG s_session_id_context = 0;
			ULONG session_id_context = ::InterlockedIncrement(&s_session_id_context);

			SSL_CTX_set_session_id_context(sslCtx, (BYTE*)&session_id_context, sizeof(session_id_context));
		}

		if(LoadCertAndKey(sslCtx, iVerifyMode, bMemory, lpPemCert, lpPemKey, lpKeyPasswod, lpCAPemCert))
		{
			iIndex = (int)m_lsSslCtxs.size();
			m_lsSslCtxs.push_back(sslCtx);
		}
	}

	if(iIndex < 0)
		EXECUTE_RESTORE_ERROR(SSL_CTX_free(sslCtx));
	
	return iIndex;
}

BOOL CSSLContext::LoadCertAndKey(SSL_CTX* sslCtx, int iVerifyMode, BOOL bMemory, LPVOID lpPemCert, LPVOID lpPemKey, LPVOID lpKeyPasswod, LPVOID lpCAPemCert)
{
	if(bMemory)
		return LoadCertAndKeyByMemory(sslCtx, iVerifyMode, (LPCSTR)lpPemCert, (LPCSTR)lpPemKey, (LPCSTR)lpKeyPasswod, (LPCSTR)lpCAPemCert);
	else
		return LoadCertAndKeyByFile(sslCtx, iVerifyMode, (LPCTSTR)lpPemCert, (LPCTSTR)lpPemKey, (LPCTSTR)lpKeyPasswod, (LPCTSTR)lpCAPemCert);
}

BOOL CSSLContext::LoadCertAndKeyByFile(SSL_CTX* sslCtx, int iVerifyMode, LPCTSTR lpszPemCertFile, LPCTSTR lpszPemKeyFile, LPCTSTR lpszKeyPassword, LPCTSTR lpszCAPemCertFileOrPath)
{
	USES_CONVERSION;

	if(::IsStrNotEmpty(lpszCAPemCertFileOrPath))
	{
		LPCTSTR lpszCAPemCertFile = nullptr;
		LPCTSTR lpszCAPemCertPath = nullptr;

		if(!ATLPath::FileExists(lpszCAPemCertFileOrPath))
		{
			::SetLastError(ERROR_FILE_NOT_FOUND);
			return FALSE;
		}

		if(!ATLPath::IsDirectory(lpszCAPemCertFileOrPath))
			lpszCAPemCertFile = lpszCAPemCertFileOrPath;
		else
			lpszCAPemCertPath = lpszCAPemCertFileOrPath;

		if(!SSL_CTX_load_verify_locations(sslCtx, T2CA(lpszCAPemCertFile), T2CA(lpszCAPemCertPath)))
		{
			::SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}

		if(!SSL_CTX_set_default_verify_paths(sslCtx))
		{
			::SetLastError(ERROR_FUNCTION_FAILED);
			return FALSE;
		}

		if(m_enSessionMode == SSL_SM_SERVER && (iVerifyMode & SSL_VM_PEER) && lpszCAPemCertFile != nullptr)
		{
			STACK_OF(X509_NAME)* caCertNames = SSL_load_client_CA_file(T2CA(lpszCAPemCertFile));

			if(caCertNames == nullptr)
			{
				::SetLastError(ERROR_EMPTY);
				return FALSE;
			}

			SSL_CTX_set_client_CA_list(sslCtx, caCertNames);
		}
	}

	if(::IsStrNotEmpty(lpszPemCertFile))
	{
		if(	!ATLPath::FileExists(lpszPemCertFile)	||
			ATLPath::IsDirectory(lpszPemCertFile)	)
		{
			::SetLastError(ERROR_FILE_NOT_FOUND);
			return FALSE;
		}

		if(	::IsStrEmpty(lpszPemKeyFile)			||
			!ATLPath::FileExists(lpszPemKeyFile)	||
			ATLPath::IsDirectory(lpszPemKeyFile)	)
		{
			::SetLastError(ERROR_FILE_NOT_FOUND);
			return FALSE;
		}
		
		if(::IsStrNotEmpty(lpszKeyPassword))
			SSL_CTX_set_default_passwd_cb_userdata(sslCtx, (void*)T2CA(lpszKeyPassword));

		if(!SSL_CTX_use_PrivateKey_file(sslCtx, T2CA(lpszPemKeyFile), SSL_FILETYPE_PEM))
		{
			::SetLastError(ERROR_INVALID_PASSWORD);
			return FALSE;
		}

		if(!SSL_CTX_use_certificate_chain_file(sslCtx, T2CA(lpszPemCertFile)))
		{
			::SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}

		if(!SSL_CTX_check_private_key(sslCtx))
		{
			::SetLastError(ERROR_INVALID_ACCESS);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSSLContext::LoadCertAndKeyByMemory(SSL_CTX* sslCtx, int iVerifyMode, LPCSTR lpszPemCert, LPCSTR lpszPemKey, LPCSTR lpszKeyPassword, LPCSTR lpszCAPemCert)
{
	if(!LoadCAPemCertByMemory(sslCtx, iVerifyMode, lpszCAPemCert))
		return FALSE;
	if(!LoadPemCertAndKeyByMemory(sslCtx, lpszPemCert, lpszPemKey, lpszKeyPassword))
		return FALSE;

	return TRUE;
}

BOOL CSSLContext::LoadCAPemCertByMemory(SSL_CTX* sslCtx, int iVerifyMode, LPCSTR lpszCAPemCert)
{
	if(::IsStrEmptyA(lpszCAPemCert))
		return TRUE;

	if(!AddCAPemCertToStoreByMemory(sslCtx, lpszCAPemCert))
		return FALSE;

	if(!SSL_CTX_set_default_verify_paths(sslCtx))
	{
		::SetLastError(ERROR_FUNCTION_FAILED);
		return FALSE;
	}

	if(m_enSessionMode == SSL_SM_SERVER && (iVerifyMode & SSL_VM_PEER))
	{
		if(!SetClientCAListByMemory(sslCtx, lpszCAPemCert))
			return FALSE;
	}

	return TRUE;
}

BOOL CSSLContext::LoadPemCertAndKeyByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert, LPCSTR lpszPemKey, LPCSTR lpszKeyPassword)
{
	if(::IsStrEmptyA(lpszPemCert))
		return TRUE;

	if(::IsStrEmptyA(lpszPemKey))
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	if(::IsStrNotEmptyA(lpszKeyPassword))
		SSL_CTX_set_default_passwd_cb_userdata(sslCtx, (void*)(lpszKeyPassword));

	if(!SetPrivateKeyByMemory(sslCtx, lpszPemKey))
		return FALSE;

	if(!SetCertChainByMemory(sslCtx, lpszPemCert))
		return FALSE;

	if(!SSL_CTX_check_private_key(sslCtx))
	{
		::SetLastError(ERROR_INVALID_ACCESS);
		return FALSE;
	}

	return TRUE;
}

BOOL CSSLContext::AddCAPemCertToStoreByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert)
{
	BOOL isOK			= FALSE;
	int iCount			= 0;
	BIO* pBIO			= BIO_new_mem_buf(lpszPemCert, -1);
	X509_STORE* pStore	= SSL_CTX_get_cert_store(sslCtx);
	STACK_OF(X509_INFO) * pStack = nullptr;

	if(pBIO == nullptr)
	{
		::SetLastError(ERROR_CREATE_FAILED);
		goto _END;
	}

	if(pStore == nullptr)
	{
		::SetLastError(ERROR_NOT_FOUND);
		goto _END;
	}

	pStack = PEM_X509_INFO_read_bio(pBIO, nullptr, nullptr, nullptr);

	if(pStack == nullptr)
	{
		::SetLastError(ERROR_NO_DATA);
		goto _END;
	}

	for(int i = 0; i < sk_X509_INFO_num(pStack); i++)
	{
		X509_INFO* pInfo = sk_X509_INFO_value(pStack, i);

		if(pInfo->x509)
		{
			if(!X509_STORE_add_cert(pStore, pInfo->x509))
			{
				::SetLastError(ERROR_INVALID_DATA);
				goto _END;
			}

			++iCount;
		}

		if(pInfo->crl)
		{
			if(!X509_STORE_add_crl(pStore, pInfo->crl))
			{
				::SetLastError(ERROR_INVALID_DATA);
				goto _END;
			}

			++iCount;
		}
	}

	if(iCount > 0)
		isOK = TRUE;
	else
		::SetLastError(ERROR_EMPTY);

_END:

	if(pStack != nullptr)
		sk_X509_INFO_pop_free(pStack, X509_INFO_free);

	if(pBIO != nullptr)
		BIO_free(pBIO);

	return isOK;
}

BOOL CSSLContext::SetClientCAListByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert)
{
	return false;
	
}

BOOL CSSLContext::SetPrivateKeyByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemKey)
{
	return false;
}

BOOL CSSLContext::SetCertChainByMemory(SSL_CTX* sslCtx, LPCSTR lpszPemCert)
{
	return false;
}

int CSSLContext::InternalServerNameCallback(SSL* ssl, int* ad, void* arg)
{
	return 0;
}

int __HP_CALL CSSLContext::DefaultServerNameCallback(LPCTSTR lpszServerName, PVOID pContext)
{
	CSSLContext* pThis = (CSSLContext*)pContext;

	if(pThis->m_sslServerNames.empty())
		return 0;

	LPCTSTR lpszTmp = lpszServerName;
	LPCTSTR lpszSep = ::StrChr(lpszTmp, SSL_DOMAIN_SEP_CHAR);

	while(lpszSep != nullptr)
	{
		CServerNameMap::const_iterator it = pThis->m_sslServerNames.find(lpszTmp);

		if(it != pThis->m_sslServerNames.end())
			return it->second;

		lpszTmp = (lpszSep + 1);
		lpszSep = ::StrChr(lpszTmp, SSL_DOMAIN_SEP_CHAR);
	}

	return 0;
}

SSL_CTX* CSSLContext::GetContext(int i) const
{
	SSL_CTX* sslCtx = nullptr;

	if(i >= 0 && i < (int)m_lsSslCtxs.size())
		sslCtx = m_lsSslCtxs[i];

	return sslCtx;
}

BOOL CSSLSession::WriteRecvChannel(const BYTE* pData, int iLength)
{
	ASSERT(pData && iLength > 0);

	BOOL isOK = TRUE;
	int bytes = BIO_write(m_bioRecv, pData, iLength);

	if(bytes > 0)
		ASSERT(bytes == iLength);
	else if(!BIO_should_retry(m_bioRecv))
	{
		::SetLastError(ERROR_INVALID_DATA);
		isOK = FALSE;
	}

	return isOK;
}

BOOL CSSLSession::ReadRecvChannel()
{
	BOOL isOK = TRUE;
	int bytes = SSL_read(m_ssl, m_bufRecv.buf, m_pitRecv->Capacity());

	if(bytes > 0)
		m_bufRecv.len = bytes;
	else if(!IsFatalError(bytes))
		m_bufRecv.len = 0;
	else
		isOK = FALSE;

	if(isOK && m_enStatus == SSL_HSS_PROC && SSL_is_init_finished(m_ssl))
		m_enStatus = SSL_HSS_SUCC;

	return isOK;
}

BOOL CSSLSession::WriteSendChannel(const BYTE* pData, int iLength)
{
	ASSERT(IsReady());
	ASSERT(pData && iLength > 0);

	BOOL isOK = TRUE;
	int bytes = SSL_write(m_ssl, pData, iLength);

	if(bytes > 0)
		ASSERT(bytes == iLength);
	else if(IsFatalError(bytes))
		isOK = FALSE;

	return isOK;
}

BOOL CSSLSession::WriteSendChannel(const WSABUF pBuffers[], int iCount)
{
	ASSERT(pBuffers && iCount > 0);

	BOOL isOK = TRUE;

	for(int i = 0; i < iCount; i++)
	{
		const WSABUF& buffer = pBuffers[i];

		if(buffer.len > 0)
		{
			if(!WriteSendChannel((const BYTE*)buffer.buf, buffer.len))
			{
				isOK = FALSE;
				break;
			}
		}
	}

	return isOK;
}

BOOL CSSLSession::ReadSendChannel()
{
	if(BIO_pending(m_bioSend) == 0)
	{
		m_bufSend.len = 0;
		return TRUE;
	}

	BOOL isOK = TRUE;
	int bytes = BIO_read(m_bioSend, m_bufSend.buf, m_pitSend->Capacity());

	if(bytes > 0)
		m_bufSend.len = bytes;
	else if(BIO_should_retry(m_bioSend))
		m_bufSend.len = 0;
	else
	{
		::SetLastError(ERROR_INVALID_DATA);
		isOK = FALSE;
	}

	return isOK;
}

CSSLSession* CSSLSession::Renew(const CSSLContext& sslCtx, LPCSTR lpszHostName)
{
	ASSERT(!IsValid());

	m_ssl		= SSL_new(sslCtx.GetDefaultContext());
	m_bioSend	= BIO_new(BIO_s_mem());
	m_bioRecv	= BIO_new(BIO_s_mem());

	SSL_set_bio(m_ssl, m_bioRecv, m_bioSend);

	if(sslCtx.GetSessionMode() == SSL_SM_SERVER)
		SSL_accept(m_ssl);
	else
	{
		USES_CONVERSION;

		if(lpszHostName && lpszHostName[0] != 0 && !::IsIPAddress(A2CT(lpszHostName)))
			SSL_set_tlsext_host_name(m_ssl, lpszHostName);

		SSL_connect(m_ssl);
	}

	m_pitSend		= m_itPool.PickFreeItem();
	m_pitRecv		= m_itPool.PickFreeItem();
	m_bufSend.buf	= (char*)m_pitSend->Ptr();
	m_bufRecv.buf	= (char*)m_pitRecv->Ptr();
	m_enStatus		= SSL_HSS_PROC;

	return this;
}

BOOL CSSLSession::Reset()
{
	BOOL isOK = FALSE;

	if(IsValid())
	{
		CCriSecLock locallock(m_csSend);

		if(IsValid())
		{
			m_enStatus = SSL_HSS_INIT;

			SSL_shutdown(m_ssl);
			SSL_free(m_ssl);

			m_itPool.PutFreeItem(m_pitSend);
			m_itPool.PutFreeItem(m_pitRecv);

			m_pitSend	= nullptr;
			m_pitRecv	= nullptr;
			m_ssl		= nullptr;
			m_bioSend	= nullptr;
			m_bioRecv	= nullptr;
			m_dwFreeTime= ::TimeGetTime();

			isOK = TRUE;
		}
	}

	ERR_clear_error();

	return isOK;
}

inline BOOL CSSLSession::IsFatalError(int iBytes)
{
	int iErrorCode = SSL_get_error(m_ssl, iBytes);

	if(	iErrorCode == SSL_ERROR_NONE			||
		iErrorCode == SSL_ERROR_WANT_READ		||
		iErrorCode == SSL_ERROR_WANT_WRITE		||
		iErrorCode == SSL_ERROR_WANT_CONNECT	||
		iErrorCode == SSL_ERROR_WANT_ACCEPT		)
		return FALSE;

#ifdef _DEBUG
	char szBuffer[512];
#endif

	int i		= 0;
	int iCode	= iErrorCode;

	for(; iCode != SSL_ERROR_NONE; i++)
	{
#ifdef _DEBUG
		ERR_error_string_n(iCode, szBuffer, sizeof(szBuffer));
		TRACE("	> SSL Error: %d - %s\n", iCode, szBuffer);
#endif

		iCode = ERR_get_error();
	}

	if(iErrorCode == SSL_ERROR_SYSCALL && i == 1)
		return FALSE;

	::SetLastError(ERROR_INVALID_DATA);

	return TRUE;
}

BOOL CSSLSession::GetSessionInfo(EnSSLSessionInfo enInfo, LPVOID* lppInfo)
{
	return FALSE;
}

CSSLSession* CSSLSessionPool::PickFreeSession(LPCSTR lpszHostName)
{
	DWORD dwIndex;
	CSSLSession* pSession = nullptr;

	if(m_lsFreeSession.TryLock(&pSession, dwIndex))
	{
		if(::GetTimeGap32(pSession->GetFreeTime()) >= m_dwSessionLockTime)
			ENSURE(m_lsFreeSession.ReleaseLock(nullptr, dwIndex));
		else
		{
			ENSURE(m_lsFreeSession.ReleaseLock(pSession, dwIndex));
			pSession = nullptr;
		}
	}

	if(!pSession) pSession = CSSLSession::Construct(m_itPool);

	ASSERT(pSession);
	return pSession->Renew(m_sslCtx, lpszHostName);
}

void CSSLSessionPool::PutFreeSession(CSSLSession* pSession)
{
	if(pSession->Reset())
	{
		ReleaseGCSession();
		
		if(!m_lsFreeSession.TryPut(pSession))
			m_lsGCSession.PushBack(pSession);
	}
}

void CSSLSessionPool::Prepare()
{
	m_itPool.Prepare();
	m_lsFreeSession.Reset(m_dwSessionPoolSize);
}

void CSSLSessionPool::Clear()
{
	m_lsFreeSession.Clear();

	ReleaseGCSession(TRUE);
	ENSURE(m_lsGCSession.IsEmpty());

	m_itPool.Clear();
}

void CSSLSessionPool::ReleaseGCSession(BOOL bForce)
{
	::ReleaseGCObj(m_lsGCSession, m_dwSessionLockTime, bForce);
}

#endif