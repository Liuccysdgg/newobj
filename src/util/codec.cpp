#include "codec.h"
#ifdef LIB_HPSOCKET
#include "hpsocket/hpsocket.h"
#endif
#ifdef LIB_BOOST
#include "boost/uuid/name_generator_sha1.hpp"
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#endif
#include "public/exception.h"
#include "sdk/codec/base64.h"
#include "sdk/codec/qaesencryption.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#if LIB_QT == 1
#include "QtCore/quuid.h"
#include "QtCore/qcryptographichash.h"
#endif



nstring newobj::codec::url::en(const nstring& str)
{
    if (str.length() <= 0)
        return "";
    if (str.length() > NEWOBJ_BUFFER_MAX_SIZE)
        return "";
    newobj::buffer result;
    result.resize(str.length() * 5);
    DWORD resultLen = str.length() * 5;
    auto rcode = SYS_UrlEncode((BYTE*)str.c_str(), str.length(), (BYTE*)result.data(), resultLen);
    return result.left(resultLen);
}

nstring newobj::codec::url::de(const nstring& str)
{
    if (str.length() <= 0)
        return "";
    if (str.length() > NEWOBJ_BUFFER_MAX_SIZE)
        return "";
    newobj::buffer result;
    result.resize(str.length() * 2);
    DWORD resultLen = str.length() * 2;
    auto rcode = SYS_UrlDecode((BYTE*)str.c_str(), str.length(), (BYTE*)result.data(), resultLen);
    return result.left(resultLen);
}
nstring  newobj::codec::base64::en(const nstring& str)
{
    return newobj::codec::base64::en(newobj::buffer(str));
}

nstring  newobj::codec::base64::en(const stream_view& data)
{
#if 0
    if (data.length() == 0)
        return "";
    newobj::buffer result;
    uint32 out_length = BASE64_ENCODE_OUT_SIZE(data.length());
    if (out_length == 0)
        return "";
    result.resize(out_length);
    uint32 size = base64_encode((uchar*)data.data(), data.length(), (char*)result.data());
    if(size > out_length)
        return "";
    return result.to_string();
#else
    DWORD resultLen = SYS_GuessBase64EncodeBound(data.length());
    if (resultLen <= 0)
        return newobj::buffer();
    if (resultLen > NEWOBJ_BUFFER_MAX_SIZE)
        return newobj::buffer();
    newobj::buffer result;
    result.resize(resultLen);
    auto rcode = SYS_Base64Encode((const BYTE*)data.data(), data.length(), (BYTE*)result.data(), resultLen);
    return result.left(resultLen);
#endif
}

newobj::buffer  newobj::codec::base64::de(const nstring& data)
{
#if 0
    newobj::buffer result;
    uint32 out_length = BASE64_DECODE_OUT_SIZE(data.length());
    if (out_length == 0)
        return newobj::buffer();
    result.resize(out_length);
    uint32 size = base64_decode((char*)data.c_str(), data.length(), (uchar*)result.data()); 
    if (size > out_length)
        return newobj::buffer();
    return result.left(size);
#else
    DWORD resultLen = SYS_GuessBase64DecodeBound((const BYTE*)data.c_str(),data.length());
    if (resultLen <= 0)
        return newobj::buffer();
    if (resultLen > NEWOBJ_BUFFER_MAX_SIZE)
        return newobj::buffer();
    newobj::buffer result;
    result.resize(resultLen);
    auto rcode = SYS_Base64Decode((const BYTE*)data.c_str(), data.length(), (BYTE*)result.data(), resultLen);
    return newobj::buffer(result.left(resultLen));
#endif
}
nstring newobj::codec::to_utf8(const nstring& gbk)
{
#ifdef _WIN32
    //gbk转unicode  
    int len = MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, NULL, 0);
    wchar_t* strUnicode = new wchar_t[len];
    wmemset(strUnicode, 0, len);
    MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, strUnicode, len);

    //unicode转UTF-8  
    len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
    char* strUtf8 = new char[len+1];
    strUtf8[len] = 0;
    WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

    std::string strTemp(strUtf8);//此时的strTemp是UTF-8编码  
    delete[]strUnicode;
    delete[]strUtf8;
    strUnicode = NULL;
    strUtf8 = NULL;
    return strTemp;
#else
    char *dst = new char[gbk.length()*4];
    int dst_length = gbk.length()*4;
    if(SYS_GbkToUtf8(gbk.c_str(),dst,dst_length) == false)
        return "";
    return nstring(dst,dst_length);
#endif
}

nstring newobj::codec::to_gbk(const nstring& utf8)
{
#ifdef _WIN32
    //UTF-8转unicode  
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    wchar_t* strUnicode = new wchar_t[len];//len = 2  
    wmemset(strUnicode, 0, len);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, strUnicode, len);

    //unicode转gbk  
    len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
    char* strGbk = new char[len+1];//len=3 本来为2，但是char*后面自动加上了\0  
    memset(strGbk, 0, len);
    WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strGbk, len, NULL, NULL);
    strGbk[len] = 0;
    std::string strTemp(strGbk);//此时的strTemp是GBK编码  
    delete[]strUnicode;
    delete[]strGbk;
    strUnicode = NULL;
    strGbk = NULL;
    return strTemp;
#else
    char *dst = new char[utf8.length()*4];
    int dst_length = utf8.length()*4;
    if(SYS_Utf8ToGbk(utf8.c_str(),dst,dst_length) == false)
        return "";
    return nstring(dst,dst_length);
