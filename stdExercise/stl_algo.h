#ifndef STL_ALGO_H_INCLUDED
#define STL_ALGO_H_INCLUDED

#include "stl_iterator.h"
#include "type_traits.h"
#include "stl_heap.h"
#include <assert.h>

namespace stdExercise
{

///......................................adjacent_find........................///
template<typename InputIterator>
InputIterator adjacent_find(InputIterator first,InputIterator last)
{
    for(InputIterator prev=first;first!=last&&++first!=last;++prev)
    {
        if(*prev==*first) return prev;
    }
    return last;
}

template<typename InputIterator,typename BinaryPredicate>
InputIterator adjacent_find(InputIterator first,InputIterator last,BinaryPredicate bp)
{
    for(InputIterator prev=first;first!=last&&++first!=last;++prev)
    {
        if(bp(*prev,*first)) return prev;
    }
    return last;
}

///......................................count count_if........................///
template<typename InputIterator ,typename T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first,InputIterator last,const T&value)
{
    typename iterator_traits<InputIterator>::difference_type cnt=0;
    for(;first!=last;++first)
        if(*first==value)++cnt;
    return cnt;
}

template<typename InputIterator ,typename UnaryPredicate>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first,InputIterator last,UnaryPredicate up)
{
    typename iterator_traits<InputIterator>::difference_type cnt=0;
    for(;first!=last;++first)
        if(up(*first))++cnt;
    return cnt;
}

///................find find_if.....................................///
///................find_end.........................................///
///................find_first_of....................................///
template<typename InputIterator ,typename T>
InputIterator find(InputIterator first,InputIterator last,const T&value)
{
    while (first!=last && *first!=value) ++first;
    return first;
}

template<typename InputIterator ,typename UnaryPredicate>
InputIterator find_if(InputIterator first,InputIterator last,UnaryPredicate up)
{
    while (first!=last && !up(*first)) ++first;
    return first;
}

///.......................自定义版本.................................///
//template<typename InputIterator1,typename InputIterator2>
//InputIterator1 find_end(InputIterator1 first1,InputIterator1 last1,
//                        InputIterator2 first2 ,InputIterator2 last2)
//{
//    InputIterator1 result=last1;
//    while(first1!=last1)
//    {
//        if(*first1==*first2)
//        {
//            InputIterator2 tmp=__find_end(first1,last1,first2,last2);
//            if(tmp==last2)
//                result=first1;
//        }
//        ++first1;
//    }
//    return result;
//}
//
//template<typename InputIterator1,typename InputIterator2>
//InputIterator2 __find_end(InputIterator1 first1,InputIterator1 last1,
//                          InputIterator2 first2,InputIterator2 last2)
//{
//    while(first1!=last1 && first2!=last2 && *first1==*first2 )
//    {
//        ++first1;
//        ++first2;
//    }
//    return first2;
//}

template<typename InputIterator1,typename InputIterator2>
InputIterator1 find_end(InputIterator1 first1,InputIterator1 last1,
                        InputIterator2 first2 ,InputIterator2 last2,
                        input_iterator_tag)
{
    if(first2==last2)
        return last1;
    else
    {
        InputIterator1 result=last1;
        while(true)
        {
            InputIterator1 new_result=search(first1,last1,first2,last2);
            if(new_result==last1)
                return result;
            else
            {
                result=new_result;
                first1=++new_result;
            }
        }
    }
}

//template<typename BidirectionalIterator1,typename BidirectionalIterator2>
//BidirectionalIterator1 find_end(BidirectionalIterator1 first1,BidirectionalIterator1 last1,
//                        BidirectionalIterator2 first2 ,BidirectionalIterator2 last2,
//                        bidirectional_iterator_tag)
//{
//    typedef reverse_iterator<BidirectionalIterator1> reviter1;
//    typedef reverse_iterator<BidirectionalIterator2> reviter2;
//    reviter1 rlast1(first1);
//    reviter2 rlast2(first2);
//
//    if(first2==last2)
//        return last1;
//    else
//    {
//        reviter1 rresult=search(reviter1(last1),rlast1,
//                                reviter2(last2),rlast2);
//        if(rresult==rlast1)
//            return last1;
//        else
//        {
//            BidirectionalIterator1 result=rresult.base();
//            advance(result,-distance(first2,last2));
//            return result;
//        }
//    }
//}
//
//template<typename InputIterator1,typename InputIterator2>
//InputIterator1 find_end(InputIterator1 first1,InputIterator1 last1,
//                        InputIterator2 first2 ,InputIterator2 last2)
//{
//    typedef typename iterator_traits<InputIterator1>::iterator_category iterator_category1;
//    typedef typename iterator_traits<InputIterator2>::iterator_category iterator_category2;
//    return __find_end(first1,last1,first2,last2,iterator_category1(),iterator_category2());
//}

