#ifndef STL_VECTOR_H_INCLUDED
#define STL_VECTOR_H_INCLUDED

#include "Memory.h"
#include "type_traits.h"

namespace stdExercise
{
template<typename T,typename Alloc=alloc>
class vector
{
public:                                                            //内嵌类型设定
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type*         iterator;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
protected:
    typedef simple_alloc<value_type,Alloc> data_allocator;
    iterator start;                                                 //目前使用空间的头
    iterator finish;                                                //目前使用空间的尾
    iterator end_of_storage;                                        //目前可用空间的尾
protected:
    void insert_aux(iterator position,const value_type&x);
    void deallocate()
    {
        if(start)
            data_allocator::deallocate(start,end_of_storage-start);
    }

    void fill_initialize(size_type n,const value_type&value)       //申请能够存储n个value_type值的空间
    {                                                                 //并初始化为value
        start=allocate_and_fill(n,value);
        finish=start+n;
        end_of_storage=finish;
    }
    iterator allocate_and_fill(size_type n,const T&x)
    {
        iterator result=data_allocator::allocate(n);
        uninitialized_fill_n(result,n,x);
        return result;
    }
public:                                                               //拷贝控制操作
    vector():start(0),finish(0),end_of_storage(0){}                   //默认构造函数
    vector(const vector&vec);                                        //拷贝构造函数
    vector& operator=(const vector&rhs);                            //拷贝赋值运算符
    ~vector()                                                         //析构函数
    {
        destroy(start,finish);
        deallocate();
    }

    vector(vector&&vec)noexcept;                                    //移动构造函数
    vector& operator=(vector&&rhs)noexcept;                        //移动赋值运算符

    reference operator[](size_type n){return *(begin()+n);}       //下标访问运算符
    vector(size_type n,const T&value) { fill_initialize(n,value); }
    vector(int n,const T&value) { fill_initialize(n,value); }
    vector(long n,const T&value) { fill_initialize(n,value); }
    template<typename ForwardIterator>
    vector(ForwardIterator first,ForwardIterator last);
    explicit vector(size_type n);
public:
    void reserve(size_type n);
    void insert(iterator position,size_type n,const T&x);
    iterator begin()const{return start;}
    iterator end() const{return finish;}
    size_type size() const {return static_cast<size_type>(end()-begin());}
    size_type capacity()const
    {
        return static_cast<size_type>(end_of_storage-begin());
    }
    bool empty()const {return begin()==end();}
    reference front(){return *begin();}
    const_reference front()const {return *begin();}
    reference back() { return *(end()-1); }
    const_reference back() const {return *(end()-1);}
    void push_back(const T&value)
    {
        if(finish!=end_of_storage)
        {
            construct(finish,value);
            ++finish;
        }
        else
        {
            insert_aux(end(),value);
        }
    }
    void pop_back()
    {
        --finish;
        destroy(finish);
    }

    iterator erase(iterator position)
    {
        if(position+1!=end())
        {
            copy(position+1,end(),position);
        }
        pop_back();
        return position;
    }
    iterator erase(iterator first,iterator last)
    {
        iterator i=copy(last,finish,first);
        destroy(i,finish);
        finish=finish-(last-first);
        return first;
    }
    void resize(size_type new_size,const T&value=T())
    {
        if(new_size<size())
        {
            erase(begin()+new_size,end());
        }
        else
        {
            insert(end(),new_size-size(),value);
        }
    }

    void clear(){ erase(begin(),end()); }

