//equal, fill, fill_n, iter_swap, lexicographical_compare
//max, min, mismatch, swap, copy, copy_backward
#ifndef STL_ALGOBASE_H_INCLUDED
#define STL_ALGOBASE_H_INCLUDED

#include "stl_iterator.h"
#include "type_traits.h"

namespace stdExercise
{

///.........................memmove and memcpy,,,,,,,,,,,,,,,,///
inline void* memmove(void *dst,const void *src,size_t n)
{
    char temp[n];
    size_t i;
    char *pd = static_cast<char*>(dst);
    const char *ps = static_cast<const char*>(src);

    for (i = 0; i < n; i++)
        temp[i] = ps[i];
    for (i = 0; i < n; i++)
        pd[i] = temp[i];

    return static_cast<void*>(pd+n);
}

inline void* memcpy(void *dst, const void *src, size_t n)
{
    const char *ps=static_cast<const char*>(src);
    char *pd=static_cast<char*>(dst);
    while(n--)
        *(pd++)=*(ps++);
    return static_cast<void*>(pd);
}

///...............................equal filll fill_n ...  .............///
template<typename InputIterator1,typename InputIterator2>
inline bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2)
{
        for(;first1!=last1;++first1,++first2)
        {
            if(*first1 != *first2)
                return false;
        }
        return true;
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
inline bool equal(InputIterator1 first1,
                  InputIterator1 last1,
                  InputIterator2 first2,
                  BinaryPredicate binary_pred)
{
    for( ; first1 != last1 ; ++first1, ++first2 )
        if(!(binary_pred(*first1,*first2)))
            return false;
    return true;
}

template<typename ForwardIterator,typename T>
void fill(ForwardIterator first,ForwardIterator last,const T&value)
{
    for(;first!=last;++first)
        *first=value;
}
template<typename OutputIterator ,typename Size,typename T>
OutputIterator fill_n(OutputIterator first,Size n,const T&value)
{
    for( ; n > 0 ; --n, ++first )
        *first=value;
    return first;
}

template<typename ForwardIterator1,typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 iter1,ForwardIterator2 iter2)
{
    typename iterator_traits<ForwardIterator1>::value_type tmp=*iter2;
    *iter2=*iter1;
    *iter1=tmp;
}

template<typename InputIterator1, typename InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1,
                                    InputIterator1 last1,
                                    InputIterator2 first2,
                                    InputIterator2 last2)

{
    for( ; first1!=last1 && first2 != last2 ; ++first1, ++last2 )
    {
        if(*first1 < *first2)
            return true;
        if( *first2 < *first1 )
            return false;
    }
    return first1==last1 && first2 != last2 ;
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
inline bool lexicographical_compare(InputIterator1 first1,
                                    InputIterator1 last1,
                                    InputIterator2 first2,
                                    InputIterator2 last2,
                                    Compare cmp)

{
    for( ; first1!=last1 && first2 != last2 ; ++first1, ++last2 )
    {
        if( cmp( *first1 , *first2 ))
            return true;
        if( cmp( *first2 , *first1 ))
            return false;
    }
    return first1==last1 && first2 != last2 ;
}

template<typename T>
inline const T& max(const T&lhs,const T&rhs)
{
    return lhs < rhs ? rhs : lhs ;
}

template<typename T, typename Compare>
inline const T& max(const T&lhs,const T&rhs, Compare cmp)
{
    return cmp(lhs,rhs) ? rhs : lhs ;
}

template<typename T>
inline const T& min(const T&lhs,const T&rhs)
{
    return lhs < rhs ? lhs : rhs ;
}

template<typename T, typename Compare>
inline const T& min(const T&lhs,const T&rhs, Compare cmp)
{
    return cmp(lhs,rhs) ? lhs : rhs ;
}


//template<typename InputIterator1, typename InputIterator2>
//inline pair<InputIterator1, InputIterator2>
//mismatch(InputIterator1 first1,InputIterator1 last1, InputIterator2 first2)
//{
//    while(first1!= last1 && *first1==*first2)
//    {
//        ++first1;
//        ++first2;
//    }
//    return pair<InputIterator1,InputIterator2>(first1,first2);
//}
//template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
//inline pair<InputIterator1, InputIterator2>
//mismatch(InputIterator1 first1,InputIterator1 last1, InputIterator2 first2,
//         BinaryPredicate bp)
//{
//    while(first1!= last1 && bp(*first1,*first2))
//    {
//        ++first1;
//        ++first2;
//    }
//    return pair<InputIterator1,InputIterator2>(first1,first2);
//}

template<typename T>
inline void swap( T&lhs, T&rhs)
{
    T tmp=lhs;
    lhs=rhs;
    rhs=tmp;
}

///.....................copy............................///
inline char* copy(const char* first,const char* last, char*result)
{
    memmove(result,first,last-first);
    return result+(last-first);
}

inline wchar_t* copy(const wchar_t *first,
                     const wchar_t *last,
                     wchar_t *result)
{
    memmove(result,first,sizeof(wchar_t)*(last-first));
    return result+(last-first);
}

template<typename RandomAccessIterator,typename OutputIterator, typename  Distance>
inline OutputIterator __copy_d(RandomAccessIterator first,
                               RandomAccessIterator last,
                               OutputIterator result,
                               Distance*)
{
    for(Distance n=last-first; n>0;--n,++first,++result)
        *result=*first;
    return result;
}

template<typename T>
inline T* __copy_t(const T*first,const T*last,T*result,__true_type)
{
    memmove(result,first,sizeof(T)*(last-first));
    return result+(last-first);
}

template<typename T>
inline T* __copy_t(const T*first,const T*last,T*result,__false_type)
{
    return __copy_d(first,last,result,distance_type(first));
}

template<typename InputIterator , typename OutputIterator>
inline OutputIterator __copy(InputIterator first,InputIterator last,
                             OutputIterator result,input_iterator_tag)
{
    for(;first!=last;++first,++result)
        *result=*first;
    return result;
}

template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first,
                             RandomAccessIterator last,
                             OutputIterator result,
                             random_access_iterator_tag)
{
    return __copy_d(first,last,result,distance_type(first));
}