template<typename InputIterator1,typename InputIterator2>
InputIterator1 find_first_of(InputIterator1 first1,InputIterator2 last1,
                              InputIterator2 first2,InputIterator2 last2)
{
    for(;first1!=last1;++first1)
        for(InputIterator2 ite=first2;ite!=last2;++ite)
            if(*first1==*ite) return first1;
    return last1;
}

template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
InputIterator1 find_first_of(InputIterator1 first1,InputIterator2 last1,
                              InputIterator2 first2,InputIterator2 last2,
                              BinaryPredicate bp)
{
    for(;first1!=last1;++first1)
        for(InputIterator2 ite=first2;ite!=last2;++ite)
            if(bp(*first1,*ite)) return first1;
    return last1;
}

///.........................self definition for_each .........................///
//template<typename InputIterator,typename Operation>
//inline void __for_each(InputIterator first,
//                       InputIterator last,
//                       Operation op,
//                       input_iterator_tag)
//{
//    for(;first!=last;++first)
//        op(*first);
//}
//
//template<typename RandomAccessIterator,typename Operation>
//inline void __for_each(RandomAccessIterator first,
//                       RandomAccessIterator last,
//                       Operation op,
//                       random_access_iterator_tag)
//{
//    typename iterator_traits<RandomAccessIterator>::difference_type n;
//    for( n = last - first; n>0 ; --n, ++first)
//        op(*first);
//}
//template<typename InputIterator,typename Operation>
//void for_each(InputIterator first,InputIterator last,Operation op)
//{
//    __for_each(first,last,op,iterator_category(first));
//}

///.........................sgi stl for_each....................///
template<typename InputIterator ,typename Function>
Function for_each(InputIterator first,InputIterator last, Function f)
{
    for(; first!=last; ++first)
        f(*first);
    return f;
}

///..........................generate generate_n ............................///
template<typename ForwardIterator ,typename Generator>
void generate(ForwardIterator first, ForwardIterator last,
               Generator gen)
{
    for(;first!=last;++first)
        *first=gen();
}

template<typename OutputIterator ,typename Size,typename Generator>
OutputIterator generate_n(OutputIterator first, Size n,
               Generator gen)
{
    for(;n>0;--n,++first)
        *first=gen();
    return first;
}

///.................................includes..................///
template<typename InputIterator1, typename InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
               InputIterator2 first2, InputIterator2 last2)
{
    while(first1!=last1&&first2!=last2)
    {
        if(*first1<*first2)
            ++first1;
        else if(*first2<*first1)
            return false;
        else
        {
            ++first1;
            ++first2;
        }
    }
    return first2==last2;
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool includes( InputIterator1 first1, InputIterator1 last1 ,
              InputIterator2 first2, InputIterator2 last2 ,
              Compare comp)
{
    while(first1!=last1&&first2!=last2)
    {
        if(comp(*first1,*first2))
            ++first1;
        else if(comp(*first2,*first1))
            return false;
        else
            ++first1,++first2;
    }
    return first2==last2;
}

///................................max_element min_element.................///
template<typename InputIterator>
InputIterator max_element(InputIterator first, InputIterator last)
{
    if(first==last) return last;
    InputIterator result = first;
    while(++first!=last)
        if(*result<*first)
            result=first;
    return result;
}

template<typename InputIterator, typename Compare >
InputIterator max_element(InputIterator first, InputIterator last,
                           Compare comp)
{
    if(first==last) return last;
    InputIterator result=first;
    while(++first!=last)
        if(comp(*result,*first))
            result=first;
    return result;
}

template<typename InputIterator>
InputIterator min_element(InputIterator first, InputIterator last)
{
    if(first==last) return last;
    InputIterator result = first;
    while(++first!=last)
        if(*first<*result)
            result=first;
    return result;
}

template<typename InputIterator, typename Compare >
InputIterator min_element(InputIterator first, InputIterator last,
                           Compare comp)
{
    if(first==last) return last;
    InputIterator result=first;
    while(++first!=last)
        if(comp(*first,*result))
            result=first;
    return result;
}
///....................merge....................................///
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result)
{
    while(first1!=last1&&first2!=last2)
    {
        if(*first1<*first2)
        {
            *result=*first1;
            ++first1;
        }
        else
        {
            *result=*first2;
            ++first2;
        }
        ++result;
    }
    return copy(first2,last2,copy(first1,last1,result));
}

