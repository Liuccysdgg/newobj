#pragma once
#include "public/define.h"
#include "public/string.h"
#ifdef SDK_ALIYUN_OSS


namespace AlibabaCloud
{
    namespace OSS
    {
        class OssClient;
    }
}

namespace newobj
{
    namespace sdk
    {
        namespace aliyun
        {
            namespace oss
            {

                /*************************************************************
                 * Class：阿里云OSS客户端
                 *************************************************************/
                class client
                {
                public:
                    /********************************************************************
                     * Param
                     *          endpoint                    ：           例:http://oss-cn-hangzhou.aliyuncs.com
                     *          accessKeyId
                     *          accessKeySecret
                     ********************************************************************/
                    client(const nstring& endpoint, const nstring& accessKeyId, const nstring& accessKeySecret);
                    ~client();
                    /*******************************************************************
                     * Function：上传
                     * Param
                     *          filepath                    ：           本地路径
                     *          bucket                      ：          bucket名称
                     *          remote_path           ：             远程路径
                     *          errormsg                    ：           错误描述
                     *******************************************************************/
                    bool upload(const nstring& filepath, const nstring& bucket, const nstring& remote_path, nstring* errormsg);
                    bool del(const nstring& bucket, const nstring& remote_path,nstring* errorMsg);
                private:
                    AlibabaCloud::OSS::OssClient* m_client;
                };
            }
        }
    }
}
#endif