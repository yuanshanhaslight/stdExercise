//还需要实现能够由用户指定元素之间大小比较的函数
#ifndef STL_HEAP_H_INCLUDED
#define STL_HEAP_H_INCLUDED
#include "stl_iterator.h"
namespace stdExercise
{
template<typename DistanceType>
inline DistanceType heap_node_parent(DistanceType n)       //求下标为n的节点的父节点的下标
{
    return (n-1)/2;
}
template<typename DistanceType>
inline DistanceType heap_node_left_child(DistanceType n)   //求下标为n的节点的左子节点的下标
{
    return n*2+1;
}
template<typename DistanceType>
inline DistanceType heap_node_right_child(DistanceType n)  //求下标为n的节点的右子节点的下标
{
    return n*2+2;
}

///.....................默认使用 < 号作为元素比较运算符.......................///

template<typename RandomAccessIterator,typename Distance,typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
                 Distance topIndex, T value);

template<typename RandomAccessIterator ,typename Distance,typename T>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last,
                            Distance*,
                            T*);

template<typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)   //将元素压入最大堆中
{
    __push_heap_aux(first,last,distance_type(first),value_type(first));
}
template<typename RandomAccessIterator ,typename Distance,typename T>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last,
                            Distance*,
                            T*)
{
    __push_heap(first,Distance((last-first)-1),Distance(0),
                T(*(last-1)));
}
template<typename RandomAccessIterator,typename Distance,typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
                 Distance topIndex, T value)
{
    Distance parent=heap_node_parent(holeIndex);
    while(holeIndex>topIndex&&*(first+parent)<value)
    {
        *(first+holeIndex)=*(first +parent);
        holeIndex=parent;
        parent=heap_node_parent(parent);
    }
    *(first+holeIndex)=value;
}

template<typename RandomAccessIterator,typename T,typename Distance>
inline void __pop_heap(RandomAccessIterator first,
                       RandomAccessIterator last,
                       RandomAccessIterator result,
                       T value, Distance*);

template<typename RandomAccessIterator, typename T>
inline void __pop_heap_aux(RandomAccessIterator first,
                           RandomAccessIterator last,T*);

template<typename RandomAccessIterator ,typename Distance,typename T>
inline void __adjust_heap(RandomAccessIterator first,
                          Distance holeIndex,
                          Distance len,
                          T value);

template<typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,                             //从最大堆中弹出元素
                     RandomAccessIterator last)
{
    __pop_heap_aux(first,last,value_type(first));
}
template<typename RandomAccessIterator, typename T>
inline void __pop_heap_aux(RandomAccessIterator first,
                           RandomAccessIterator last,T*)
{
    __pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(first));
}
template<typename RandomAccessIterator,typename T,typename Distance>
inline void __pop_heap(RandomAccessIterator first,
                       RandomAccessIterator last,
                       RandomAccessIterator result,
                       T value, Distance*)
{
    *result=*first;
    __adjust_heap(first,Distance(0),Distance(last-first),value);
}

template<typename RandomAccessIterator ,typename Distance,typename T>
inline void __adjust_heap(RandomAccessIterator first,
                          Distance holeIndex,
                          Distance len,
                          T value)
{
    Distance left,right,largest;
    *(first+holeIndex)=value;
    while(holeIndex<len)
    {
        left=heap_node_left_child(holeIndex);
        right=heap_node_right_child(holeIndex);
        largest=holeIndex;
        if(left<len && *(first+largest) < *(first+left))
            largest=left;
        if(right<len && *(first+largest)< *(first+right))
            largest=right;
        if(largest==holeIndex)
            return ;
        else
        {
            *(first+holeIndex)=*(first+largest);
            *(first+largest)=value;
            holeIndex=largest;
        }
    }
}

template<typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last)                                  //堆排序
{
    while(last-first>1)
        pop_heap(first,last--);
}

template<typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first,
                 RandomAccessIterator last,
                 T*, Distance*);
template<typename RandomAccessIterator>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last)                             //建立最大堆
{
    __make_heap(first,last,value_type(first),distance_type(first));
}
template<typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first,
                 RandomAccessIterator last,
                 T*, Distance*)
{
    if(last-first<2) return ;
    Distance len=last-first;
    Distance parent=heap_node_parent(len-1);
    while(true)
    {
        __adjust_heap(first,parent,len,T(*(first+parent)));
        if(0==parent) return ;
        --parent;
    }
}

///.....................由用户指定元素比较运算.......................///

template<typename RandomAccessIterator,
        typename Distance,typename T,
        typename BinaryPredicate>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
                 Distance topIndex, T value,BinaryPredicate BinPredicate);

template<typename RandomAccessIterator ,typename Distance,typename T,typename BinaryPredicate>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last,
                            Distance*,
                            T*,BinaryPredicate BinPredicate);

