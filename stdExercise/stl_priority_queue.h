#ifndef STL_PRIORITY_QUEUE_H_INCLUDED
#define STL_PRIORITY_QUEUE_H_INCLUDED
#include "stl_vector.h"
#include "stl_heap.h"
#include "Algorithm.h"

namespace stdExercise
{
template<typename T,typename Sequence=vector<T>,
        typename Compare=less<typename Sequence::value_type>>
class priority_queue
{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence seq;
    Compare comp;
public:
    priority_queue():seq(){}
    explicit priority_queue(const Compare&x):seq(),comp(x){}

    template<typename InputIterator>
    priority_queue(InputIterator first,InputIterator last, const Compare&x)
        :seq(first,last),comp(x)
    {
        make_heap(seq.begin(),seq.end(),comp);
    }
    template<typename InputIterator>
    priority_queue(InputIterator first,InputIterator last)
        :seq(first,last)
    {
        make_heap(seq.begin(),seq.end(),comp);
    }

    bool empty()const {return seq.empty();}
    size_type size()const {return seq.size();}
    const_reference top() const {return seq.front();}
    void push(const value_type&value)
    {
        //此处应有异常处理 commit or rollback
        seq.push_back(value);
        push_heap(seq.begin(),seq.end,comp);
    }
    void pop()
    {
        //此处应有异常处理 commit or rollback
        pop_heap(seq.begin(),seq.end(),comp);
        seq.pop_back();
    }
};//end of class priority queue
}//end of namespace stdExercise


#endif // STL_PRIORITY_QUEUE_H_INCLUDED
