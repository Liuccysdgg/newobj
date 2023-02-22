#include "oss.h"
#include <fstream>
#ifndef MSVC_2010
#ifdef SDK_ALIYUN_OSS
#include "alibabacloud/oss/OssClient.h"
newobj::sdk::aliyun::oss::client::client(const nstring& endpoint, const nstring& accessKeyId, const nstring& accessKeySecret)
{
    m_client = nullptr;
    AlibabaCloud::OSS::InitializeSdk();
    AlibabaCloud::OSS::ClientConfiguration conf;
    conf.maxConnections = 20;
    /* 设置连接池数，默认为16个。*/
    conf.maxConnections = 20;

    /* 设置请求超时时间，超时没有收到数据就关闭连接，默认为10000ms。*/
    conf.requestTimeoutMs = 8000;

    /* 设置建立连接的超时时间，默认为5000ms。*/
    conf.connectTimeoutMs = 8000;
    m_client = new AlibabaCloud::OSS::OssClient(endpoint, accessKeyId, accessKeySecret, conf);
}

newobj::sdk::aliyun::oss::client::~client()
{
    delete m_client;
    AlibabaCloud::OSS::ShutdownSdk();
}

bool newobj::sdk::aliyun::oss::client::upload(const nstring& filepath, const nstring& bucket, const nstring& remote_path, nstring* errormsg)
{
    std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(filepath, std::ios::in | std::ios::binary);

    AlibabaCloud::OSS::PutObjectRequest request(bucket, remote_path, content);
    /* 上传文件 */
    auto outcome =m_client->PutObject(request);

    if (!outcome.isSuccess()) {
        /* 异常处理 */
        if (errormsg != nullptr)
        {
            *errormsg = nstring("PutObject fail" +
                nstring(",code:") + outcome.error().Code() +
                ",message:" +outcome.error().Message() +
                ",requestId:"+ outcome.error().RequestId());
        }
        return false;
    }
    return true;

}
bool newobj::sdk::aliyun::oss::client::del(const nstring& bucket, const nstring& remote_path, nstring* errorMsg)
{
    AlibabaCloud::OSS::DeleteObjectRequest request(bucket, remote_path);
    /*删除文件 */
    auto outcome = m_client->DeleteObject(request);
    if (!outcome.isSuccess()) {
        /* 异常处理 */
        if (errorMsg != nullptr)
        {
            *errorMsg = nstring("DeleteObject fail" +
                nstring(",code:") + outcome.error().Code() +
                ",message:" + outcome.error().Message() +
                ",requestId:" + outcome.error().RequestId());
        }
        return false;
    }
    return true;
}
#endif
#endif