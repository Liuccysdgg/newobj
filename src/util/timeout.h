#pragma once
#include <string>
#include "public/define.h"
#include "thread.h"
#include "idx_maker.h"
#include<map>
#include <list>
#include "public/define.h"
#include "public/nobject.h"
#include <array>
#include "public/string.h"
#include "lock.h"
namespace newobj
{
	enum TimeOutRet
	{
		TOR_CONTINUE,       //�������й��ڣ�������ʼʱ��
		TOR_CLOSE               //����
	};
	//���ڼ�Ҫ
	struct TimeOutBrief
	{
		TimeOutBrief() {
			timeout_go = 0;
			id = 0;
		}
		timestamp timeout_go;   //����ʱ����ڸ�ʱ������˳�
		uint32 id;
	};
	//�¼�����
	struct TimeOutCont
	{
		TimeOutCont()
		{
			create = 0;
			update = 0;
			callback_ = NULL;
			extra_data = 0;
			thread_callback = false;
		}
		timestamp create;
		timestamp update;
		std::function<TimeOutRet(timestamp create/*����ʱ��*/, uint32& timeout_msec/*����ʱ��[����]*/, nstring id/*��ʱ�¼�ID*/, ptr extra_data/*��������*/)>  callback_;
		ptr extra_data;
		uint32 timeout_msec;
		bool thread_callback;
	};
	/****************************************************
	* Class�����ڹ�����
	****************************************************/
	class  NEWOBJ_API timeout :private ithread
	{
	public:
		timeout(const nstring& name = "");
		~timeout();
		/*********************************************************************************
		 * Fun�����ӹ����¼�
		 * Param��
		 *          timeout_callback                 :           ���ڻص�����
		 *          timeout                               ��         �����¼�[����]
		 *          extra_data                           ��         �������ݣ�һ���������֡�
		 *          lock                                     ��         �Ƿ����
		 *          thread_callback                   ��         �߳��ڻص�
		 * Return:
		 *          nstring �������¼�ID
		 *********************************************************************************/
		nstring add(std::function<TimeOutRet(timestamp create, uint32& timeout_msec, nstring id, ptr extra_data)> timeout_callback, uint32 timeout, ptr extra_data = 0, bool lock = true, bool thread_callback = false);
		/*********************************************************************************
		 * Fun�����ӹ����¼�
		 * Param��
		 *          timeoutID                           ��         ָ����timeouID
		 *          timeout_callback                 :           ���ڻص�����
		 *          timeout                               ��         �����¼�[����]
		 *          extra_data                           ��         �������ݣ�һ���������֡�
		 *          lock                                     ��         �Ƿ����
		 *          thread_callback                   ��         �߳��ڻص�
		 *********************************************************************************/
		bool add(nstring& timeoutID, std::function<TimeOutRet(timestamp create, uint32& timeout_msec, nstring id, ptr extra_data)> timeout_callback, uint32 timeout_msec, ptr extra_data = 0, bool lock = true, bool thread_callback = false);
		/*********************************************************************************
		 * Fun��ɾ�������¼�
		 * Param��
		 *          id                                       :           �¼�ID
		 * Return��
		 *      true = ���ڲ���ɾ��
		 *      false = �����ڻ��ѹ���
		 *********************************************************************************/
		bool del(const nstring& timeoutID);
		/*********************************************************************************
		 * Fun���Ƿ���ڹ����¼�
		 * Param��
		 *          id                                       :           �¼�ID
		 *********************************************************************************/
		bool exist(const nstring& timeoutID, bool lock = true);
		/*********************************************************************************
		 * Fun�����¹���
		 * Param��
		 *          id                                       :           �¼�ID
	   *********************************************************************************/
		bool update(const nstring& timeoutID, bool lock = true);
		/*********************************************************************************
		 * Fun���޸Ĺ���ʱ��
		 * Param��
		 *          id                                       :           �¼�ID
		 *			timeout_msec					  ��		 ����ʱ��
	   *********************************************************************************/
		bool set_timeout(const nstring& timeoutID,uint32 timeout_sec, bool lock = true);
		/*********************************************************************************
		 * Fun����ȡ�����¼�
		 * Param��
		 *          id                                                  :           �¼�ID
		 *          [��ַ]timeout_callback                 :           ���ڻص�����
		 *          [��ַ]timeout_msec                     ��         �����¼�[����]
		 *          [��ַ]extra_data                           ��         �������ݣ�һ���������֡�
		 *          lock                                              ��         �Ƿ����
		 *********************************************************************************/
		bool get(const nstring& timeoutID, timestamp& create/*����ʱ��*/, uint32& timeout_msec/*����ʱ��[����]*/, ptr& extra_data/*��������*/, bool lock = true);
		void clear();
		bool out(const nstring& timeoutID, bool curr_thread = false, bool lock = true);
		bool set_extra(const nstring& timeoutID, ptr extra, bool lock = true);
		bool get_extra(const nstring& timeoutID, ptr& extra, bool lock = true);
		uint32 size();
		/*������ID*/
		nstring create_id();
	private:
		// ͨ�� Thread �̳�
		virtual bool run() override;
	private:
		idx_maker m_indexMaker;            //�¼�ID������
		nstring m_name;                                 //����
		std::map<nstring, TimeOutCont*> m_events;
	public:
		newobj::mutex m_mutex;
	};
	/****************************************************
	 * Class����ʱ���ӿ�
	 ****************************************************/
	class NEWOBJ_API itimer :private ithread
	{
	public:
		itimer();
		~itimer();
		/*******************************************************************
		 * Function������
		 * Param
		 *          msec                    ��           ����
		 *******************************************************************/
		void start(uint32 msec);
		/*******************************************************************
		 * Function��ֹͣ
		 *******************************************************************/
		void stop();
	protected:
		virtual bool on_timer() = 0;
	private:
		// �ӳ�ʱ��
		uint32 m_msec;
		// ��־��0=��״̬ 1=������ 2=�ر��ź�
		int32 m_idx;
	private:
		// ͨ�� Thread �̳�
		virtual bool run() override;

	};

	typedef bool(nobject::* CALLBACK_TIMER)();
	/****************************************************
	 * Class����ʱ��
	 ****************************************************/
	class NEWOBJ_API timer :public itimer
	{
	public:
		timer();
		~timer();
		void on_callback(nobject* recver, CALLBACK_TIMER pfun_timer);
	private:
		CALLBACK_TIMER m_pfun ;
		nobject* m_recver ;
	private:
		// ͨ�� ITimer �̳�
		virtual bool on_timer() override;

	};
}