    void swap(vector&vec);

};//end of container vector

template<typename T,typename Alloc>
void vector<T,Alloc>::insert_aux(iterator position,const T&value)
{
    if(finish !=end_of_storage)
    {
        construct(finish,*(finish-1));
        ++finish;
        T value_copy=value;
        copy_backward(position, finish-2,finish-1);
        *position=value_copy;
    }
    else
    {
        const size_type old_size=size();
        const size_type len=old_size != 0 ? 2*old_size : 1;
        iterator new_start=data_allocator::allocate(len);
        iterator new_finish=new_start;
        try
        {
            new_finish=uninitialized_copy(start,position,new_start);
            construct(new_finish,value);
            ++new_finish;
            new_finish=uninitialized_copy(position,finish,new_finish);
        }
        catch(...)
        {
            destroy(new_start,new_finish);
            data_allocator::deallocate(new_start,len);
            throw;
        }
        destroy(begin(),end());
        deallocate();

        start=new_start;
        finish=new_finish;
        end_of_storage=start+len;
    }
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position ,size_type n, const T &x)
{
    if(n!=0)
    {
        if(static_cast<size_type>(end_of_storage-finish)>=n)
        {
            T x_copy=x;
            const size_type elems_after = finish-position;
            iterator old_finish=finish;
            if(elems_after >n )
            {
                uninitialized_copy(finish-n,finish,finish);
                finish+=n;
                copy_backward(position, old_finish - n, old_finish);
                fill(position, position + n, x_copy);
            }
            else
            {
                uninitialized_fill_n(finish, n- elems_after,x_copy);
                finish += n - elems_after;
                uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        }
        else
        {
            const size_type old_size = size();
            const size_type len= old_size + max(old_size,n);
            iterator new_start=data_allocator::allocate(len);
            iterator new_finish=new_start;
            try
            {
                new_finish=uninitialized_copy(start,position,new_start);
                new_finish=uninitialized_fill_n(new_finish,n,x);
                new_finish=uninitialized_copy(position,finish,new_finish);
            }
            catch(...)
            {
                destroy(new_start,new_finish);
                data_allocator::deallocate(new_start,len);
                throw;
            }
            destroy(start,finish);
            deallocate();
            start = new_start ;
            finish = new_finish ;
            end_of_storage = start + len ;
        }
    }
}

template<typename T,typename Alloc>
vector<T,Alloc>::vector(const vector&vec)
{
    iterator new_start=data_allocator::allocate(vec.size());
    try
    {
        uninitialized_copy(vec.begin(),vec.end(),new_start);
    }
    catch(...)
    {
        data_allocator::deallocate(new_start,vec.size());
        throw;
    }
    size_type num_elements=vec.size();
    start=new_start;
    finish=new_start+num_elements;
    end_of_storage=new_start+num_elements;
}

template<typename T,typename Alloc>
vector<T,Alloc>&
vector<T,Alloc>::operator=(const vector&rhs)
{
    if(this!=&rhs)
    {
        if(capacity()<rhs.size())
        {
            vector tmp(rhs);
            swap(tmp);
        }
        else
        {
            clear();
            uninitialized_copy(rhs.begin(),rhs.end(),start);
        }
    }
    return *this;
}

template<typename T,typename Alloc>
vector<T,Alloc>::vector(vector&&vec)noexcept
{
    start=vec.start;
    finish=vec.finish;
    end_of_storage=vec.end_of_storage;
    vec.start=vec.finish=vec.end_of_storage=nullptr;
}

template<typename T,typename Alloc>
vector<T,Alloc>&
vector<T,Alloc>::operator=(vector&&rhs)noexcept
{
    if(this!=&rhs)
    {
        vector tmp(move(rhs));
        swap(tmp);
    }
    return *this;
}

template<typename T,typename Alloc>
vector<T,Alloc>::vector(size_type n)
{
    start=data_allocator::allocate(n);
    finish=start;
    end_of_storage=start+n;
}

template<typename T,typename Alloc>
template<typename InputIterator>
vector<T,Alloc>::vector(InputIterator first,InputIterator last)
{
    size_type n=distance(first,last);
    start=data_allocator::allocate(n);
    uninitialized_copy(first,last,start);
    finish=start+n;
    end_of_storage=finish;
}

template<typename T,typename Alloc>
void vector<T,Alloc>::reserve(size_type n)
{
    if( n > capacity() )
    {
        value_type*new_start=data_allocator::allocate(n);
        try
        {
            uninitialized_copy(begin(),end(),new_start);
        }
        catch(...)
        {
            data_allocator::deallocate(new_start,n);
            throw;
        }
        size_type num_elements=size();
        destroy(begin(),end());
        start=new_start;
        finish=new_start+num_elements;
        end_of_storage=new_start+n;
    }
}

template<typename T,typename Alloc>
void vector<T,Alloc>::swap(vector&vec)
{
    stdExercise::swap(start,vec.start);
    stdExercise::swap(finish,vec.finish);
    stdExercise::swap(end_of_storage,vec.end_of_storage);
}

}//end of namespace stdExercise

#endif // STL_VECTOR_H_INCLUDED