template<typename RandomAccessIterator,typename BinaryPredicate>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last,BinaryPredicate BinPredicate)
{
    __push_heap_aux(first,last,distance_type(first),value_type(first),BinPredicate);
}
template<typename RandomAccessIterator ,typename Distance,typename T,typename BinaryPredicate>
inline void __push_heap_aux(RandomAccessIterator first,
                            RandomAccessIterator last,
                            Distance*,
                            T*,BinaryPredicate BinPredicate)
{
    __push_heap(first,Distance((last-first)-1),Distance(0),
                T(*(last-1)),BinPredicate);
}
template<typename RandomAccessIterator,typename Distance,typename T,typename BinaryPredicate>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
                 Distance topIndex, T value,BinaryPredicate BinPredicate)
{
    Distance parent=heap_node_parent(holeIndex);
    while(holeIndex>topIndex&&BinPredicate(*(first+parent),value))
    {
        *(first+holeIndex)=*(first +parent);
        holeIndex=parent;
        parent=heap_node_parent(parent);
    }
    *(first+holeIndex)=value;
}

template<typename RandomAccessIterator,typename T,typename Distance,typename BinaryPredicate>
inline void __pop_heap(RandomAccessIterator first,
                       RandomAccessIterator last,
                       RandomAccessIterator result,
                       T value, Distance*,BinaryPredicate BinPredicate);

template<typename RandomAccessIterator, typename T,typename BinaryPredicate>
inline void __pop_heap_aux(RandomAccessIterator first,
                           RandomAccessIterator last,T*,BinaryPredicate BinPredicate);

template<typename RandomAccessIterator ,typename Distance,typename T,typename BinaryPredicate>
inline void __adjust_heap(RandomAccessIterator first,
                          Distance holeIndex,
                          Distance len,
                          T value,BinaryPredicate BinPredicate);

template<typename RandomAccessIterator,typename BinaryPredicate>
inline void pop_heap(RandomAccessIterator first,
                     RandomAccessIterator last,BinaryPredicate BinPredicate)
{
    __pop_heap_aux(first,last,value_type(first),BinPredicate);
}
template<typename RandomAccessIterator, typename T,typename BinaryPredicate>
inline void __pop_heap_aux(RandomAccessIterator first,
                           RandomAccessIterator last,T*,BinaryPredicate BinPredicate)
{
    __pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(first),BinPredicate);
}
template<typename RandomAccessIterator,typename T,typename Distance,typename BinaryPredicate>
inline void __pop_heap(RandomAccessIterator first,
                       RandomAccessIterator last,
                       RandomAccessIterator result,
                       T value, Distance*,BinaryPredicate BinPredicate)
{
    *result=*first;
    __adjust_heap(first,Distance(0),Distance(last-first),value,BinPredicate);
}

template<typename RandomAccessIterator ,typename Distance,typename T,typename BinaryPredicate>
inline void __adjust_heap(RandomAccessIterator first,
                          Distance holeIndex,
                          Distance len,
                          T value,BinaryPredicate BinPredicate)
{
    Distance left,right,largest;
    *(first+holeIndex)=value;
    while(holeIndex<len)
    {
        left=heap_node_left_child(holeIndex);
        right=heap_node_right_child(holeIndex);
        largest=holeIndex;
        if(left<len && BinPredicate(*(first+largest),*(first+left)))
            largest=left;
        if(right<len && BinPredicate(*(first+largest), *(first+right)))
            largest=right;
        if(largest==holeIndex)
            return ;
        else
        {
            *(first+holeIndex)=*(first+largest);
            *(first+largest)=value;
            holeIndex=largest;
        }
    }
}

template<typename RandomAccessIterator,typename BinaryPredicate>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last,BinaryPredicate BinPredicate)
{
    while(last-first>1)
        pop_heap(first,last--);
}

template<typename RandomAccessIterator, typename T, typename Distance,typename BinaryPredicate>
void __make_heap(RandomAccessIterator first,
                 RandomAccessIterator last,
                 T*, Distance*,BinaryPredicate BinPredicate);
template<typename RandomAccessIterator,typename BinaryPredicate>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last,BinaryPredicate BinPredicate)
{
    __make_heap(first,last,value_type(first),distance_type(first),BinPredicate);
}
template<typename RandomAccessIterator, typename T, typename Distance,typename BinaryPredicate>
void __make_heap(RandomAccessIterator first,
                 RandomAccessIterator last,
                 T*, Distance*,
                 BinaryPredicate BinPredicate)
{
    if(last-first<2) return ;
    Distance len=last-first;
    Distance parent=heap_node_parent(len-1);
    while(true)
    {
        __adjust_heap(first,parent,len,T(*(first+parent)));
        if(0==parent) return ;
        --parent;
    }
}
}//end of namespace stdExercise


#endif // STL_HEAP_H_INCLUDED
