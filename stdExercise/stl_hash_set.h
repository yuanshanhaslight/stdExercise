#ifndef STL_HASH_SET_H_INCLUDED
#define STL_HASH_SET_H_INCLUDED
#include "stl_hashtable.h"

namespace stdExercise
{

template<typename Value, typename HashFcn=hash<Value>,
          typename EqualKey=equal_to<Value>,
          typename Alloc=alloc>
class hash_set
{
private:
    typedef hashtable<Value,Value,HashFcn,Identity<Value>,EqualKey,Alloc> ht;
    ht rep;
public:
    typedef ht::key_type                    key_type;
    typedef ht::value_type                 value_type;
    typedef ht::hasher                      hasher;
    typedef typename ht::key_equal         key_equal

    typedef typename ht::size_type         size_type;
    typedef typename ht::difference_type  difference_type;
    typedef typename ht::const_pointer      pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::const_reference   reference;
    typedef typename ht::const_reference   const_reference;

    typedef typename ht::const_itertor      iterator;
    typedef typename ht::const_iterator    const_iterator;

public:
    hasher hash_funct() const {return rep.hash_funct();}
    key_equal key_eq() const { return rep.key_eq(); }

    hash_set():rep(100,hasher(),key_equal()){}
    explicit hash_set(size_type n): rep(n,hasher(),key_equal()){}
    hash_set(size_type n,const hasher&hf):rep(n,hf,key_equal()){}
    hash_set(size_type n,const hasher&hf,const key_equal&eql)
        :req(n,hf,eql){}

    template<typename InputIterator>
    hash_set(InputIterator first,InputIterator last)
        :ret(100,hasher(),key_equal()) {rep.insert_unique(first,last);}

    template<typename InputIterator>
    hash_set(InputIterator first,InputIterator last,size_type n)
        :ret(n,hasher(),key_equal()) {rep.insert_unique(first,last);}

    template<typename InputIterator>
    hash_set(InputIterator first,InputIterator last,size_type n,const hasher&hf)
        :ret(n,hf,key_equal()) {rep.insert_unique(first,last);}

    template<typename InputIterator>
    hash_set(InputIterator first,InputIterator last,size_type n,
             const hasher&hf,const key_equal&eql)
        :ret(n,hf,eql) {rep.insert_unique(first,last);}

public:
    size_type size() const {return rep.size();}
    size_type max_size() const { return rep.max_size();}
    bool empty() const { return rep.empty() ;}
    void swap(hash_set&hs) { rep.swap(hs.rep); }
    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

public:


};end of hash_set

}//end of namespace stdExercise


#endif // STL_HASH_SET_H_INCLUDED
