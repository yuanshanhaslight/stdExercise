#ifndef STL_UNINITIALIZED_H_INCLUDED
#define STL_UNINITIALIZED_H_INCLUDED

#include <cassert>
#include "type_traits.h"
#include "stl_algobase.h"
#include "stl_construct.h"

namespace stdExercise
{

//将first~last区间内的数据拷贝到result指向的未初始化的内存上
///.................................uninitialized_copy..........................///

template<typename InputIterator,typename ForwardIterator,typename T>
inline ForwardIterator __uninitialized_copy(
                InputIterator first,
                InputIterator last,
                ForwardIterator result,
                T*);

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(
                InputIterator first,
                InputIterator last,
                ForwardIterator result,
                __true_type);

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(
                InputIterator first,
                InputIterator last,
                ForwardIterator result,
                __false_type);

template<typename InputIterator,typename ForwardIterator>
ForwardIterator uninitialized_copy(
                InputIterator first,
                InputIterator last,
                ForwardIterator result)
{
    return __uninitialized_copy(first,last,result,value_type(result));
}

template<typename InputIterator,typename ForwardIterator,typename T>
inline ForwardIterator __uninitialized_copy(
                InputIterator first,
                InputIterator last,
                ForwardIterator result,
                T*)
{
    typedef typename __type_traits<T>::is_POD_type is_POD ;
    return __uninitialized_copy_aux(first,last,result,is_POD());
}

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(
                InputIterator first,
                InputIterator last,
                ForwardIterator result,
                __true_type)
{
    return copy(first,last,result);
}

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(
                InputIterator first,
                InputIterator last,
                ForwardIterator result,
                __false_type)
{
    ForwardIterator cur=result;
    for(;first!=last;++first,++cur)
    {
        construct(&*cur,*first);
    }
    return cur;
}

inline char* uninitialized_copy(const char*first,const char*last,char*result)
{
    memmove(result,first,last-first);
    return result+(last-first);
}

inline wchar_t* uninitialized_copy(const wchar_t*first,const wchar_t*last,wchar_t*result)
{
    memmove(result,first,sizeof(wchar_t)*(last-first));
    return result+(last-first);
}

//将first~last所指定的未初始化的内存区间全部填充用户指定的值
///..................................uninitialized_fill..............................///

template<typename ForwardIterator,typename T,typename T1>
inline void __unitialized_fill(
                        ForwardIterator first,
                        ForwardIterator last,
                        const T&x,
                        T1*);

template<typename ForwardIterator,typename T>
inline void
__unitialized_fill_aux(
                       ForwardIterator first,
                       ForwardIterator last,
                       const T&x,
                       __false_type);

template<typename ForwardIterator,typename T>
inline void
__unitialized_fill_aux(
                       ForwardIterator first,
                       ForwardIterator last,
                       const T&x,
                       __true_type);

template<typename ForwardIterator,typename T>
void uninitialized_fill(
                ForwardIterator first,
                ForwardIterator last,
                const T&x)
{
    __unitialized_fill(first,last,x,value_type(first));
}

template<typename ForwardIterator,typename T,typename T1>
inline void __unitialized_fill(
                        ForwardIterator first,
                        ForwardIterator last,
                        const T&x,
                        T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    __unitialized_fill_aux(first,last,x,is_POD());
}

template<typename ForwardIterator,typename T>
inline void
__unitialized_fill_aux(
                       ForwardIterator first,
                       ForwardIterator last,
                       const T&x,
                       __true_type)
{
    fill(first,last,x);
}

template<typename ForwardIterator,typename T>
inline void
__unitialized_fill_aux(
                       ForwardIterator first,
                       ForwardIterator last,
                       const T&x,
                       __false_type)
{
    ForwardIterator cur=first;
    for(;cur!=last;++cur)
    {
        construct(&*cur,x);
    }
}

//从first所指的地址开始，填充n个用户指定的值
///.................................uninitialized_fill_n...................///

template<typename ForwardIterator ,typename Size,typename T,typename T1>
inline ForwardIterator __unitialized_fill_n(ForwardIterator first,Size n,const T&x,T1*);

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator
__unitialized_fill_n_aux(ForwardIterator first,Size n,const T&x,__true_type);

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator
__unitialized_fill_n_aux(ForwardIterator first,Size n,const T&x,__false_type);

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator uninitialized_fill_n(
                ForwardIterator first,
                Size n,
                const T&x)
{
    return __unitialized_fill_n(first,n,x,value_type(first));
}

template<typename ForwardIterator ,typename Size,typename T,typename T1>
inline ForwardIterator __unitialized_fill_n(ForwardIterator first,Size n,const T&x,T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __unitialized_fill_n_aux(first,n,x,is_POD());
}

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator
__unitialized_fill_n_aux(ForwardIterator first,Size n,const T&x,__true_type)
{
    return fill_n(first,n,x);
}

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator
__unitialized_fill_n_aux(ForwardIterator first,Size n,const T&x,__false_type)
{
    ForwardIterator cur=first;
    for(;n>0;--n,++cur)
    {
        construct(&*cur,x);
    }
    return cur;
}

}//end of namespace stdExercise
#endif // STL_UNINITIALIZED_H_INCLUDED
