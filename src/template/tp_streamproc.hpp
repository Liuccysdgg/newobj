#pragma once
#include "public/buffer.h"
namespace newobj
{
	/****************************************
	 * Enum：StreamProcTP错误
	 ****************************************/
	enum StreamProcTpError
	{
		// 校验失败
		CHECK_FAILED,
		// 流转失败
		TO_STREAMSTRUCT_FAILED,
	};
	/******************************************************************************
	 * Class：流数据处理模板
	 * Template：
	 *			Extra							：					附加数据
	 ******************************************************************************/
	template <typename Extra>
	class istreamproc
	{
	public:
		/******************************************************************************
		 * Param
		 *			signal_thread				：			单线程
		 * Return
		 *			true  = 成功，进行拆包
		 *			false = 失败，触发 Error() 函数。
		 ******************************************************************************/
		istreamproc(bool signal_thread = true)
		{
			m_signal_thread = signal_thread;
			if (m_signal_thread == false)
			{
				trw_str("Multithreading is not allowed");
			}
				
		}
		~istreamproc()
		{

		}
		/******************************************************************************
		 * Function：校验数据
		 * Desc：可通过该函数对源数据进行校验，以判断是否进行下一步处理。
		 * Param
		 *			data							：					原数据[可修改]
		 * Return
		 *			true  = 成功，进行拆包
		 *			false = 失败，触发 Error() 函数。
		 ******************************************************************************/
		virtual bool checkdata(newobj::buffer & data) = 0;
		/******************************************************************************
		 * Function：错误回调
		 * Desc：检验数据失败会调用该回调
		 * Param
		 *			data							：					原数据
		 *			error							：					错误描述
		 *			extra							：					附加数据
		 ******************************************************************************/
		virtual void error(const newobj::buffer& data, StreamProcTpError error,const Extra &extra) = 0;
		/******************************************************************************
		 * Function：执行数据
		 * Desc：校验并流转成功后会调用该回调
		 * Param
		 *			data							：					原数据
		 *			extra							：					附加数据
		 * Return
		 *			处理情况，将会返回调用方  "Post()"
		 ******************************************************************************/
		virtual bool exec(const newobj::buffer& data, const Extra& extra) = 0;
		/******************************************************************************
		 * Function：投递数据
		 * Param
		 *			data							：					数据指针
		 *			len							：					数据长度
		 * Return
		 *			处理情况
		 ******************************************************************************/
		bool post(const char* data, int32 len,const Extra &extra)
		{
			buffer buf((uchar*)data, len);

			/*校验数据*/
			if (checkdata(buf) == false)
			{
				StreamProcTpError errors;
				errors = StreamProcTpError::CHECK_FAILED;
				istreamproc::error(buf,errors,extra);
				return false;
			}
			return exec(buf,extra);
		}
	private:
		// 单线程
		bool m_signal_thread = true;
	};
}