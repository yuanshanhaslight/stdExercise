#ifndef STL_DEQUE_H_INCLUDED
#define STL_DEQUE_H_INCLUDED
#include "Memory.h"
#include "Algorithm.h"
namespace stdExercise
{
///缓冲区大小，按元素个数统计
inline size_t __deque_buf_size(size_t n, size_t sz)
{
    return n != 0 ? n : ( sz < 512 ? ( 512 / sz ) : 1 );
}

template<typename T, typename Ref, typename Ptr, size_t BufSiz>
struct __deque_iterator                                                      //deque的迭代器
{
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
    typedef __deque_iterator<T,const T&,const T*,BufSiz> const_iterator;
    static size_t buffer_size()                                             //计算一个连续缓冲区的大小
    {
        return __deque_buf_size(BufSiz,sizeof(T));
    }

    typedef random_access_iterator_tag iterator_category;                 //deque的迭代器属于随机存取迭代器
    typedef T       value_type;
    typedef Ptr     pointer;
    typedef Ref     reference;
    typedef size_t  size_type;
    typedef ptrdiff_t difference_type;
    typedef T**     map_pointer;

    typedef __deque_iterator self;

    T* cur;                                       //迭代器所指元素的位置
    T* first;                                     //该元素所在缓冲区的首地址
    T* last;                                      //该元素所在内攒片段的尾地址
    map_pointer node;                             //node所指的元素值为该缓冲区的地址

    void set_node(map_pointer new_node)          //改变迭代器所指的缓冲区
    {
        node = new_node;
        first= *new_node;
        last=first + static_cast<difference_type>(buffer_size());
    }
    reference operator*(){return *cur;}
    pointer operator->(){return &(operator*());}
    difference_type operator-(const self&x)const                              //重载减法运算
    {
        return static_cast<difference_type>(buffer_size())*(node-x.node-1)+
            (cur-first)+(x.last-x.cur);
    }
    self& operator++()                                                         //重载前置递增运算符
    {
        ++cur;
        if(cur==last)
        {
            set_node(node+1);
            cur=first;
        }
        return *this;
    }
    self operator++(int)                                                        //重载后置递增运算符
    {
        self tmp=*this;
        ++*this;
        return tmp;
    }
    self& operator--()                                                          //重载前置递减运算符
    {
        if(cur==first)
        {
            set_node(node-1);
            cur=last;
        }
        else
            --cur;
        return *this;
    }
    self operator--(int)                                                      //重载后置递减运算符
    {
        self tmp=*this;
        --*this;
        return tmp;
    }
    self& operator+=(difference_type n)                                     //重载+=复合运算符
    {
        difference_type offset=n + (cur-first);
        if(offset>=0 && offset < static_cast<difference_type>(buffer_size()))
            cur+=n;
        else
        {
            difference_type node_offset=
                offset>0? offset/static_cast<difference_type>(buffer_size()):
                          -static_cast<difference_type>((-offset-1)/buffer_size())-1;
            set_node(node+node_offset);
            cur=first+(offset-node_offset*static_cast<difference_type>(buffer_size()));
        }
        return *this;
    }
    self operator+(difference_type n) const                              //重载加法运算符
    {
        self tmp=*this;
        return tmp+=n;
    }
    self& operator-=(difference_type n)                                  // 重载-=复合运算符
    {
        return *this+=-n;
    }
    self operator-(difference_type n)                                    //重载减法运算符
    {
        self tmp=*this;
        return tmp-=n;
    }
    reference operator[](difference_type n)const                       //重载下标访问运算符
    {
        return *(*this+n);
    }
    bool operator==(const self&x)const
    {
        return cur==x.cur;
    }
    bool operator!=(const self&x)const
    {
        return !(*this==x);
    }
    bool operator<(const self&x)const
    {
        return (node==x.node)
        ? cur<x.cur : node<x.node;
    }
};/// end of __deque_iterator


template<typename T, typename Alloc=alloc, size_t BufSiz=0>
class deque
{
public:                                                             //内置内型定义
    typedef T                       value_type;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef size_t                  size_type;
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
    typedef typename iterator::difference_type difference_type;
protected:
    typedef pointer*                        map_pointer;
    typedef simple_alloc<value_type,Alloc>  data_allocator;        //缓冲区空间配置器
    typedef simple_alloc<pointer,Alloc>     map_allocator;         //map空间配置器
protected:
    map_pointer                     map;                           //指向内存控制中枢map，
    size_type                       map_size;                     //map所能管理的缓冲区数量
    iterator                        start;                        //deque的首迭代器
    iterator                        finish;                       //deque的尾迭代器
protected:
    void fill_initialize(size_type n, const value_type& value);
    void create_map_and_nodes(size_type n);
    size_type initial_map_size(){return 8;}
    pointer allocate_node()                                       //分配一个缓冲区
    {
        return data_allocator::allocate(iterator::buffer_size());
    }
    void deallocate_node(pointer node)                           //回收缓冲区
    {
        data_allocator::deallocate(node,iterator::buffer_size());
    }
    void reserve_map_at_back(size_type);
    void push_back_aux(const value_type& value);
    void push_front_aux(const value_type& value);
    void reserve_map_at_front(size_type);
    void reallocate_map(size_type nodes_to_add, bool add_at_front);
    void pop_back_aux();
    void pop_front_aux();
    iterator insert_aux(iterator position, const value_type&value);
public:                                                               //拷贝控制操作
    deque();
    deque(const deque&deq);
    deque& operator=(const deque&deq);
    deque(deque&&deq)noexcept;
    deque& operator=(deque&&deq)noexcept;
    ~deque();