#endif
}
#ifdef LIB_HPSOCKET
newobj::buffer  newobj::codec::ungzip(const stream_view& data)
{
    uint32 size = SYS_GZipGuessUncompressBound((const BYTE*)data.data(),data.length());
    if (size <= 0)
        return newobj::buffer();
    if (size > NEWOBJ_BUFFER_MAX_SIZE)
        return newobj::buffer();
    newobj::buffer result;
	result.resize(size*2);
    DWORD resultLen = size*2;
	auto rcode = SYS_GZipUncompress((const BYTE*)data.data(), data.length(), (BYTE*)result.data(), resultLen);
	return newobj::buffer(result.left(resultLen));
}
newobj::buffer  newobj::codec::gzip(const stream_view& data)
{
    newobj::buffer result;
    result.resize(data.length()+1024);
    DWORD resultLen = result.length();
    if (SYS_GZipCompress((const BYTE*)data.data(), data.length(), (BYTE*)result.data(), resultLen) == 0)
        return newobj::buffer(result.left(resultLen));
    return result;
}
newobj::buffer  newobj::codec::hp_compress(const stream_view& data)
{
    newobj::buffer result;
    result.resize(data.length() + 1024);
    DWORD resultLen = result.length();
    if (SYS_Compress((const BYTE*)data.data(), data.length(), (BYTE*)result.data(), resultLen) == 0)
        return newobj::buffer(result.left(resultLen));
    return result;
}
newobj::buffer  newobj::codec::hp_uncompress(const stream_view& data)
{
    if (data.length() < 1)
        return newobj::buffer();
    newobj::buffer result;
    result.resize(data.length());
    DWORD resultLen = result.length();
    auto rcode = SYS_Uncompress((const BYTE*)data.data(), data.length(), (BYTE*)result.data(), resultLen);
    if (rcode == 0)
        return newobj::buffer();
    return newobj::buffer(result.left(resultLen));
}
#endif
#ifdef LIB_BOOST
//nstring newobj::codec::md5(const char* buff, uint32 len)
//{
//    if (buff == nullptr || len == 0)
//    {
//        trw_str("The data to be encrypted is empty or the length is equal to 0");
//    }
//
//    boost::uuids::detail::md5 boost_md5;
//    boost_md5.process_bytes(buff, len);
//    boost::uuids::detail::md5::digest_type digest;
//    boost_md5.get_digest(digest);
//    const auto char_digest = reinterpret_cast<const char*>(&digest);
//    nstring str_md5;
//    boost::algorithm::hex(char_digest, char_digest + sizeof(boost::uuids::detail::md5::digest_type), std::back_inserter(str_md5));
//
//    return str_md5;
//}

buffer newobj::codec::sha1(const char* buff, uint32 len)
{
    buffer hash;
    if (buff == nullptr || len == 0)
    {
        return buffer();
    }
    boost::uuids::detail::sha1 s1;
    boost::uuids::detail::sha1::digest_type digest;
    s1.process_bytes(buff, len);
    s1.get_digest(digest);
    
    hash.setsize(20);
    for (int i = 0; i < 5; ++i) {
        const char* tmp = reinterpret_cast<char*>(digest);
        hash[i * 4] = tmp[i * 4 + 3];
        hash[i * 4 + 1] = tmp[i * 4 + 2];
        hash[i * 4 + 2] = tmp[i * 4 + 1];
        hash[i * 4 + 3] = tmp[i * 4];
    }
    return hash;
}

#endif

nstring newobj::codec::uuid()
{
#if LIB_QT == 1
    auto uuid = nstring::from(QUuid::createUuid().toString());
    for (size_t i = 0; i < uuid.length(); i++)
    {
        if (uuid[i] == '}' || uuid[i] == '{')
            uuid[i] = ' ';
    }
    return uuid.replace(' ',"");
#else
    // 非QT暂不支持哦~
    assert(0);
	return "";
#endif
}
#if LIB_QT == 1
buffer  newobj::codec::aes::en(const stream_view& src, const nstring& key_param, int type)
{
    if(src.length() == 0)
        return src;
    newobj::buffer en_data = src;
    auto lb = src.length() % 16;
    if (lb != 0)
    {
        for (uint32 i = 0; i < 16 - lb; i++)
            en_data.append('\0');
    }
    QAESEncryption encryption((QAESEncryption::Aes)type, QAESEncryption::ECB, QAESEncryption::ZERO);
    //QByteArray hashKey = QCryptographicHash::hash(key_param.operator QByteArray(), QCryptographicHash::Md5);
    return newobj::buffer(encryption.encode(en_data, key_param));
}

buffer  newobj::codec::aes::de(const stream_view& src, const nstring& key_param, int type)
{
    if (src.length() == 0)
        return src;
    newobj::buffer en_data = src;
    auto lb = src.length() % 16;
    if (lb != 0)
    {
        for (uint32 i = 0; i < 16 - lb; i++)
        {
            en_data.append('\0');
        }
    }

    QAESEncryption encryption((QAESEncryption::Aes)type, QAESEncryption::ECB, QAESEncryption::ZERO);
    //QByteArray hashKey2 = QCryptographicHash::hash(key_param, QCryptographicHash::Md5);
    auto result = encryption.decode(en_data, key_param);
    newobj::buffer r2;
    r2.append(result);
    return r2;
}
#endif
