#ifndef STL_CONSTRUCT_H_INCLUDED
#define STL_CONSTRUCT_H_INCLUDED

#include <new>
#include "type_traits.h"
#include "stl_iterator.h"

namespace stdExercise
{

///.................................construct()..................///

template<typename T1,typename T2>
inline void construct(T1*p,const T2&value)
{
    new(p) T1(value);                         //调用定位new构造对象
}

///.................................destroy()...................///

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__false_type);

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator,__true_type);

template<typename ForwardIterator,typename T>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*);

template<typename T>
inline void destroy(T*p)
{
    p->~T();                                   //调用对象的析构函数销毁对象
}

template<typename ForwardIterator>
inline void destroy(ForwardIterator first,ForwardIterator last)
{
    __destroy(first,last,value_type(first));
}

template<typename ForwardIterator,typename T>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first,last,trivial_destructor());
}

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__false_type)
{
    for(;first!=last;++first)
        destroy(&*first);
}

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator,__true_type){}

inline void destroy(char*,char*){}

inline void destroy(wchar_t*,wchar_t*){}

}//end of namespace stdExercise

#endif // STL_CONSTRUCT_H_INCLUDED