    template<typename InputIterator>
    deque(InputIterator first,InputIterator last);
    deque(int n, const value_type&value)
        :map(0),map_size(0),start(),finish()
    {
        fill_initialize(n,value);
    }
public:
    iterator begin(){return start;}
    iterator end(){return finish;}
    iterator begin()const {return start;}
    iterator end()const {return finish;}
    reference operator[](size_type n)
    {
        return start[static_cast<typename iterator::difference_type>(n)];
    }
    reference operator[](size_type n) const
    {
        return start[static_cast<typename iterator::difference_type>(n)];
    }
    reference front(){return *start;}
    reference back()
    {
        iterator tmp=finish;
        return *--tmp;
    }
    size_type size()const {return finish-start;}
    size_type max_size()const {return static_cast<size_type>(-1);}
    bool empty()const {return finish==start;}

    void push_back(const value_type&value)
    {
        if(finish.cur != finish.last-1)
        {
            construct(finish.cur,value);
            ++finish.cur;
        }
        else
        {
            push_back_aux(value);
        }
    }

    void push_front(const value_type& value)
    {
        if(start.cur!=start.first)
        {
            construct(start.cur-1,value);
            --start;
        }
        else
            push_front_aux(value);
    }
    void pop_back();
    void pop_front();
    void clear();
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator position , const value_type&value);
    void swap(deque&deq);
};/// end of container deque

template<typename T,typename Alloc, size_t BufSiz>
void deque<T,Alloc,BufSiz>::fill_initialize(size_type n, const value_type& value)
{
    create_map_and_nodes(n);
    map_pointer cur;
    //此处应有异常处理commit or rollback
    for(cur=start.node; cur<finish.node;++cur)
    {
        uninitialized_fill(*cur,*cur+iterator::buffer_size(),value);
        uninitialized_fill(finish.first,finish.cur,value);
    }
}
template<typename T, typename Alloc, size_t BuffSiz>
void deque<T,Alloc,BuffSiz>::create_map_and_nodes(size_type num_elements)
{
    //所需节点数
    size_type num_nodes=num_elements/iterator::buffer_size() +1;

    //实际节点数
    map_size = max(initial_map_size(), num_nodes+2);
    map = map_allocator::allocate(map_size);

    map_pointer nstart=map + (map_size - num_nodes)/2;
    map_pointer nfinish=nstart+num_nodes - 1;

    map_pointer cur;
    //此处应有异常处理 commit or rollback
    for(cur=nstart;cur<=nfinish;++cur)
    {
        *cur=allocate_node();
    }

    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur=start.first;
    finish.cur=finish.first+num_elements%iterator::buffer_size();
}

template<typename T, typename Alloc, size_t BuffSiz>
void deque<T,Alloc,BuffSiz>::push_back_aux(const value_type& value)
{
    value_type value_copy=value;
    reserve_map_at_back();
    *(finish.node+1)=allocate_node();
    //此处应有异常处理 commit or rollback
    construct(finish.cur,value_copy);
    finish.set_node(finish.node + 1);
    finish.cur=finish.first;
}
template<typename T, typename Alloc, size_t BuffSiz>
void deque<T,Alloc,BuffSiz>::push_front_aux(const value_type& value)
{
    value_type value_copy=value;
    reserve_map_at_front();
    *(start.node-1)=allocate_node();
    //此处应有异常处理 commit or rollback
    start.set_node(start.node-1);
    start.cur=start.last-1;
    construct(start.cur,value_copy);
//    catch()
//    {
//        start.set_node(start.node+1);
//        start.cur=start.first;
//        dealocate_node(*(start.node-1));
//        throw;
//    }
}

