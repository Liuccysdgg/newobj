#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include <functional>
#include "thread.h"
#include <list>
#ifdef _WIN32
namespace newobj
{
	namespace process
	{
		/*�����ػ�*/ 
		class guard :private newobj::ithread
		{
		public:
			struct process_info
			{
				process_info() { pid = 0; extra = nullptr; }
				nstring path;
				uint32 pid;
				void* extra;
				std::function<bool(const process_info& proc, void* extra)> callback;
			};
		public:
			guard();
			~guard();
			/*
				�����ػ�������Ϣ[�Զ���������]
				@path				��				����·��
				@extra				��				��������
				@callback		��				�����쳣�����ص�
			*/
			bool add(const nstring& path, void* extra, std::function<bool(const process_info& proc, void* extra)> callback);
			/*�Ƴ��ػ�������Ϣ*/
			void* remove(const nstring& path);
		private:
			std::vector<process_info> m_map;

			// ͨ�� thread �̳�
			virtual bool run() override;
		};
	}
	namespace process
	{
		/**
		* @brief ͨ������ID������·��
		* @return
		*/
		//nstring NEWOBJ_API getpath(uint32 pid);
		nstring NEWOBJ_API getpath(uint32 pid);
		/*��������*/
		struct ProcData
		{
			ProcData()
			{
				parent = 0;
				id = 0;
			}
			nstring path()
			{
				return process::getpath(id);
			}
			nstring name;
			nstring __path;
			ptr parent;
			ptr id;
		};
		/*
			����Ӧ��
			@param
					rundir						��						Ӧ������Ŀ¼������wait=trueʱ��Ч
					wait							:						�ȴ�Ӧ�ý���
			@return
					==0		���ɹ�
					  !=0		��������
		*/
		int32 NEWOBJ_API exec(const nstring& path, const nstring& rundir,bool wait = false,bool show = true);
		/**
				* @brief ȡϵͳ�����б�
				* @return 
				*/
		 void NEWOBJ_API list(std::list<ProcData> & value);

		/**
			* @brief �Ƿ����ָ������·��
			* @param procpath
			* @return
			*/
		bool NEWOBJ_API exist(const nstring& procpath);
		/*ͨ������·��ȡPID*/
		uint32 NEWOBJ_API getpid(const nstring& procpath);
		/*Kill����*/
		bool NEWOBJ_API kill(uint32 pid);
		/*������ID*/
		uint32 NEWOBJ_API current_pid();
		/*
			���࿪
			Desc��ʹ�øú�������ֹ�࿪����δ��⵽�����࿪�����򴴽�Ψһ������
			Return
					true = �Ѷ࿪
					false	= δ�࿪
		*/
		bool NEWOBJ_API more_open(const nstring& name);
		bool NEWOBJ_API exist_mutex(const nstring& name);
		/*������PID*/
		uint32 NEWOBJ_API parent_pid(uint32 pid);
		/*�Ƿ���Ч*/
		bool NEWOBJ_API effective(uint32 pid);
	}
}
#endif