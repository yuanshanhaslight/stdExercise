#ifndef STL_ITERATOR_H_INCLUDED
#define STL_ITERATOR_H_INCLUDED

#include <cstddef>

namespace stdExercise
{

struct input_iterator_tag {};                                            //输入迭代器
struct output_iterator_tag {};                                           //输出迭代器
struct forward_iterator_tag :public input_iterator_tag{};              //前向迭代器
struct bidirectional_iterator_tag:public forward_iterator_tag {};     //双向迭代器
struct random_access_iterator_tag:public bidirectional_iterator_tag{};//随机存取迭代器

template<typename Category,typename T,typename Distance=ptrdiff_t,
          typename Pointer=T*,typename Reference=T&>
struct iterator
{
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance     difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

template<typename Iterator>
struct iterator_traits                                                    //迭代器特征提取器
{
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer               pointer;
    typedef typename Iterator::reference            reference;
};

template<typename T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag    iterator_category;
    typedef T                                value_type;
    typedef ptrdiff_t                        difference_type;
    typedef T*                               pointer;
    typedef T&                               reference;
};

template<typename T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag    iterator_category;
    typedef T                                value_type;
    typedef ptrdiff_t                        difference_type;
    typedef const T*                        pointer;
    typedef const T&                        reference;
};

template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*> (0);
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*> (0);
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first,InputIterator last,input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n=0;
    while(first!=last)
    {
        ++first;
        ++n;
    }
    return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first,RandomAccessIterator last,random_access_iterator_tag)
{
    return last-first;
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first,InputIterator last)                           //计算两个迭代器之间的距离
{
    return __distance(first,last,iterator_category(first));
}

template<typename InputIterator,typename Distance>
inline void __advance(InputIterator&i,Distance n,input_iterator_tag)
{
    while(n--) ++i;
}

template<typename BidirectionalIterator,typename Distance>
inline void __advance(BidirectionalIterator&i,Distance n,bidirectional_iterator_tag)
{
    if(n>=0)
        while(n--) ++i;
    else
        while(n++) --i;
}

template<typename RandomAccessIterator,typename Distance>
inline void __advance(RandomAccessIterator&i,Distance n,random_access_iterator_tag)
{
    i+=n;
}

template<typename InputIterator,typename Distance>
inline void advance(InputIterator &i,Distance n)                                   //使迭代器前进距离n
{
    __advance(i,n,iterator_category(i));
}
}//end of namespace stdExercise
#endif // STL_ITERATOR_H_INCLUDED
