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
             * class��FORM��������
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
             * class��������
             *************************************************************************/
            class parser 
            {
            public:
                /******************************************************************
                 * function����ʼ��
                 * param
                 *
                 *      url                     ��           �����ַ
                 *      method                  ��           ��������
                 *      data                    ��           ��������
                 *      content_type            ��           ����Э��ͷ
                 ******************************************************************/
                bool init(const nstring& url,const network::http::method& method,newobj::buffer* data,const nstring& content_type);
            public:
                parser();
                ~parser();

                const newobj::json& json();
                nstring text();
                /******************************************************************
                 * function��ȡURL����
                 * param
                 *      name                    ��           ����
                 *      value                   ��           ����
                 ******************************************************************/
                bool url_param(const nstring& name,nstring& value);
                bool url_param_exist(const nstring& name);
                /******************************************************************
                 * function��ȡURL����������б�
                 ******************************************************************/
                std::vector<nstring> url_param_names();
                /******************************************************************
                 * function��ȡBODY����
                 * param
                 *      name                    ��           ����
                 *      value                   ��           ����
                 ******************************************************************/
                bool body_param(const nstring& name,nstring& value);
                bool body_param_exist(const nstring& name);
                /******************************************************************
                 * function��ȡBODY����������б�
                 ******************************************************************/
                std::vector<nstring> body_param_names();
                /******************************************************************
                 * function��ȡBODY����������б�
                 ******************************************************************/
                form_parser* form();
            private:
                /******************************************************************
                 * function������URL
                 ******************************************************************/
                void parse_url(const nstring& url,std::map<nstring,nstring>& map);
                /******************************************************************
                 * function������JSON
                 ******************************************************************/
                void parse_json();
                /******************************************************************
                 * function������FORM��
                 ******************************************************************/
                void parse_form();
                /******************************************************************
                 * function������URL��ʽ����
                 ******************************************************************/
                bool read_url_param(const std::map<nstring,nstring>& map,const nstring& name,nstring& value);
            private:
                // URL����
                std::map<nstring,nstring> m_url_param;
                // BODY����
                std::map<nstring,nstring> m_body_param;
                // JSON����
                newobj::json m_json_param;
                // FORM������
                network::http::form_parser m_form;
                // �����ַ
                nstring m_url;
                // ��������
                newobj::buffer* m_data;
                // ������������
                nstring m_content_type;
                // ��������
                network::http::method m_method;

            };

		}
	}
}
#endif