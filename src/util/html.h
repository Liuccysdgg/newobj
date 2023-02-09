#pragma once

#include <map>
#include "public/string.h"
#include "sdk/text/htmlcxx/ParserDom.h"
namespace newobj
{
    namespace html
    {
        class node
        {
        public:
            node();
            ~node();
        public:
            std::map<nstring, nstring> attrs;
            std::vector<newobj::html::node*> childs;
            newobj::html::node* parent;
            nstring name;
            nstring content;
            bool m_root;
            std::vector<newobj::html::node*> all;
            uint32 offset;
            uint32 length;
        };
        /*½âÎö*/
        bool parser(const nstring& data,html::node & root);
    }
}