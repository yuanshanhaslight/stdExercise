#ifndef STL_LIST_H_INCLUDED
#define STL_LIST_H_INCLUDED
#include "Memory.h"
namespace stdExercise
{

template<typename T>
struct __list_node                                             //list 的节点定义
{
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template<typename T, typename Ref, typename Ptr>
struct __list_iterator                                        //list 的迭代器定义
{
    typedef __list_iterator<T, T&, T*>      iterator;
    typedef __list_iterator<T, T&, T*>      self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T                             value_type;
    typedef Ptr                           pointer;
    typedef Ref                           reference;
    typedef __list_node<T>*               link_type;
    typedef size_t                       size_type;
    typedef ptrdiff_t                    difference_type;

    link_type node;

    __list_iterator(link_type x):node(x){}                      //构造函数定义
    __list_iterator(){}
    __list_iterator(const iterator&x):node(x.node){}

    bool operator==(const self&x) const {return node==x.node;}
    bool operator!=(const self&x) const {return node!=x.node;}
    reference operator*()const {return (*node).data;}
    pointer operator->()const {return &(operator*());}

    self&operator++()
    {
        node=static_cast<link_type>((*node).next);
        return *this;
    }
    self operator++(int)
    {
        self tmp=*this;
        ++*this;
        return tmp;
    }

    self& operator--()
    {
        node = static_cast<link_type>( (*node).prev );
        return *this;
    }
    self operator--(int)
    {
        self tmp=*this;
        --*this;
        return tmp;
    }
};//end of list iterator

template<typename T, typename Ref, typename Ptr>
inline
bool operator== (const __list_iterator<T,Ref,Ptr>&ite,nullptr_t p )
{
    return ite.node==p;
}
template<typename T, typename Ref, typename Ptr>
inline
bool operator== (nullptr_t p,const __list_iterator<T,Ref,Ptr>&ite)
{
    return ite.node==p;
}
template<typename T, typename Ref, typename Ptr>
inline
bool operator!= (const __list_iterator<T,Ref,Ptr>&ite,nullptr_t p )
{
    return ite.node!=p;
}
template<typename T, typename Ref, typename Ptr>
inline
bool operator!= (nullptr_t p, const __list_iterator<T,Ref,Ptr>&ite )
{
    return ite.node!=p;
}

///........................................list.................///

template<typename T, typename Alloc=alloc>
class list
{
protected:
    typedef __list_node<T> list_node;
public:
    typedef list_node*                  link_type;
    typedef __list_iterator<T,T&,T*>    iterator;
    typedef size_t                      size_type;
    typedef T&                          reference;
    typedef T*                          pointer;

protected:
    link_type node;                                               //指向哨兵节点的指针
    typedef simple_alloc<list_node, Alloc> list_node_allocator;   //空间配置器
protected:
    link_type get_node()                                          //分配一个节点的空间
    {
        return list_node_allocator::allocate();
    }
    void put_node(link_type p)                                    //回收一个节点的空间
    {
        list_node_allocator::deallocate(p);
    }
    link_type create_node(const T&x)                              //创建一个节点
    {
        link_type p = get_node();
        construct(&p->data,x);
        return p;
    }
    void destroy_node(link_type p)                                 //销毁一个节点
    {
        destroy(&p->data);
        put_node(p);
    }
    void empty_initialize()                                        //空链表初始化
    {
        node=get_node();
        node->next=node;
        node->prev=node;
    }
    void transfer(iterator position, iterator first, iterator last) //将first last所指定的一段链表
    {                                                                 //移到position之前
        if(position==last)return ;
        (*(static_cast<link_type>((*last.node).prev))).next=position.node;
        (*(static_cast<link_type>((*first.node).prev))).next=last.node;
        (*(static_cast<link_type>((*position.node).prev))).next=first.node;
        link_type tmp = static_cast<link_type>((*position.node).prev);
        (*position.node).prev=(*last.node).prev;
        (*last.node).prev=(*first.node).prev;
        (*first.node).prev=tmp;
    }
public:                                                             //拷贝控制操作
    list(){empty_initialize();}                                     //默认构造函数
    list(const list& lst);                                         //拷贝构造函数
    list& operator=(const list &rhs);                              //拷贝赋值运算符
    ~list();                                                        //析构函数

    list(list&&lst)noexcept;                                       //移动构造函数
    list& operator=(list&&rhs)noexcept;                          //移动复制运算符

                                                                    //其他操作
    template<typename InputIterator>
    list(InputIterator first,InputIterator last)
    {
        empty_initialize();
        for(;first!=last;++first)
            insert(end(),*first);
    }
public:
    iterator begin()
    {
        return static_cast<link_type>((*node).next);
    }
    iterator end(){return node;}
    iterator begin()const
    {
        return static_cast<link_type>((*node).next);
    }
    iterator end()const {return node;}

