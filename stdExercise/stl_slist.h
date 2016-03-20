//还缺少clear(),insert(),erase(),splice(),insert_after(),erase_after().....
//思考：slist的迭代器为什么使用类的继承来实现，而list的迭代器没有？
#ifndef STL_SLIST_H_INCLUDED
#define STL_SLIST_H_INCLUDED

#include "Memory.h"
#include "stl_iterator.h"

namespace stdExercise
{

struct __slist_node_base                                            //slist的节点的父类定义
{
    __slist_node_base*next;
};

template<typename T>
struct __slist_node : public __slist_node_base                      //slist的节点定义
{
    T data;
};

inline __slist_node_base* __slist_make_link(
                           __slist_node_base*prev_node,
                           __slist_node_base*new_node)
{
    new_node->next=prev_node->next;
    prev_node->next=new_node;
    return new_node;
}
inline size_t __slist_size(__slist_node_base*node)
{
    size_t result = 0;
    for(;node!=0;node=node->next)
        ++result;
    return result;
}
struct __slist_iterator_base                                         //slist的迭代器的父类定义
{
    typedef size_t      size_type;
    typedef ptrdiff_t    difference_type;
    typedef forward_iterator_tag iterator_category;

    __slist_node_base* node;                                        //迭代器中指向节点的指针

    __slist_iterator_base(__slist_node_base*x):node(x){}

    void incr() {node=node->next;}
    bool operator==(const __slist_iterator_base&x)const
    {
        return node==x.node;
    }
    bool operator != (const __slist_iterator_base&x)const
    {
        return node!=x.node;
    }
};//end of struct __slist_iterator_base

template<typename T, typename Ref, typename Ptr>
struct __slist_iterator : public __slist_iterator_base                //slist的迭代器定义
{
    typedef __slist_iterator<T,T&,T*> iterator;
    typedef __slist_iterator<T,const T&,const T*> const_iterator;
    typedef __slist_iterator<T,Ref,Ptr> self;

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __slist_node<T> list_node;

    __slist_iterator(list_node* x):__slist_iterator_base(x){}
    __slist_iterator():__slist_iterator_base(0){}
    __slist_iterator(const iterator&x):__slist_iterator_base(x.node){}

    reference operator*()const
    {
        return static_cast<list_node*>(node)->data;
    }
    pointer operator->()const
    {
        return &(operator*());
    }

    self& operator++()
    {
        incr();
        return *this;
    }
    self operator++(int)
    {
        self tmp=*this;
        incr();
        return tmp;
    }
};//end of struct __slist_iterator

template<typename T,typename Alloc = alloc>
class slist                                                            //slist的定义
{
public:                                                                //类型别名定义
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t            difference_type;

    typedef __slist_iterator<value_type,reference,pointer> iterator;
    typedef __slist_iterator<
        value_type,
        const_reference,
        const_pointer> const_iterator;

private:
    typedef __slist_node<T> list_node;
    typedef __slist_node_base list_node_base;
    typedef __slist_iterator_base iterator_base;
    typedef simple_alloc<list_node,Alloc> list_node_allocator;

    static list_node* create_node(const value_type&x);//创建一个节点
    static void destroy_node(list_node*node);          //销毁节点
private:
    list_node_base head;                               //slist的头节点

public:                                               //拷贝控制操作
    slist();
    slist(const slist&slst);
    slist(slist&&slst)noexcept;
    slist& operator=(slist&&rhs)noexcept;
    slist& operator=(const slist&rhs);
    ~slist();
public:
    iterator begin()
    {
        return iterator(static_cast<list_node*>(head.next));
    }
    iterator end()
    {
        return iterator(0);
    }
    size_type size()const {return __slist_size(head.next);}
    bool empty()const {return head.next==0;}

public:
    void swap(slist&sl)
    {
        list_node_base *tmp=head.next;
        head.next=sl.head.next;
        sl.head.next=tmp;
    }
    reference front()
    {
        return static_cast<list_node*>(head.next)->data;
    }
    void push_front(const value_type&x)
    {
        __slist_make_link(&head,create_node(x));
    }

    void pop_front()
    {
        list_node*node=static_cast<list_node*>(head.next);
        head.next=node->next;
        destroy_node(node);
    }

};//end of class slist

template<typename T,typename Alloc>
typename slist<T,Alloc>::list_node*
slist<T,Alloc>::create_node(const value_type&x)    //创建一个节点
{
    list_node*node=list_node_allocator::allocate();
    try
    {
        construct(&node->data,x);
    }
    catch(...)
    {
        list_node_allocator::deallocate(node);
        throw;
    }
    node->next=0;
    return node;
}
template<typename T,typename Alloc>
void slist<T,Alloc>::destroy_node(list_node*node)          //销毁节点
{
    destroy(&node->data);
    list_node_allocator::deallocate(node);
}

template<typename T,typename Alloc>
slist<T,Alloc>::slist()
{
    head.next=nullptr;
}

template<typename T,typename Alloc>
slist<T,Alloc>::slist(const slist&slst)
{
    list_node_base* tail=&head;
    try
    {
        for(list_node_base *cur=slst.head.next;nullptr!=cur;cur=cur->next)
        {
            tail->next=create_node(static_cast<list_node*>(cur)->data);
            tail=tail->next;
        }
    }
    catch(...)
    {
        if(tail!=&head)
        {
            for(list_node_base*cur=head.next;cur!=tail;cur=cur->next)
            {
                destroy_node(static_cast<list_node*>(cur));
            }
            destroy_node(static_cast<list_node*>(tail));
        }
    }

}

template<typename T,typename Alloc>
slist<T,Alloc>::slist(slist&&slst)noexcept
{
    head.next=nullptr;
    swap(slst);
}

template<typename T,typename Alloc>
slist<T,Alloc>&
slist<T,Alloc>::operator=(const slist&rhs)
{
    if(this!=&rhs)
    {
        slist tmp(rhs);
        swap(tmp);
    }
    return *this;
}

template<typename T,typename Alloc>
slist<T,Alloc>&
slist<T,Alloc>::operator=(slist&&rhs)noexcept
{
    if(this!=&rhs)
    {
        slist tmp;
        tmp.swap(rhs);
        swap(tmp);
    }
    return *this;
}

template<typename T,typename Alloc>
slist<T,Alloc>::~slist()
{
    for(list_node_base*cur=head.next;cur!=nullptr;cur=cur->next)
        destroy_node(static_cast<list_node*>(cur));
}
}//end of namespace stdExercise

#endif // STL_SLIST_H_INCLUDED
