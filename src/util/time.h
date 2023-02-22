#pragma once
#include "basetype/string.h"
namespace newobj
{

	
	namespace time
	{
			struct datetime
			{
				datetime() {
					year = 0;
					month = 0;
					day = 0;
					hour = 0;
					minute = 0;
					second = 0;
				}
				short year;
				char month;
				char day;
				char hour;
				char minute;
				char second;
			};

			/*����ʱ��*/
			timestamp network_msec();

			/*��׼�ַ���ʱ��תʱ���*/
			timestamp NEWOBJ_API to_ts(const nstring& timestr, const nstring& formart = "%d-%d-%d %d:%d:%d");
			timestamp  NEWOBJ_API to_ts(struct newobj::time::datetime& datetime);
			/*��ǰʱ���*/
			uint32 NEWOBJ_API now_sec();
			timestamp NEWOBJ_API now_msec();
			timestamp NEWOBJ_API now_usec();
			/*

					%a ���ڼ��ļ�д
					%A ���ڼ���ȫ��
					%b �·ֵļ�д
					%B �·ݵ�ȫ��
					%c ��׼�����ڵ�ʱ�䴮
					%C ��ݵĺ���λ����
					%d ʮ���Ʊ�ʾ��ÿ�µĵڼ���
					%D ��/��/��
					%e �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���
					%F ��-��-��
					%g ��ݵĺ���λ���֣�ʹ�û����ܵ���
					%G ��֣�ʹ�û����ܵ���
					%h ��д���·���
					%H 24Сʱ�Ƶ�Сʱ
					%I 12Сʱ�Ƶ�Сʱ
					%j ʮ���Ʊ�ʾ��ÿ��ĵڼ���
					%m ʮ���Ʊ�ʾ���·�
					%M ʮʱ�Ʊ�ʾ�ķ�����
					%n ���з�
					%p ���ص�AM��PM�ĵȼ���ʾ
					%r 12Сʱ��ʱ��
					%R ��ʾСʱ�ͷ��ӣ�hh:mm
					%S ʮ���Ƶ�����
					%t ˮƽ�Ʊ��
					%T ��ʾʱ���룺hh:mm:ss
					%u ÿ�ܵĵڼ��죬����һΪ��һ�� ��ֵ��0��6������һΪ0��
					%U ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��
					%V ÿ��ĵڼ��ܣ�ʹ�û����ܵ���
					%w ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��
					%W ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��
					%x ��׼�����ڴ�
					%X ��׼��ʱ�䴮
					%y �������͵�ʮ������ݣ�ֵ��0��99��
					%Y �����Ͳ��ֵ�ʮ�������
					%z��%Z ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���
					%% �ٷֺ�
			*/
			/*ȡʱ���ַ���*/
			nstring  NEWOBJ_API now_time(const nstring& format = "%Y-%m-%d %H:%M:%S");

			nstring  NEWOBJ_API format(timestamp time,const nstring& format = "%Y-%m-%d %H:%M:%S");

			void NEWOBJ_API format(timestamp time, struct newobj::time::datetime& datetime);


			

            newobj::time::datetime  NEWOBJ_API now_time2();

			/*��ȡ��ǰʱ��*/
			int32  NEWOBJ_API now_zone();
			/*��ȡ��ǰGMTʱ��*/
			void  NEWOBJ_API now_gmt(nstring& gmt);
			/*ʱ���תGMTʱ��*/
			void  NEWOBJ_API to_gmt(const timestamp &timestamp,nstring& gmt);

			/*ȡ��ǰ�賿ʱ��*/
			timestamp now_zero_sec();
			nstring now_zero_time();
		}
}