template<typename T, typename Alloc, size_t BuffSiz>
inline void deque<T,Alloc,BuffSiz>::reserve_map_at_back(size_type nodes_to_add=1)
{
    if(nodes_to_add+1>map_size-(finish.node-map))
        reallocate_map(nodes_to_add,false);
}
template<typename T, typename Alloc, size_t BuffSiz>
inline void deque<T,Alloc,BuffSiz>::reserve_map_at_front(size_type nodes_to_add=1)
{
    if(nodes_to_add>start.node-map)
        reallocate_map(nodes_to_add,true);
}
template<typename T, typename Alloc, size_t BuffSiz>
void deque<T,Alloc,BuffSiz>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
    size_type old_num_nodes=finish.node-start.node+1;
    size_type new_num_nodes=old_num_nodes+nodes_to_add;

    map_pointer new_nstart;
    if(map_size > 2*new_num_nodes)
    {
        new_nstart=map+(map_size-new_num_nodes)/2+
                   (add_at_front? nodes_to_add:0);
        if(new_nstart<start.node)
            copy(start.node,finish.node+1,new_nstart);
        else
            copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
    }
    else
    {
        size_type new_map_size=map_size+max(map_size,nodes_to_add)+2;
        map_pointer new_map=map_allocator::allocate(new_map_size);
        new_nstart=new_map+(new_map_size-new_num_nodes)/2
                   +(add_at_front?nodes_to_add:0);
        copy(start.node,finish.node+1,new_nstart);
        map_allocator::deallocate(map,map_size);
        map=new_map;
        map_size=new_map_size;
    }
    start.set_node(new_nstart);
    finish.set_node(new_nstart+old_num_nodes-1);
}

template<typename T,typename Alloc,size_t BuffSiz>
inline void deque<T,Alloc,BuffSiz>::pop_back()
{
    if(finish.cur!=finish.first)
    {
        --finish.cur;
        destroy(finish.cur);
    }
    else
        pop_back_aux();
}
template<typename T,typename Alloc, size_t BuffSiz>
void inline deque<T,Alloc,BuffSiz>::pop_back_aux()
{
    deallocate_node(finish.first);
    finish.set_node(finish.node-1);
    finish.cur=finish.last-1;
    destroy(finish.cur);
}
template<typename T,typename Alloc,size_t BuffSiz>
inline void deque<T,Alloc,BuffSiz>::pop_front()
{
    if(start.cur!=start.last-1)
    {
        destroy(start.cur);
        ++start.cur;
    }
    else
        pop_front_aux();
}
template<typename T, typename Alloc, size_t BuffSiz>
inline void deque<T,Alloc,BuffSiz>::pop_front_aux()
{
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node+1);
    start.cur=start.first;
}

template<typename T,typename Alloc, size_t BuffSiz>
inline void deque<T, Alloc,BuffSiz>::clear()
{
    for(map_pointer node=start.node+1;node<finish.node;++node)
    {
        destroy(*node,*node+iterator::buffer_size());
        data_allocator::deallocate(*node,iterator::buffer_size());
    }
    if(start.node!=finish.node)
    {
        destroy(start.cur,start.last);
        destroy(finish.first,finish.cur);
        data_allocator::deallocate(start.first,iterator::buffer_size());
    }
    else
        destroy(start.cur,finish.cur);
    finish=start;
}
template<typename T,typename Alloc, size_t BuffSiz>
typename deque<T,Alloc,BuffSiz>::iterator
deque<T,Alloc,BuffSiz>::erase(iterator position)
{
    iterator next=position;
    ++next;
    difference_type index=position-start;
    if(index<size()/2)
    {
        copy_backward(start,position,next);
        pop_front();
    }
    else
    {
        copy(next,finish,position);
        pop_back();
    }
    return start+index;
}
template<typename T, typename Alloc, size_t BuffSiz>
typename deque<T,Alloc,BuffSiz>::iterator
deque<T,Alloc,BuffSiz>::erase(iterator first, iterator last)
{
    if(first==start&&last==finish)
    {
        clear();
        return ;
    }
    else
    {
        difference_type n=last-first;
        difference_type elems_before=first-start;
        if(elems_before<(size()-n)/2)
        {
            copy_backward(start,first,last);
            iterator new_nstart=start+n;
            destroy(start,new_nstart);
            for(map_pointer node=start.node;node<new_nstart.node;++node)
                data_allocator::deallocate(*node,iterator::buffer_size());
            start=new_nstart;
        }
        else
        {
            copy(last,finish,first);
            iterator new_nfinish=finish-n;
            destroy(new_nfinish,finish);
            for(map_pointer node=finish.node;node>new_nfinish.node;--node)
                data_allocator::deallocate(*node,iterator::buffer_size());
            finish=new_nfinish;
        }
        return start+elems_before;
    }
}