template<typename InputIterator,typename OutputIterator>
struct __copy_dispatch
{
    OutputIterator operator()(InputIterator first,
                              InputIterator last,
                              OutputIterator result)
    {
        return __copy(first,last,result,iterator_category(first));
    }
};

template<typename T>
struct __copy_dispatch<T*,T*>
{
    T* operator()(T *first, T *last, T *result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first,last,result,t());
    }
};

template<typename T>
struct __copy_dispatch<const T*,T*>
{
    T* operator()(const T*first,const T*last,T*result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first,last,result,t());
    }
};

template<typename InputIterator,typename OutputIterator>
inline OutputIterator copy(InputIterator first,InputIterator last,OutputIterator result)
{
    return __copy_dispatch<InputIterator,OutputIterator>()(first,last,result);
}

///.......................copy_backward ....................///

inline char* copy_backward(const char* first,const char* last, char*result)
{
    memmove(result-(last-first),first,last-first);
    return result-(last-first);
}

inline wchar_t* copy_backward(const wchar_t *first,
                     const wchar_t *last,
                     wchar_t *result)
{
    memmove(result-(last-first),first,sizeof(wchar_t)*(last-first));
    return result-(last-first);
}

template<typename RandomAccessIterator,typename Bidirectionalterator2, typename  Distance>
inline Bidirectionalterator2 __copy_backward_d(
                    RandomAccessIterator first,
                    RandomAccessIterator last,
                    Bidirectionalterator2 result,
                    Distance*)
{
    --last;
    --result;
    for(Distance n=last-first+1; n>0;--n,--last,--result)
        *result=*last;
    return result;
}

template<typename T>
inline T* __copy_backward_t(const T*first,const T*last,T*result,__true_type)
{
    memmove(result-(last-first),first,sizeof(T)*(last-first));
    return result-(last-first);
}

template<typename T>
inline T* __copy_backward_t(const T*first,const T*last,T*result,__false_type)
{
    return __copy_backward_d(first,last,result,distance_type(first));
}

template<typename Bidirectionalterator1 , typename Bidirectionalterator2>
inline Bidirectionalterator2 __copy_backward(
                Bidirectionalterator1 first,
                Bidirectionalterator1 last,
                Bidirectionalterator2 result,
                bidirectional_iterator_tag)
{
    --last;
    --result;
    for(;last!=first;--last,--result)
        *result=*last;
    return result;
}

template<typename RandomAccessIterator, typename Bidirectionalterator2>
inline Bidirectionalterator2 __copy_backward(RandomAccessIterator first,
                             RandomAccessIterator last,
                             Bidirectionalterator2 result,
                             random_access_iterator_tag)
{
    return __copy_backward_d(first,last,result,distance_type(first));
}

template<typename Bidirectionalterator1,typename Bidirectionalterator2>
struct __copy_backward_dispatch
{
    Bidirectionalterator2 operator()(Bidirectionalterator1 first,
                              Bidirectionalterator1 last,
                              Bidirectionalterator2 result)
    {
        return __copy_backward(first,last,result,iterator_category(first));
    }
};

template<typename T>
struct __copy_backward_dispatch<T*,T*>
{
    T* operator()(T *first, T *last, T *result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first,last,result,t());
    }
};

template<typename T>
struct __copy_backward_dispatch<const T*,T*>
{
    T* operator()(const T*first,const T*last,T*result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_backward_t(first,last,result,t());
    }
};

template<typename BidirectionalIterator1,typename Bidirectionalterator2>
inline Bidirectionalterator2 copy_backward(BidirectionalIterator1 first,
                                           BidirectionalIterator1 last,
                                           Bidirectionalterator2 result)
{
    return __copy_backward_dispatch<BidirectionalIterator1,
                                    Bidirectionalterator2>()(first,last,result);
}

}//end of namespace stdExercise
#endif // STL_ALGOBASE_H_INCLUDED
