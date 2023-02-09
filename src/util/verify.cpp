#include "verify.h"
namespace newobj
{
        namespace verify
        {
            /*********************************************************
            *	Function : 判断字符是否为数字
            *********************************************************/
            bool is_num(char code)
            {
                if (code < 48 || code > 57)
                    return false;
                return true;
            }
            /*********************************************************
            *	Function : 判断字符串是否为数字
            *********************************************************/
            bool is_nums(const nstring& str)
            {
                if (str.length() == 0)
                    return false;
                bool find_point = false;
                for (size_t i = 0; i < str.length(); i++)
                {
                    if (str[i] == 46 && find_point == false)
                    {
                        if (i == 0 || i == str.length() - 1)
                            return false;
                        find_point = true;
                    }
                    else if (str[i] < 48 || str[i] > 57)
                        return false;
                }
                return true;
            }
            /*********************************************************
            *	Function : 判断两个字符串是否相等
            *********************************************************/
            bool equal_en(const nstring& str1, const nstring str2)
            {
                if (str1.length() != str2.length())
                {
                    return false;
                }
                for (size_t i = 0; i < str1.length(); i++)
                {
                    //判断是否为字母
                    if (is_en(str1[i]) && is_en(str2[i]))
                    {
                        if (equal_en(str1[i], str2[i]) == false)
                        {
                            return false;
                        }
                    }
                    else
                    {

                    }
                }
                return true;
            }
            /*********************************************************
            *	Function : 判断是否为英文字符
            *********************************************************/
            bool is_en(char code)
            {
                if ((code >= 65 && code <= 90) || (code >= 97 && code <= 122))
                    return true;
                return false;
            }
            /*********************************************************
            *	Function : 判断是否两个英文字母与相等，不区分大小写
            *********************************************************/
            bool equal_en(char code1, char code2)
            {
                if (code1 + 32 == code2 || code1 - 32 == code2 || code1 == code2)
                {
                    return true;
                }
                return false;
            }
            bool is_mobile(const nstring& mobile)
            {
                if (mobile.length() != 11)
                    return false;
                if (mobile[0] != '1')
                    return false;
                return is_nums(mobile);
            }
            bool is_asc(char code)
            {
                if (code == 46 || code == 95)
                    return true;
                return false;
            }
            bool is_pwd(const nstring& password)
            {
                if (password.length() < 6 || password.length() > 20 || is_a(password) == false)
                    return false;
                return true;
            }
            bool is_a(const nstring& str)
            {
                for (unsigned int i = 0; i < str.length(); i++)
                {
                    if (is_asc(str[i]) == false && is_num(str[i]) == false && is_en(str[i]) == false)
                        return false;
                }
                return true;
            }
            bool isUint(int num)
            {
                return num >= 0;
            }
            bool isBit(int num)
            {
                return num == 0 || num == 1;
            }
        }
}
