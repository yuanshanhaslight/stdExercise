#ifndef STL_QUEUE_H_INCLUDED
#define STL_QUEUE_H_INCLUDED
#include "Memory.h"
#include "stl_deque.h"
namespace stdExercise
{
template<typename T, typename Sequence=deque<T>>
class queue
{
    template<typename ,typename >
    friend bool operator==(const queue&,const queue&);
    template<typename ,typename >
    friend bool operator<(const queue&,const queue&);
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
    reference front() {return seq.front();}
    const_reference front()const {return seq.front();}
    reference back() {return seq.back();}
    const_reference back() const {return seq.back();}

    void push(const value_type&value){seq.push_back(value);}
    void pop(){seq.pop_front();}
};//end of container adapter queue

template<typename T,typename Sequence>
bool operator==(const queue<T,Sequence>&lhs,const queue<T,Sequence>&rhs)
{
    return lhs.seq==rhs.seq;
}
template<typename T, typename Sequence>
bool operator< (const queue<T,Sequence>&lhs,const queue<T,Sequence>&rhs)
{
    return lhs.seq<rhs.seq;
}

}//end of namespace stdExercise


#endif // STL_QUEUE_H_INCLUDED