template<typename InputIterator1, typename InputIterator2,
        typename OutputIterator, typename Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp)
{
    while(first1!=last1&&first2!=last2)
    {
        if(comp(*first1,*first2))
        {
            *result=*first1;
            ++first1;
        }
        else
        {
            *result=*first2;
            ++first2;
        }
        ++result;
    }
    return copy(first2,last2,copy(first1,last1,result));
}

template<typename T>
struct less
{
    bool operator()(const T&lhs,const T&rhs)
    {
        return lhs<rhs;
    }
};

///...................................partition.................................///
template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator partition(ForwardIterator first, ForwardIterator last,
                           UnaryPredicate up)
{
    ForwardIterator result =first;
    for(;first!=last;++first)
    {
        if(up(*first))
        {
            iter_swap(result,first);
            ++result;
        }
    }
    return result;
}

///..........................remove remove_copy remove_if remove_copy_if................///
template<typename InputIterator,typename OutputIterator,typename T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                      OutputIterator result,const T&value)
{
    for(;first!=last;++first)
    {
        if(*first!=value)
        {
            *result=*first;
            ++result;
        }
    }
    return result;
}

template<typename ForwardIterator,typename T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T&value)
{
    first=find(first,last,value);
    ForwardIterator next=first;
    return first==last? first: remove_copy(++next,last,first,value);
}

template<typename InputIterator,typename OutputIterator,typename Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                      OutputIterator result,Predicate pred)
{
    for(;first!=last;++first)
    {
        if(!pred(*first))
        {
            *result=*first;
            ++result;
        }
    }
    return result;
}

template<typename ForwardIterator,typename Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                       Predicate pred)
{
    first=find_if(first,last,pred);
    ForwardIterator next=first;
    return first==last? first: remove_copy_if(++next,last,first,pred);
}

///........................replace replace_if replace_copy replace_copy_if........///
template<typename ForwardIterator ,typename T>
void replace(ForwardIterator first,ForwardIterator last,
              const T& old_value, const T&new_value)
{
    for(;first!=last;++first)
    {
        if(*first==old_value)
            *first=new_value;
    }
}

template<typename ForwardIterator ,typename Predicate,typename T>
void replace_if(ForwardIterator first, ForwardIterator last ,
                 Predicate pred,const T&new_value)
{
    for(;first!=last;++first)
        if(pred(*first)) *first=new_value;
}

template<typename InputIterator ,typename OutputIterator,typename T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T&old_value,
                            const T&new_value)
{
    for(;first!=last;++first,++result)
    {
       if(*first==old_value)
            *result=new_value;
        else
            *result=*first;
    }
     return result;
}

template<typename InputIterator ,typename OutputIterator,typename Predicate,typename T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result,Predicate pred,
                            const T&new_value)
{
    for(;first!=last;++first,++result)
    {
       if(pred(*first))
            *result=new_value;
        else
            *result=*first;
    }
     return result;
}

///...........................reverse reverse_copy...........................///
template<typename BidirectionalIterator>
void __reverse(BidirectionalIterator first,
               BidirectionalIterator last,
               bidirectional_iterator_tag)
{
    while(true)
    {
        if(first==last||first==--last)                    //当没有元素或只有一个元素时，直接返回
            return ;
        iter_swap(first++,last);
    }
}
template<typename RandomAccessIterator>
void __reverse(RandomAccessIterator first,
               RandomAccessIterator last,
               random_access_iterator_tag)
{
    while(first<last)iter_swap(first++,--last);
}

template<typename BidirectionalIterator>
void reverse(BidirectionalIterator first,
              BidirectionalIterator last)
{
    __reverse(first,last,iterator_category(first));
}

template<typename BidirectionalIterator,typename OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first,
                            BidirectionalIterator last,
                            OutputIterator result)
{
    while(first!=last)
    {
        --last;
        *result=*last;
        ++result;
    }
    return result;
}

