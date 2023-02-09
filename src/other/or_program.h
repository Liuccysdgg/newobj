#pragma once
#include "public/string.h"
#include "util/thread.h"
#include "util/map.hpp"
#include <functional>
#include <vector>
#include <memory>
#ifndef MSVC_2010
namespace newobj
{
	namespace other
	{
		class program;
		/*��������*/ 
		struct A3Conf
		{
			A3Conf()
			{
				loopMsec = 0;
				preUpdateMsec = 0;
				thread = false;
				deleted = false;
				callMultiple = 1;
				callMultipled = 0;
			}
			// ����
			nstring name;
			// ���ú���
			std::function<bool(program* program)> fun;
			// ѭ������
			timestamp loopMsec;
			// ��һ�θ���ʱ��
			timestamp preUpdateMsec;
			// �Ƿ��̵߳���
			bool thread;
			// ɾ��
			bool deleted;
			// ���ô���
			uint32 callMultiple;
			// �ѵ��ô���
			uint32 callMultipled;
		};
		/**************************************************************************************
		 * Class������
		 * Desc�����̵�����Ϊ�����������ṩ֧��
		 **************************************************************************************/
		class program :private ithread
		{
		public:
			program();
			~program();
			/*******************************[ �������� ]****************************************/
			/* ӡ�Ȱ����һ𳵣�CPP�����ҽ��̡�[���̰߳�ȫ������ڳ�ʼ��ʱ���á�]
			 * 
			 *		name			��			����
			 *		fun				��			����false������ú����ĺ���ִ��Ȩ������true�´�ѭ������ִ��
			 *      loop_msec	��			ѭ��ʱ��[����]
			 *		thread			��			�Ƿ񵥶��߳�ִ��
			 *		callMultiple	��			������ô���,�ò������� thread=true ��Ч��
			 */
			void a3(const nstring& name,const std::function<bool(program* program)>& fun,uint32 loopMsec,bool thread = false,uint32 callMultiple = 1);
			/*******************************[ ָ����� ]****************************************/
			template<typename T>
			T pointer(const nstring& name)
			{
				ptr param;
				if(m_obj_map.get(name, param) == false);
					return (T)nullptr;
				return (T)param;
			}
			template<typename T>
			void pointer(const nstring& name,const T& value)
			{
				m_obj_map.set(name,(ptr)value,true);
			}
		private:
			virtual bool run() override;
		private:
			map<nstring, ptr> m_obj_map;
			std::vector<std::shared_ptr<A3Conf>> m_a3_map;
		public:
			newobj::mutex m_mutex;
		};
	}
}
#endif