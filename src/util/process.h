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
		/*进程守护*/ 
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
				新增守护进程信息[自动启动进程]
				@path				：				进程路径
				@extra				：				附加数据
				@callback		：				进程异常结束回调
			*/
			bool add(const nstring& path, void* extra, std::function<bool(const process_info& proc, void* extra)> callback);
			/*移除守护进程信息*/
			void* remove(const nstring& path);
		private:
			std::vector<process_info> m_map;

			// 通过 thread 继承
			virtual bool run() override;
		};
	}
	namespace process
	{
		/**
		* @brief 通过进程ID区进程路径
		* @return
		*/
		//nstring NEWOBJ_API getpath(uint32 pid);
		nstring NEWOBJ_API getpath(uint32 pid);
		/*进程数据*/
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
			启动应用
			@param
					rundir						：						应用启动目录，仅当wait=true时有效
					wait							:						等待应用结束
			@return
					==0		：成功
					  !=0		：错误码
		*/
		int32 NEWOBJ_API exec(const nstring& path, const nstring& rundir,bool wait = false,bool show = true);
		/**
				* @brief 取系统进程列表
				* @return 
				*/
		 void NEWOBJ_API list(std::list<ProcData> & value);

		/**
			* @brief 是否存在指定进程路径
			* @param procpath
			* @return
			*/
		bool NEWOBJ_API exist(const nstring& procpath);
		/*通过进程路径取PID*/
		uint32 NEWOBJ_API getpid(const nstring& procpath);
		/*Kill进程*/
		bool NEWOBJ_API kill(uint32 pid);
		/*本进程ID*/
		uint32 NEWOBJ_API current_pid();
		/*
			检测多开
			Desc：使用该函数检测防止多开，如未检测到其它多开进程则创建唯一条件。
			Return
					true = 已多开
					false	= 未多开
		*/
		bool NEWOBJ_API more_open(const nstring& name);
		bool NEWOBJ_API exist_mutex(const nstring& name);
		/*父进程PID*/
		uint32 NEWOBJ_API parent_pid(uint32 pid);
		/*是否有效*/
		bool NEWOBJ_API effective(uint32 pid);
	}
}
#endif