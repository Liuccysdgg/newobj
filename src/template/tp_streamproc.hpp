#pragma once
#include "public/buffer.h"
namespace newobj
{
	/****************************************
	 * Enum��StreamProcTP����
	 ****************************************/
	enum StreamProcTpError
	{
		// У��ʧ��
		CHECK_FAILED,
		// ��תʧ��
		TO_STREAMSTRUCT_FAILED,
	};
	/******************************************************************************
	 * Class�������ݴ���ģ��
	 * Template��
	 *			Extra							��					��������
	 ******************************************************************************/
	template <typename Extra>
	class istreamproc
	{
	public:
		/******************************************************************************
		 * Param
		 *			signal_thread				��			���߳�
		 * Return
		 *			true  = �ɹ������в��
		 *			false = ʧ�ܣ����� Error() ������
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
		 * Function��У������
		 * Desc����ͨ���ú�����Դ���ݽ���У�飬���ж��Ƿ������һ������
		 * Param
		 *			data							��					ԭ����[���޸�]
		 * Return
		 *			true  = �ɹ������в��
		 *			false = ʧ�ܣ����� Error() ������
		 ******************************************************************************/
		virtual bool checkdata(newobj::buffer & data) = 0;
		/******************************************************************************
		 * Function������ص�
		 * Desc����������ʧ�ܻ���øûص�
		 * Param
		 *			data							��					ԭ����
		 *			error							��					��������
		 *			extra							��					��������
		 ******************************************************************************/
		virtual void error(const newobj::buffer& data, StreamProcTpError error,const Extra &extra) = 0;
		/******************************************************************************
		 * Function��ִ������
		 * Desc��У�鲢��ת�ɹ������øûص�
		 * Param
		 *			data							��					ԭ����
		 *			extra							��					��������
		 * Return
		 *			������������᷵�ص��÷�  "Post()"
		 ******************************************************************************/
		virtual bool exec(const newobj::buffer& data, const Extra& extra) = 0;
		/******************************************************************************
		 * Function��Ͷ������
		 * Param
		 *			data							��					����ָ��
		 *			len							��					���ݳ���
		 * Return
		 *			�������
		 ******************************************************************************/
		bool post(const char* data, int32 len,const Extra &extra)
		{
			buffer buf((uchar*)data, len);

			/*У������*/
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
		// ���߳�
		bool m_signal_thread = true;
	};
}