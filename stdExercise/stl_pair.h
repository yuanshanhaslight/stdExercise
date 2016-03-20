//简单的pair的实现，只是为了测试hashtable
#ifndef STL_PAIR_H_INCLUDED
#define STL_PAIR_H_INCLUDED
#include "stl_iterator.h"

#include <assert.h>

namespace stdExercise
{

template<typename FirstType,typename SecondType>
struct pair
{
    pair():first(),second(){}
    pair(const FirstType&ft,const SecondType&st)
        :first(ft),second(st){}

    FirstType first;
    SecondType second;

};// end of pair
}//end of namespace stdExercise
#endif // STL_PAIR_H_INCLUDED