///.................................rotate rotate_copy............................///
template<typename ForwardIterator,typename OutputIterator>
OutputIterator rotate_copy(ForwardIterator first,
                            ForwardIterator middle,
                            ForwardIterator last,
                            OutputIterator result)
{
    copy(middle,last,result);
    copy(first,middle,result);
}

///.................................search search_n................................///
template<typename ForwardIterator1,typename ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2)
{
// 自定义版本，效率不高
//    while(first1!=last1)
//    {
//        if(*first1==*first2)
//        {
//            ForwardIterator1 cur1=first1;
//            ForwardIterator2 cur2=first2;
//            for(++cur1,++cur2;cur1!=last1&&cur2!=last2;++cur1,++cur2)
//                if(*cur1!=*cur2)
//                {
//                    first1=++cur1;
//                    break;
//                }
//            return cur2==last2 ? first1:last1;
//        }
//        else
//            ++first1;
//    }
//    return last1;

    typedef typename iterator_traits<ForwardIterator1>::difference_type Distance1;
    typedef typename iterator_traits<ForwardIterator2>::difference_type Distance2;

    Distance1 d1=distance(first1,last1);
    Distance2 d2=distance(first2,last2);
    if(d1<d2) return last1;

    ForwardIterator1 current1 =first1;
    ForwardIterator2 current2 = first2;
    while(current2!=last2)
    {
        if(*current1==*current2)
        {
            ++current1;
            ++current2;
        }
        else
        {
            if(d1==d2) return last1;
            current1=++first1;
            current2=first2;
            --d1;
        }
    }
    return first1;
}

template<typename ForwardIterator1,typename ForwardIterator2,typename BinaryPredicate>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2,
                        BinaryPredicate bp)
{
    typedef typename iterator_traits<ForwardIterator1>::difference_type Distance1;
    typedef typename iterator_traits<ForwardIterator2>::difference_type Distance2;

    Distance1 d1=distance(first1,last1);
    Distance2 d2=distance(first2,last2);
    if(d1<d2) return last1;

    ForwardIterator1 current1 =first1;
    ForwardIterator2 current2 = first2;
    while(current2!=last2)
    {
        if(bp(*current1,*current2))
        {
            ++current1;
            ++current2;
        }
        else
        {
            if(d1==d2) return last1;
            current1=++first1;
            current2=first2;
            --d1;
        }
    }
    return first1;
}


///...................................lower_bound.................................///

template<typename ForwardIterator,typename T, typename Distance>
inline ForwardIterator __lower_bound(ForwardIterator first,
                              ForwardIterator last,
                              const T&value,
                              Distance*,
                              forward_iterator_tag)
{
//    初始版本，效率不高
//    while(first!=last)
//    {
//        Distance n=distance(first,last)/2;
//        ForwardIterator middle=first;
//        advance(middle,n);
//        if(*middle<value)
//        {
//            first=middle;
//            ++first
//        }
//        else
//            last=middle;
//    }
//return first;

///...............sgi 版本....................///
    Distance len=distance(first,last);
    Distance half;
    ForwardIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first;
        advance(middle,half);
        if(*middle<value)
        {
            first=middle;
            ++first;
            len=len-half-1;
        }
        else
            len=half;
    }
    return first;
}

template<typename RandomAccessIterator,typename T, typename Distance>
inline RandomAccessIterator __lower_bound(RandomAccessIterator first,
                              RandomAccessIterator last,
                              const T&value,
                              Distance*,
                              random_access_iterator_tag)
{
    Distance len=last-first;
    Distance half;
    RandomAccessIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first+half;
        if(*middle<value)
        {
            first=middle+1;
            len=len-half-1;
        }
        else
            len=half;
    }
    return first;
}

template<typename ForwardIterator,typename T, typename Compare, typename Distance>
inline ForwardIterator __lower_bound(ForwardIterator first,
                              ForwardIterator last,
                              const T&value,
                              Compare cmp,
                              Distance*,
                              forward_iterator_tag)
{
    Distance len=distance(first,last);
    Distance half;
    ForwardIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first;
        advance(middle,half);
        if(cmp(*middle<value))
        {
            first=middle;
            ++first;
            len=len-half-1;
        }
        else
            len=half;
    }
    return first;
}

