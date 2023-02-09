#pragma once
#include "public/define.h"
#include "public/string.h"
#include "public/struct.h"
#include <vector>
#ifdef _WIN32
typedef ptr ptr_menu;
typedef ptr ptr_win;

namespace newobj
{
        namespace window
        {
            /*取父句柄*/
            ptr_win NEWOBJ_API parent(ptr_win handle);
            /*取窗口类名*/
            nstring NEWOBJ_API class_name(ptr_win handle);
            /*取窗口标题*/
            nstring NEWOBJ_API title_name(ptr_win handle);
            /*置窗口标题*/
            bool NEWOBJ_API title_name(ptr_win handle,const nstring& name);
            /*
                模糊遍历窗口
                Desc：parent父窗口句柄为空则为桌面句柄
            */
            std::vector<ptr_win> NEWOBJ_API find(ptr_win parent = 0, const nstring& class_name = "", const nstring& title_name = "");
            ptr_win NEWOBJ_API find(const nstring& class_name, const nstring& title_name = "");

            ptr_win NEWOBJ_API find(const nstring& path,const nstring& class_name, const nstring& title_name = "");
            /*取所有子窗口*/
            std::vector<ptr_win> NEWOBJ_API childs(ptr_win parent = 0, const nstring& class_name = "",const nstring& title_name = "",bool childs = true);

            ptr_win NEWOBJ_API childs_signal(ptr_win parent = 0, const nstring& class_name = "", const nstring& title_name = "", bool childs = true);

            /*句柄取路径*/
            nstring NEWOBJ_API getpath(ptr_win handle);
            
            /*句柄取PID*/
            uint32 NEWOBJ_API getpid(ptr_win handle);
            /*取大小*/
            Size NEWOBJ_API size(ptr_win handle);
            /*取矩形*/
            Rect NEWOBJ_API rect(ptr_win handle);
            /*关闭*/
            void NEWOBJ_API close(ptr_win handle);
            /*取内容*/
            nstring NEWOBJ_API content(ptr_win handle);
            /*置内容*/
            void NEWOBJ_API content(ptr_win handle, const nstring& content);
            bool NEWOBJ_API content(ptr_win parent, const nstring& content,const nstring& class_name,const nstring& title_name = "");
            /*置顶*/
            bool NEWOBJ_API top(ptr_win handle);
            namespace menu
            {
                /*点击菜单*/
                bool NEWOBJ_API click(ptr handle, ptr_menu menu_child_handle,uint32 menu_item_idx);
                /*取菜单句柄*/
                ptr_menu NEWOBJ_API get(ptr_win handle);
                /*取子菜单*/
                ptr_menu NEWOBJ_API child(ptr_menu menu_handle,uint32 menu_childs_idx);
            }
            namespace checkbox
            {
                /*选中状态*/
                bool NEWOBJ_API checked(ptr_win handle);
                /*置状态*/
                bool NEWOBJ_API checked(ptr_win handle,bool checked);
            }
            namespace tab
            {
                /*现行子夹*/
                void NEWOBJ_API current(ptr_win handle,uint32 idx);
            }
            namespace button
            {
                /*按下按钮*/
                bool NEWOBJ_API click(ptr_win handle);
                bool NEWOBJ_API click(ptr_win parent, const nstring& class_name, const nstring& title_name = "");
            }
            namespace list
            {
                /*插入行*/
                void NEWOBJ_API insert(ptr_win handle,uint32 index,const nstring& value);
            }
            /*
                矩形排序
                @type 0=top 1=right 2=left 3=bottom
                @ps 正序
            */
            void NEWOBJ_API rect_sort(std::vector<ptr_win>& list,uint32 type, bool ps = true);
            /*置启动状态*/
            bool NEWOBJ_API enable(ptr win_handle, bool enable);
        }
}

#endif