template<typename T, typename Alloc, size_t BuffSiz>
typename deque<T,Alloc,BuffSiz>::iterator
deque<T,Alloc,BuffSiz>::insert(iterator position, const value_type&value)
{
    if(position==start)
    {
        push_front(value);
        return start;
    }
    else if(position==finish)
    {
        push_back(value);
        iterator tmp=finish;
        return --tmp;
    }
    else
        return insert_aux(position, value);
}
template<typename T, typename Alloc, size_t BuffSiz>
typename deque<T,Alloc,BuffSiz>::iterator
deque<T,Alloc,BuffSiz>::insert_aux(iterator position,const value_type&value)
{
    difference_type index=position-start;
    value_type value_copy=value;
    if(index<size()/2)
    {
        push_front(front());
        iterator front1=start;
        ++front1;
        iterator front2=front1;
        ++front2;
        position=start+index;
        iterator pos1=position;
        ++pos1;
        copy(front2,pos1,front1);
    }
    else
    {
        push_back(back());
        iterator back1=finish;
        --back1;
        iterator back2=back1;
        --back2;
        position=start+index;
        copy_backward(position,back2,back1);
    }
    *position=value;
    return position;
}

template<typename T,typename Alloc, size_t BuffSiz>
deque<T,Alloc,BuffSiz>::deque()
    :map(0),map_size(0),start(),finish()
{
    create_map_and_nodes(0);
}

template<typename T,typename Alloc,size_t BuffSiz>
deque<T,Alloc,BuffSiz>::deque(const deque&deq)
    :map(0),map_size(0),start(),finish()
{
    create_map_and_nodes(deq.size());
    map_pointer cur;
    try
    {
        uninitialized_copy(deq.begin(),deq.end(),begin());
    }
    catch(...)
    {
        throw;
    }
}
template<typename T,typename Alloc,size_t BuffSiz>
deque<T,Alloc,BuffSiz>&
deque<T,Alloc,BuffSiz>::operator=(const deque&deq)
{
    if(this!=&deq)
    {
        deque tmp(deq);
        swap(tmp);
    }
    return *this;
}

template<typename T,typename Alloc,size_t BuffSiz>
deque<T,Alloc,BuffSiz>::deque(deque&&deq) noexcept
    :map(0),map_size(0),start(),finish()
{
    create_map_and_nodes(0);
    swap(deq);
}

template<typename T,typename Alloc,size_t BuffSiz>
deque<T,Alloc,BuffSiz>&
deque<T,Alloc,BuffSiz>::operator=(deque&&deq)noexcept
{
    if(this!=&deq)
    {
        deque tmp(move(deq));
        swap(tmp);
    }
    return *this;
}

template<typename T,typename Alloc,size_t BuffSiz>
deque<T,Alloc,BuffSiz>::~deque()
{
    destroy(start,finish);
    for(map_pointer cur=start.node;cur<=finish.node;++cur)
        deallocate_node(*cur);
    map_allocator::deallocate(map,map_size);
}

template<typename T,typename Alloc,size_t BuffSiz>
template<typename InputIterator>
deque<T,Alloc,BuffSiz>::deque(InputIterator first,InputIterator last)
{
    size_type numElements=distance(first,last);
    create_map_and_nodes(numElements);
    try
    {
        uninitialized_copy(first,last,start);
    }
    catch(...)
    {
        for(map_pointer cur=start.node;cur<=finish.node;++cur)
            deallocate_node(*cur);
        map_allocator::deallocate(map,map_size);
    }
}

template<typename T,typename Alloc,size_t BuffSiz>
void deque<T,Alloc,BuffSiz>::swap(deque&deq)
{
    stdExercise::swap(map,deq.map);
    stdExercise::swap(map_size,deq.map_size);
    stdExercise::swap(start,deq.start);
    stdExercise::swap(finish,deq.finish);
}
}///end of namespace stdExercise
#endif // STL_DEQUE_H_INCLUDED
