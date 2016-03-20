#ifndef STL_HASHTABLE_H_INCLUDED
#define STL_HASHTABLE_H_INCLUDED
#include "Memory.h"
#include "Algorithm.h"
#include "stl_vector.h"
#include "stl_pair.h"
#include "stl_hash_fun.h"
#include "stl_function.h"

namespace stdExercise
{

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc=alloc>
class hashtable;

///...........hashtable node................///
template<typename Value>
struct __hashtable_node                                                      //hashtable�Ľڵ㶨��
{
    __hashtable_node* next;
    Value val;
};

///..............hashtable iterator......................///
template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_iterator                                                  //hashtable�ĵ���������
{
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            hash_table;

    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            iterator;

//    typedef __hashtable_const_iterator<Value, Key, HashFcn,
//                                        ExtractKey, EqualKey, Alloc>
//            const_iterator;                                                //ָ�����ĵ�������δʵ��

    typedef __hashtable_node<Value> hashtable_node;

    typedef forward_iterator_tag iterator_category;                      //hashtable�ĵ�����������Ϊǰ�������
    typedef Value                  value_type;
    typedef value_type*           pointer;
    typedef const value_type*     const_pointer;
    typedef value_type&           reference;
    typedef const value_type&     const_reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t              difference_type;

    hashtable_node *cur;                                                   //��������ָ��Ľڵ�
    hash_table *ht;                                                        //ָ���������������hashtable

    __hashtable_iterator(hashtable_node* pNode, hash_table*tab):cur(pNode),ht(tab){}
    __hashtable_iterator():cur(0),ht(0){}
    reference operator*() const {return cur->val;}
    pointer operator->()const {return &(operator*());}
    iterator& operator++();                                             //����ǰ�õ��������
    iterator operator++(int);                                           //���غ��õ��������

    bool operator==(const iterator&it)const {return cur==it.cur;}
    bool operator!=(const iterator&it)const {return cur!=it.cur;}
};//end of hashtable iterator

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
{
    const hashtable_node* old=cur;
    cur=cur->next;
    if(!cur)                                                           //���curָ��β�ڵ�
    {                                                                  //�ͽ�cur�ƶ�����һ�������ͷ�ڵ�
        size_type bucket=ht->bkt_num(old->val);
        while(!cur && ++bucket<ht->buckets.size())
            cur=ht->buckets[bucket];
    }
    return *this;
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
{
    iterator tmp=*this;
    ++*this;
    return tmp;
}

///.................................hashtable ........................///
static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes]=                  //������hashtable��buckets��С
{
    53,97,193,389,769,1543,3097,6151,12289,24593,
    49157,98317,196613,393241,786433,
    1572869,3145739,6291469,12582917,25165843,
    50331653,100663319,201326611,402653189,805306457,
    1610612741,3221225473,4294967291
};

inline unsigned long __stl_next_prime(unsigned long n)                       //�������в�С��n�ĵ�һ������
{
    const unsigned long *first=__stl_prime_list;
    const unsigned long *last=__stl_prime_list+__stl_num_primes;
    const unsigned long *pos=lower_bound(first,last,n);
    return pos==last? *(last-1):*pos;
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
class hashtable                                                               //hashtable����
{
public:
    typedef HashFcn     hasher;                                              //hash�������ͱ�������
    typedef EqualKey    key_equal;                                           //�жϹؼ����Ƿ���ȵ�ν�ʵ����ͱ�������
    typedef Key         key_type;                                            //�ؼ�����ȡ�������ͱ�������

    typedef Value               value_type;                                 //ֵ���ͱ�������
    typedef value_type*        pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
            iterator;
    friend class __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
private:
    hasher hash;
    key_equal equals;
    ExtractKey get_key;

    typedef __hashtable_node<value_type> node;                          //hashtable�Ľ�㶨��
    typedef simple_alloc<node,Alloc> node_allocator;                     //�ռ�������

    vector<node*,Alloc> buckets;                                         //��vector����������hashtable��bucket
    size_type num_elements;                                              //hashtable��Ԫ�صĸ���

protected:
    node* new_node(const value_type&obj);                               //����һ�����
    void delete_node(node*n);                                            //ɾ��һ�����
    void initialize_buckets(size_type);                                  //��ʼ��bucket
    size_type next_size(size_type n)const {return __stl_next_prime(n);}//��ȡ��һ��buckets�Ĵ�С
    void resize(size_type);
    pair<iterator,bool> insert_unique_noresize(const value_type&obj); //��unique��ʽ����һ����㣬����ɹ���buckets�Ĵ�С����
    iterator insert_equal_noresize(const value_type&obj);              //��equal��ʽ����һ����㣬����ɹ���buckets�Ĵ�С����

    size_type bkt_num(const value_type&obj,size_t n)const             //����hashtable��СΪnʱobj��hashֵ
    {
        return bkt_num_key(get_key(obj),n);
    }
    size_type bkt_num(const value_type&obj) const                     //���㵱ǰhashtable��С��obj��hashֵ
    {
        return bkt_num_key(get_key(obj));
    }
    size_type bkt_num_key(const key_type&key)const                     //���㵱ǰhashtable��С�Ĺؼ���key��hashֵ
    {
        return bkt_num_key(key,buckets.size());
    }
    size_type bkt_num_key(const key_type&key,size_t n)const           //����hashtable��СΪnʱ�ؼ���key��hashֵ
    {
        return hash(key) % n;
    }

    void clear();
    void copy_from(const hashtable&ht);
public:                                                                //hashtableû��Ĭ�Ϲ��캯��
    hashtable(const hashtable&ht);
    hashtable& operator=(const hashtable&ht);
    ~hashtable();
    hashtable(size_type n,const HashFcn&hf,const EqualKey&eql)
        :hash(hf),equals(eql),get_key(ExtractKey()),num_elements(0)
    {
        initialize_buckets(n);
    }

public:
    size_type bucket_count() const {return buckets.size();}
    size_type size()const {return num_elements;}
    size_type max_bucket_count() const{
        return __stl_prime_list[__stl_num_primes-1];
    }

    pair<iterator,bool> insert_unique(const value_type&obj)
    {
        resize(num_elements+1);
        return insert_unique_noresize(obj);
    }
    iterator insert_equal(const value_type&obj)
    {
        resize(num_elements+1);
        return insert_equal_noresize(obj);
    }
    iterator begin()
    {
        return iterator(*buckets.begin(),this);
    }
//    const_iterator begin()const                                 //const_iterator ��δʵ��
//    {
//        return const_iterator(*buckets.begin(),this);
//    }
    iterator end()
    {
        return iterator(nullptr,this);
    }
//    const_iterator end() const                                 //const_iterator ��δʵ��
//    {
//        return const_iterator(nullptr,this);
//    }

    size_type elems_in_bucket(size_type n)
    {
        node*first=buckets[n];
        size_type cnt=0;
        while(first)
        {
            first=first->next;
            ++cnt;
        }
        return cnt;
    }

    iterator find(const value_type&obj) ;
    size_type count(const value_type&obj);

};//end of hashtable

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::hashtable(const hashtable&ht)
        :hash(ht.hash),equals(ht.equals),get_key(ht.get_key),num_elements(0)
{
    initialize_buckets(ht.size());
    copy_from(ht);
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>&
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator=(const hashtable&ht)
{
    if(this!=&ht)
        copy_from(ht);
    return *this;
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::~hashtable()
{
    clear();
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node*
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::new_node(const value_type&obj)
{
    node *pNode=node_allocator::allocate();
    pNode->next=0;
    try
    {
        construct(&pNode->val,obj);
        return pNode;
    }
    catch(...)
    {
        node_allocator::deallocate(pNode);
        throw;
    }
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::delete_node(node*pNode)
{
    destroy(&pNode->val);
    node_allocator::deallocate(pNode);
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::initialize_buckets(size_type n)
{
    const size_type n_buckets=next_size(n);
    buckets.reserve(n_buckets);
    buckets.insert(buckets.end(),n_buckets,static_cast<node*>(0));
    num_elements=0;
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
{
    //���Ԫ�ظ�����������Ԫ�غ󣩴���bucket vector �Ĵ�С�����ؽ����
    const size_type old_size=buckets.size();
    if(num_elements_hint>old_size)
    {
        const size_type new_size=next_size(num_elements_hint);
        if(new_size>old_size)
        {
            vector<node*,Alloc> tmp(new_size,static_cast<node*>(0));
            try
            {
                for(size_type bucket=0;bucket<old_size;++bucket)
                {
                    node*first=buckets[bucket];
                    while(first)
                    {
                        size_type new_bucket=bkt_num(first->val,new_size);
                        buckets[bucket]=first->next;
                        first->next=tmp[new_bucket];
                        tmp[new_bucket]=first;
                        first=buckets[bucket];
                    }
                }
            }
            catch(...)
            {
                //�˴��쳣�����޷�����commit or rollback
                throw;
            }
            buckets.swap(tmp);
        }
    }
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type&obj)
{
    size_type bucket=bkt_num(obj);
    node*first=buckets[bucket];
    for(node*cur=first;cur;cur=cur->next)
    {
        if(equals(get_key(cur->val),get_key(obj)))
            return pair<iterator,bool>(iterator(cur,this),false);
    }
    node*pNode=new_node(obj);
    pNode->next=first;
    buckets[bucket]=pNode;
    ++num_elements;
    return pair<iterator,bool>(iterator(pNode,this),true);
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type&obj)
{
    size_type bucket=bkt_num(obj);
    node*first=buckets[bucket];
    for(node*cur=first;cur;cur=cur->next)
    {
        if(equals(get_key(cur->val),get_key(obj)))
        {
            node*pNode=new_node(obj);
            pNode->next=cur->next;
            cur->next=pNode;
            ++num_elements;
            return iterator(pNode,this);
        }
    }
    node*pNode=new_node(obj);
    pNode->next=first;
    buckets[bucket]=pNode;
    ++num_elements;
    return iterator(pNode,this);
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear()
{
    for(size_type bucket=0;bucket!=buckets.size();++bucket)
    {
        node*cur=buckets[bucket];
        node*next=cur->next;
        while(cur)
        {
            delete_node(cur);
            cur=next;
            next=next->next;
        }
        buckets[bucket]=0;
    }
    num_elements=0;
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
void
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable&ht)
{
    //���buckets ��Ԥ����Ŀ��hashtable��buckets.size��С�Ŀռ�
    buckets.clear();
    buckets.reserve(ht.bucket_count());

    buckets.insert(buckets.end(),ht.bucket_count(),static_cast<node*>(0));

    //��������ڵ�
    try
    {
        for(size_type i=0;i<ht.buckets.size();++i)
        {
            if(const node*cur=ht.buckets[i])
            {
                node*pNode=new_node(cur->val);
                buckets[i]=pNode;
                for(node*next=cur->next;next;next=next->next)
                {
                    pNode->next=new_node(next->val);
                    pNode=pNode->next;
                }
            }
        }
        num_elements=ht.num_elements;
    }
    catch(...)
    {
        clear();
        throw;
    }
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const value_type&obj)
{
    node* first=buckets[bkt_num(obj)];
    while(first!=nullptr&&!equals(get_key(first->val),get_key(obj)))
        first=first->next;
    return iterator(first,this);
}

template<typename Value, typename Key,typename HashFcn,
          typename ExtractKey, typename EqualKey, typename Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::count(const value_type&obj)
{
    size_type cnt=0;
    for(node* first=buckets[bkt_num(obj)];first!=nullptr;first=first->next)
    {
        if(equals(get_key(first->val),get_key(obj)))
            ++cnt;
    }
    return cnt;
}

}//end of namespace stdExercise
#endif // STL_HASHTABLE_H_INCLUDED
