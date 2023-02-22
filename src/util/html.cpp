#include "html.h"
#include "public/buffer.h"
#include "util/file.h"
#include "util/forstring.h"

static uint32 node_new = 0;
static uint32 node_delete = 0;
newobj::html::node::node()
{
    m_root = false;
    parent = nullptr;
    offset = 0;
    length = 0;
}
newobj::html::node::~node()
{
    for (uint32 i = 0; i < childs.size(); i++)
    {
        node_delete++;
        delete childs[i];
    }
}
bool newobj::html::parser(const nstring& value, html::node& root)
{


    htmlcxx::HTML::ParserDom parser_dom;
    auto dom = parser_dom.parseTree(value);
    if (dom.size() == 0)
        return false;
    std::map<tree_node_<htmlcxx::HTML::Node>*, newobj::html::node*> node_list;
    for_iter(iter, dom)
    {
        if (iter->isTag() == false || iter->tagName() == "")
            continue;

        newobj::html::node* no = nullptr;
        // 父节点
        if (iter.node->parent == NULL || iter.node->parent->data.offset() == 0 &&  forstring::en_case(iter->tagName(),false) == "html")
        {
            root.name = iter->tagName();
            node_list.insert(std::pair<tree_node_<htmlcxx::HTML::Node>*, newobj::html::node*>(iter.node,&root));
            
            no = &root;
            no->m_root = true;
            no->offset = iter.node->data.offset();
            no->length = iter.node->data.length();
        }
        else
        {
            
            auto node = node_list.find(iter.node->parent);
            if (node == node_list.end())
                continue;
            no = new newobj::html::node;
            node_new++;
            no->parent = node->second;
            no->name = iter->tagName();
            no->offset = iter.node->data.offset();
            no->length = iter.node->data.length();

            node->second->childs.push_back(no);
            root.all.push_back(no);

            node_list.insert(std::pair<tree_node_<htmlcxx::HTML::Node>*, newobj::html::node*>(iter.node, no));
        }
        
        // 属性
        iter->parseAttributes();
        iter->attributes();
        std::map<std::string, std::string> atrr_map = iter->attributes();
        for_iter(iter_attr, atrr_map)
        {
            no->attrs.insert(std::pair<nstring, nstring>(iter_attr->first.c_str(), iter_attr->second.c_str()));
        }
        //其它
        //no->m_treenode = iter.node;
    }

    for_iter(iter, node_list)
    {
        // 内容
        if (iter->second->childs.size() == 0)
        {
            uint32 start = iter->first->data.offset() + iter->first->data.text().length();
            uint32 len = iter->first->data.length() - iter->first->data.closingText().length() - iter->first->data.text().length();
            if (start + len > value.length())
                continue;
            iter->second->content = value.substr(start, len);
            continue;
        }
    }
    return true;
}