template<typename RandomAccessIterator,typename T, typename Compare, typename Distance>
inline RandomAccessIterator __lower_bound(RandomAccessIterator first,
                              RandomAccessIterator last,
                              const T&value,
                              Compare cmp,
                              Distance*,
                              random_access_iterator_tag)
{
    Distance len=last-first;
    Distance half;
    RandomAccessIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first+half;
        if(cmp(*middle<value))
        {
            first=middle+1;
            len=len-half-1;
        }
        else
            len=half;
    }
    return first;
}

template<typename ForwardIterator,typename T>
inline ForwardIterator lower_bound(ForwardIterator first,
                            ForwardIterator last,
                            const T&value)
{
    return __lower_bound(first,last,value,distance_type(first),iterator_category(first));
}

template<typename ForwardIterator,typename T,typename Compare>
inline ForwardIterator lower_bound(ForwardIterator first,
                            ForwardIterator last,
                            const T&value,
                            Compare cmp)
{
    return __lower_bound(first,last,value,cmp,distance_type(first),iterator_category(first));
}

///....................................upper_bound...........................///
template<typename ForwardIterator,typename T,typename Distance>
ForwardIterator __upper_bound(ForwardIterator first,
                              ForwardIterator last,
                              const T&value,
                              Distance*,
                              forward_iterator_tag)
{
    Distance len=distance(first,last);
    Distance half;
    ForwardIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first;
        advance(middle,half);
        if(value<*middle)
            len=half;
        else
        {
            first=middle;
            ++first;
            len=len-half-1;
        }
    }
    return first;
}

template<typename RandomAccessIterator,typename T,typename Distance >
RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                   RandomAccessIterator last,
                                   const T&value,
                                   Distance*,
                                   random_access_iterator_tag)
{
    Distance len=last-first;
    Distance half;
    RandomAccessIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first+half;
        if(value<*middle)
            len=half;
        else
        {
            first=middle+1;
            len=len-half-1;
        }
    }
    return first;
}

template<typename ForwardIterator,typename T,typename Compare,typename Distance>
ForwardIterator __upper_bound(ForwardIterator first,
                              ForwardIterator last,
                              const T&value,
                              Compare cmp,
                              Distance*,
                              forward_iterator_tag)
{
    Distance len=distance(first,last);
    Distance half;
    ForwardIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first;
        advance(middle,half);
        if(cmp(value,*middle))
            len=half;
        else
        {
            first=middle;
            ++first;
            len=len-half-1;
        }
    }
    return first;
}

template<typename RandomAccessIterator,typename T,typename Compare,typename Distance >
RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                   RandomAccessIterator last,
                                   const T&value,
                                   Compare cmp,
                                   Distance*,
                                   random_access_iterator_tag)
{
    Distance len=last-first;
    Distance half;
    RandomAccessIterator middle;
    while(len>0)
    {
        half=len>>1;
        middle=first+half;
        if(cmp(value,*middle))
            len=half;
        else
        {
            first=middle+1;
            len=len-half-1;
        }
    }
    return first;
}

template<typename ForwardIterator,typename T>
ForwardIterator upper_bound(ForwardIterator first,
                            ForwardIterator last,
                            const T&value)

{
    return __upper_bound(first,last,value,distance_type(first),iterator_category(first));
}
template<typename ForwardIterator,typename T,typename Compare>
ForwardIterator upper_bound(ForwardIterator first,
                            ForwardIterator last,
                            const T&value,
                            Compare cmp)

{
    return __upper_bound(first,last,value,cmp,distance_type(first),iterator_category(first));
}

///..................................binary_search....................///
template<typename ForwardIterator,typename T>
bool binary_search(ForwardIterator first,
                   ForwardIterator last,
                   const T&value)
{
    ForwardIterator ite=lower_bound(first,last,value);
    if(ite==last)
        return false;
    else
        return *ite<value||value<*ite ? false : true ;
}

template<typename ForwardIterator,typename T,typename Compare>
bool binary_search(ForwardIterator first,
                   ForwardIterator last,
                   const T&value,
                   Compare cmp)
{
    ForwardIterator ite=lower_bound(first,last,value,cmp);
    if(ite==last)
        return false;
    else
        return cmp(*ite,value)||cmp(value,*ite ) ? false : true ;
}

}

#endif // STL_ALGO_H_INCLUDED
