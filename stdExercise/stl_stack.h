#ifndef STL_STACK_H_INCLUDED
#define STL_STACK_H_INCLUDED
#include "Memory.h"
#include"stl_deque.h"
namespace stdExercise
{
template<typename T, typename Sequence=deque<T>>
class stack
{
    template<typename ,typename >
    friend bool operator==(const stack&,const stack&);
    template<typename ,typename >
    friend bool operator<(const stack&,const stack&);
public:
    typedef typename Sequence::value_type   value_type;
    typedef typename Sequence::size_type    size_type;
    typedef typename Sequence::reference    reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence seq;
public:
    bool empty()const{return seq.empty();}
    size_type size()const{return seq.size();}
    reference top(){return seq.back();}
    const_reference top()const {return seq.back();}
    void push(const value_type&value){seq.push_back(value);}
    void pop(){seq.pop_back();}
};//end of container adapter stack

template<typename T,typename Sequence>
bool operator==(const stack<T,Sequence>&lhs,const stack<T,Sequence>&rhs)
{
    return lhs.seq==rhs.seq;
}
template<typename T, typename Sequence>
bool operator< (const stack<T,Sequence>&lhs,const stack<T,Sequence>&rhs)
{
    return lhs.seq<rhs.seq;
}
}//end of namespace stdExercise

#endif // STL_STACK_H_INCLUDED
