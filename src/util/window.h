#pragma once
#include "public/define.h"
#include "basetype/string.h"
#include "public/struct.h"
#include <vector>
#ifdef _WIN32
typedef ptr ptr_menu;
typedef ptr ptr_win;

namespace newobj
{
        namespace window
        {
            /*ȡ�����*/
            ptr_win NEWOBJ_API parent(ptr_win handle);
            /*ȡ��������*/
            nstring NEWOBJ_API class_name(ptr_win handle);
            /*ȡ���ڱ���*/
            nstring NEWOBJ_API title_name(ptr_win handle);
            /*�ô��ڱ���*/
            bool NEWOBJ_API title_name(ptr_win handle,const nstring& name);
            /*
                ģ����������
                Desc��parent�����ھ��Ϊ����Ϊ������
            */
            std::vector<ptr_win> NEWOBJ_API find(ptr_win parent = 0, const nstring& class_name = "", const nstring& title_name = "");
            ptr_win NEWOBJ_API find(const nstring& class_name, const nstring& title_name = "");

            ptr_win NEWOBJ_API find(const nstring& path,const nstring& class_name, const nstring& title_name = "");
            /*ȡ�����Ӵ���*/
            std::vector<ptr_win> NEWOBJ_API childs(ptr_win parent = 0, const nstring& class_name = "",const nstring& title_name = "",bool childs = true);

            ptr_win NEWOBJ_API childs_signal(ptr_win parent = 0, const nstring& class_name = "", const nstring& title_name = "", bool childs = true);

            /*���ȡ·��*/
            nstring NEWOBJ_API getpath(ptr_win handle);
            
            /*���ȡPID*/
            uint32 NEWOBJ_API getpid(ptr_win handle);
            /*ȡ��С*/
            Size NEWOBJ_API size(ptr_win handle);
            /*ȡ����*/
            Rect NEWOBJ_API rect(ptr_win handle);
            /*�ر�*/
            void NEWOBJ_API close(ptr_win handle);
            /*ȡ����*/
            nstring NEWOBJ_API content(ptr_win handle);
            /*������*/
            void NEWOBJ_API content(ptr_win handle, const nstring& content);
            bool NEWOBJ_API content(ptr_win parent, const nstring& content,const nstring& class_name,const nstring& title_name = "");
            /*�ö�*/
            bool NEWOBJ_API top(ptr_win handle);
            namespace menu
            {
                /*����˵�*/
                bool NEWOBJ_API click(ptr handle, ptr_menu menu_child_handle,uint32 menu_item_idx);
                /*ȡ�˵����*/
                ptr_menu NEWOBJ_API get(ptr_win handle);
                /*ȡ�Ӳ˵�*/
                ptr_menu NEWOBJ_API child(ptr_menu menu_handle,uint32 menu_childs_idx);
            }
            namespace checkbox
            {
                /*ѡ��״̬*/
                bool NEWOBJ_API checked(ptr_win handle);
                /*��״̬*/
                bool NEWOBJ_API checked(ptr_win handle,bool checked);
            }
            namespace tab
            {
                /*�����Ӽ�*/
                void NEWOBJ_API current(ptr_win handle,uint32 idx);
            }
            namespace button
            {
                /*���°�ť*/
                bool NEWOBJ_API click(ptr_win handle);
                bool NEWOBJ_API click(ptr_win parent, const nstring& class_name, const nstring& title_name = "");
            }
            namespace list
            {
                /*������*/
                void NEWOBJ_API insert(ptr_win handle,uint32 index,const nstring& value);
            }
            /*
                ��������
                @type 0=top 1=right 2=left 3=bottom
                @ps ����
            */
            void NEWOBJ_API rect_sort(std::vector<ptr_win>& list,uint32 type, bool ps = true);
            /*������״̬*/
            bool NEWOBJ_API enable(ptr win_handle, bool enable);
        }
}

#endif