    bool empty() const
    {
        return node->next==node;
    }
    size_type size() const
    {
        size_type result = 0;
        distance(begin(),end(),result);
        return result;
    }
    reference front(){return *begin();}
    reference back(){return *--end();}
    iterator insert(iterator position, const T& x)
    {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        static_cast<link_type>(position.node->prev)->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }
    void push_back(const T&x)
    {
        insert(end(),x);
    }
    void push_front(const T&x)
    {
        insert(begin(),x);
    }
    iterator erase(iterator position)
    {
        link_type next_node=static_cast<link_type>(position.node->next);
        link_type prev_node=static_cast<link_type>(position.node->prev);
        next_node->prev=prev_node;
        prev_node->next=next_node;
        destroy_node(position.node);
        return static_cast<iterator>(next_node);
    }
    void pop_front()
    {
        erase(begin());
    }
    void pop_back()
    {
        erase(--(end()));
    }
    void clear()
    {
        link_type curr=static_cast<link_type>(node->next);
        while(curr!=node)
        {
            link_type tmp=curr;
            curr=static_cast<link_type>(curr->next);
            destroy_node(tmp);
        }
        node->next=node;
        node->prev=node;
    }

    void swap(list&lst)
    {
        link_type tmp=node;
        node=lst.node;
        lst.node=tmp;
    }
    void remove(const T&x)
    {
        iterator first=begin();
        iterator last=end();
        while(first!=last)
        {
            if(*first==x)
            {
                iterator tmp=first;
                ++first;
                erase(tmp);
            }
            else
                ++first;
        }
    }
    void unique()
    {
        iterator first=begin();
        iterator last=end();
        if(first==last) return ;
        iterator prev=first++;
        while (first!=last)
        {
            if(*prev==*first)
            {
                erase(prev);
            }
            prev=first;
            ++first;
        }
    }
    void splice(iterator position, list&x)
    {
        if(x.begin()!=x.end())
            transfer(position, x.begin(), x.end());
    }
    void splice(iterator position, list&, iterator i)
    {
        iterator j=i;
        ++j;
        if(j!=position && i!=position )
            transfer(position, i, j);
    }
    void splice(iterator position, list&, iterator first, iterator last)
    {
        if(first!= last)
            transfer(position, first, last);
    }
    void merge(list&x);
    void reverse();
    void sort();
};///end of  list

template<typename T,typename Alloc>
list<T,Alloc>::list(const list&src)
{
    empty_initialize();
    for(iterator ite=src.begin();ite!=src.end();++ite)
        insert(end(),*ite);
}

template<typename T,typename Alloc>
list<T,Alloc>& list<T,Alloc>::operator=(const list&rhs)
{
    if(this!=&rhs)
    {
        list tmp(rhs);
        swap(tmp);
    }
    return *this;
}

template<typename T,typename Alloc>
list<T,Alloc>::~list()
{
    clear();
    put_node(node);
}

template<typename T,typename Alloc>
list<T,Alloc>::list(list&&src) noexcept
{
    empty_initialize();
    swap(src);
}

template<typename T,typename Alloc>
list<T,Alloc>& list<T,Alloc>::operator=(list&&rhs) noexcept
{
    if(this!=&rhs)
    {
        clear();
        swap(rhs);
    }
    return *this;
}

template<typename T, typename Alloc>
void list<T,Alloc>::merge(list<T,Alloc>&x)
{
    iterator first1= begin();
    iterator last1=end();
    iterator first2=x.begin();
    iterator last2=x.end();
    while(first2!=last2&&first1!=last1)
    {
        if(*first1<*first2)
        {
            ++first1;
        }
        else
        {
            iterator tmp=first2++;
            transfer(first1,tmp,first2);
        }
    }
    if(first2!=last2) transfer(last1,first2,last2);
}

template<typename T, typename Alloc>
void list<T,Alloc>::reverse()
{
    if(node->next==node||
       static_cast<link_type>(node->next)->next==node) return ;
    iterator first = begin();
    iterator last = end();
    ++first;
    while(first!= last)
    {
        iterator old= first;
        ++first;
        transfer(begin(),old, first);
    }
}

//归并排序，还没有理解
template<typename T, typename Alloc>
void list<T,Alloc>::sort()
{

    if(node->next==node||static_cast<link_type>(node->next)->next==node)
        return ;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill=0;
    while(!empty())
    {
        carry.splice(carry.begin(),*this,begin());
        int i=0;
        while(i<fill && !counter[i].empty())
        {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if(i==fill) ++fill;
    }
    for(int i=1;i<fill;++i)
        counter[i].merge(counter[i-1]);
    swap(counter[fill-1]);
}
}//end of namespace stdExercise
#endif // STL_LIST_H_INCLUDED
