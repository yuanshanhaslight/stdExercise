#ifndef STL_NUMERIC_H_INCLUDED
#define STL_NUMERIC_H_INCLUDED

#include "stl_iterator.h"

namespace stdExercise
{
///..................accumulate...................///
template<typename InputIterator,typename T>
T accumulate(InputIterator first,
             InputIterator last,
             const T init)
{
    for(;first!=last;++first)
        init =init + *first;
    return init;
}

template<typename InputIterator ,typename T, typename BinaryOperation>
T accumulate(InputIterator first,
             InputIterator last,
             T init,
             BinaryOperation bp)
{
    for(;first!=last;++first)
        init=bp(init,*first);
    return init;
}

template<typename InputIterator,typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first,
                           InputIterator last,
                           OutputIterator result)
{
    if(first!=last)
    {
        *result=*first;

        //需要使用iterator_traits<InputIterator>::value_type
        //声明一个临时变量来保存前一个值，使得in place 方式也能正确进行
        typename iterator_traits<InputIterator>::value_type value=*first;
        while(++first!=last)
        {
            typename iterator_traits<InputIterator>::value_type tmp=*first;
            *++result=tmp-value;
            value=tmp;
        }
    }
    return ++result;
}

template<typename InputIterator,typename OutputIterator,typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first,
                           InputIterator last,
                           OutputIterator result,
                           BinaryOperation binary_op)
{
    if(first!=last)
    {
        *result=*first;

        //需要使用iterator_traits<InputIterator>::value_type
        //声明一个临时变量来保存前一个值，使得in place 方式也能正确进行
        typename iterator_traits<InputIterator>::value_type value=*first;
        while(++first!=last)
        {
            typename iterator_traits<InputIterator>::value_type tmp=*first;
            *++result=binary_op(tmp,value);
            value=tmp;
        }
    }
    return result;
}

///...............................inner_product.......................///
template<typename InputIterator1 ,typename InputIterator2,typename T>
T inner_product(InputIterator1 first1,
                 InputIterator1 last1,
                 InputIterator2 first2,
                 T init)
{
    while(first1!=last1)
    {
        init=init+*first1 * *first2;
        ++first1;
        ++first2;
    }
    return init;
}

template<typename InputIterator1 ,typename InputIterator2,typename T,
          typename BinaryOperation1,typename BinaryOperation2>
T inner_product(InputIterator1 first1,
                 InputIterator1 last1,
                 InputIterator2 first2,
                 T init,
                 BinaryOperation1 binary_op1,
                 BinaryOperation2 binary_op2)
{
    while(first1!=last1)
    {
        init=binary_op1(init,binary_op2(*first1,*first2));
        ++first1;
        ++first2;
    }
    return init;
}

///.............................partial_sum.................///
template<typename InputIterator,typename OutputIterator>
OutputIterator partial_sum(InputIterator first,
                            InputIterator last,
                            OutputIterator result)
{
    if(first!=last)
    {
        *result = *first;
        typename iterator_traits<InputIterator>::value_type value=*first;
        while(++first!=last)
        {
            value=value+*first;
            *++result=value;
        }
    }
    return ++result;
}

template<typename InputIterator ,typename OutputIterator,typename BinaryOperation>
OutputIterator partial_sum(InputIterator first,
                            InputIterator last,
                            OutputIterator result,
                            BinaryOperation binary_op)
{
    if(first!=last)
    {
        *result = *first;
        typename iterator_traits<InputIterator>::value_type value=*first;
        while(++first!=last)
        {
            value=binary_op(value,*first);
            *++result=value;
        }
    }
    return ++result;
}

///..............................power........................///
template<typename T,typename Integer,typename MonoidOperation>
T power(T value, Integer n,MonoidOperation op)
{
    if(n==0)
        return identity_element(op);
    else
    {
        while((n&1)==0)
        {
            n >>=1;
            value=op(value,value);
        }
        T result=value;
        n>>=1;
        while(n!=0)
        {
            value= op(value,value);
            if((n&1)!=0)
                result=op(result,value);
            n>>=1;
        }
        return result;
    }
}
template<typename ForwardIterator,typename T>
ForwardIterator iota(ForwardIterator first,
                     ForwardIterator last,
                     T value)
{
    for(;first!=last;++first)
        *first=value++;
}
}//end of stdExercise
#endif // STL_NUMERIC_H_INCLUDED
