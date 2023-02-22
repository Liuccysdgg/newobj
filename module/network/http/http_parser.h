#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "public/buffer.h"
#include "util/forstring.h"
#include <map>
namespace newobj
{
	namespace network
	{
		namespace http
		{
            class parser;
            /*************************************************************************
             * class：FORM表单解析器
             *************************************************************************/
            class MNETWORK_API form_parser{
            public:
                form_parser();
                ~form_parser();


                std::vector<nstring> names();
                bool get(const nstring& name,form_info& info);
                bool write_file(const nstring& name,const nstring& filepath);

                friend class parser;
            private:
                bool parse(newobj::buffer* data);
                void parse_count(std::vector<uint32>& starts,std::vector<uint32>& lengths);
                void parse_form(uint32 start,uint32 length);
                char getchar(size_t index);
            private:
                newobj::buffer* m_data;
                std::map<nstring,form_info> m_infos;
            };
            /*************************************************************************
             * class：解析器
             *************************************************************************/
            class parser 
            {
            public:
                /******************************************************************
                 * function：初始化
                 * param
                 *
                 *      url                     ：           请求地址
                 *      method                  ：           请求类型
                 *      data                    ：           请求数据
                 *      content_type            ：           类型协议头
                 ******************************************************************/
                bool init(const nstring& url,const network::http::method& method,newobj::buffer* data,const nstring& content_type);
            public:
                parser();
                ~parser();

                const newobj::json& json();
                nstring text();
                /******************************************************************
                 * function：取URL参数
                 * param
                 *      name                    ：           名称
                 *      value                   ：           数据
                 ******************************************************************/
                bool url_param(const nstring& name,nstring& value);
                bool url_param_exist(const nstring& name);
                /******************************************************************
                 * function：取URL请求参数名列表
                 ******************************************************************/
                std::vector<nstring> url_param_names();
                /******************************************************************
                 * function：取BODY参数
                 * param
                 *      name                    ：           名称
                 *      value                   ：           数据
                 ******************************************************************/
                bool body_param(const nstring& name,nstring& value);
                bool body_param_exist(const nstring& name);
                /******************************************************************
                 * function：取BODY请求参数名列表
                 ******************************************************************/
                std::vector<nstring> body_param_names();
                /******************************************************************
                 * function：取BODY请求参数名列表
                 ******************************************************************/
                form_parser* form();
            private:
                /******************************************************************
                 * function：解析URL
                 ******************************************************************/
                void parse_url(const nstring& url,std::map<nstring,nstring>& map);
                /******************************************************************
                 * function：解析JSON
                 ******************************************************************/
                void parse_json();
                /******************************************************************
                 * function：解析FORM表单
                 ******************************************************************/
                void parse_form();
                /******************************************************************
                 * function：解析URL格式数据
                 ******************************************************************/
                bool read_url_param(const std::map<nstring,nstring>& map,const nstring& name,nstring& value);
            private:
                // URL参数
                std::map<nstring,nstring> m_url_param;
                // BODY参数
                std::map<nstring,nstring> m_body_param;
                // JSON参数
                newobj::json m_json_param;
                // FORM解析器
                network::http::form_parser m_form;
                // 请求地址
                nstring m_url;
                // 请求数据
                newobj::buffer* m_data;
                // 请求内容类型
                nstring m_content_type;
                // 请求类型
                network::http::method m_method;

            };

		}
	}
}
#endif