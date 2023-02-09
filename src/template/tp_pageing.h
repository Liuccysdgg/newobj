#pragma once

#include "util/forstring.h"
#include "public/var.h"
#include "util/json.h"
#include "tp_sqlstatement.h"

#ifdef LIB_DB_MYSQL
#ifndef MSVC_2010
#include <initializer_list>
namespace newobj
{
	namespace web
	{
		/*��ҳ��ѯ����*/
		struct pageing_query_param
		{
			//����json������
			nstring query_name;
			//��������
			NVarType type;
			//�������SQL��ѯ���ֶ�����Ĭ����� queryName һ��
			nstring sql_field;
		};


		/***********************************************************
		 * Class����ҳģ��
		 * Desc��ǰ�˷�ҳ����
		 ***********************************************************/
		class NEWOBJ_API pageing
		{
		public:
			pageing(newobj::network::http::controller* httpCTL,newobj::db::mysql::pool* pool);
			~pageing();
			pageing& fields(std::initializer_list<nstring> fields);
			pageing& table(const nstring& table);
			/*���ҳ����*/
			pageing& max_count(uint32 pageMaxCount);
			pageing& where(const newobj::where& where);
			pageing& orderby(const nstring& orderBy, newobj::sort_type sort);
			/*ȡ��ѯ����*/
			uint64 count();
			/*ȡ��ѯ���*/
			void query(newobj::json& tableJson);
			nvar param(const nstring& name, NVarType varType);
			friend class newobj::network::http::controller;
		private:
			//��ѯ����
			newobj::where2 m_where;
			newobj::network::http::controller* m_httpCtl;
			//���ҳ����
			uint32 m_pageMaxCount = 10;
			//��ѯ�ֶ�
			std::vector<nstring> m_fields;
			//��
			nstring m_table;
			//����
			newobj::sort m_sort;
			// ���ӳ�
			db::mysql::pool* m_pool;
		};
	}

}


#endif
#endif