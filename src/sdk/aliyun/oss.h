#pragma once
#include "public/define.h"
#include "basetype/string.h"
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
                 * Class��������OSS�ͻ���
                 *************************************************************/
                class client
                {
                public:
                    /********************************************************************
                     * Param
                     *          endpoint                    ��           ��:http://oss-cn-hangzhou.aliyuncs.com
                     *          accessKeyId
                     *          accessKeySecret
                     ********************************************************************/
                    client(const nstring& endpoint, const nstring& accessKeyId, const nstring& accessKeySecret);
                    ~client();
                    /*******************************************************************
                     * Function���ϴ�
                     * Param
                     *          filepath                    ��           ����·��
                     *          bucket                      ��          bucket����
                     *          remote_path           ��             Զ��·��
                     *          errormsg                    ��           ��������
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