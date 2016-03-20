// ���ʣ��ڶ�������������ͷ��ڴ�?

#ifndef STL_ALLOC_H_INCLUDED
#define STL_ALLOC_H_INCLUDED

#if !defined(__HTROW_BAD_ALLOC)
#include <iostream>
#define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl; exit(1)
#endif

#include <cstdlib>

namespace stdExercise
{

//��һ���ռ������������� >128bytes ��С���ڴ�
template<int inst>
class __malloc_alloc_template
{
private:
    //�ڴ治��ʱ�Ĵ�������
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *,size_t);
    static void (*__malloc_alloc_oom_handler)();
public:
    static void *allocate(size_t n)
    {
        void *tmp=malloc(n);
        if(0==tmp)
        {
            tmp=oom_malloc(n);
        }
        return tmp;
    }
    static void deallocate(void *p,size_t/* n */)
    {
        free(p);
    }

    static void *reallocate(void*p,size_t/*old_sz*/,size_t new_sz)
    {
        void *tmp=realloc(p,new_sz);
        if(0==tmp)
        {
            tmp=oom_realloc(p,new_sz);
        }
        return tmp;
    }

    static void (*set_malloc_handler(void(*f)()))()
    {
        void(*old)()=__malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler=f;
        return old;
    }
};

//����ָ�룬ָ���ڴ治��ʱ���õĴ�������
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)()=0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void(*my_alloc_handler)();
    void *result;
    for(;;)
    {
        my_alloc_handler=__malloc_alloc_oom_handler;
        if(0==my_alloc_handler){__THROW_BAD_ALLOC;}
        (*my_alloc_handler)();
        result=malloc(n);
        if(result)return result;
    }
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void *p,size_t n)
{
    void(*my_alloc_handler)();
    void *result;
    for(;;)
    {
        my_alloc_handler=__malloc_alloc_oom_handler;
        if(0==my_alloc_handler){__THROW_BAD_ALLOC;}
        (*my_alloc_handler)();
        result=realloc(p,n);
        if(result)return result;
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

//�ڶ����ռ������������� <=128bytes ��С���ڴ�
enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES /__ALIGN };

template<bool threads,int inst >
class __default_alloc_template
{
private:
    static size_t ROUND_UP(size_t bytes)                //��bytes������8�ı���
    {
        return (bytes+__ALIGN-1)&~(__ALIGN-1);
    }
private:
    union obj                                           //����ڵ㣬���������ڴ�Ƭ��
    {
        obj* free_list_link;
        char client_data[1];
    };
private:
    static obj* volatile free_list[__NFREELISTS];      //����16����������
    static size_t FREELIST_INDEX(size_t bytes)         //����bytes������������ı�ͷָ�������
    {
        return (bytes-1)/__ALIGN;
    }

    static void *refill(size_t n);                     //��������Ŀ����ڴ治��ʱ�����ڴ��Ҫ�ڴ�

    static char *chunk_alloc(size_t size,unsigned int &nobjs);// �����ڴ��

    //�ڴ��״ָ̬ʾ��
    static char *start_free;                            //�ڴ��ʣ��ռ����ʼλ��
    static char *end_free;                              //�ڴ��ʣ��ռ��ĩβ
    static size_t heap_size;                            //�ڴ����������ڴ���ܵĴ�С
public:
    static void *allocate(size_t n);
    static void deallocate(void *p,size_t n);
    static void *reallocate(void *p,size_t old_sz,size_t new_sz);

};//end of template class __default_alloc_template

template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::start_free=0;  //��ʼ���ڴ��״ָ̬ʾ��

template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::end_free=0;    //��ʼ���ڴ��״ָ̬ʾ��

template<bool threads,int inst>
size_t __default_alloc_template<threads,inst>::heap_size=0;   //��ʼ���ڴ��״ָ̬ʾ��

template<bool threads,int inst>
typename __default_alloc_template<threads,inst>::obj * volatile
__default_alloc_template<threads,inst>::free_list[__NFREELISTS]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//�����ڴ棬n�������0
template<bool threads,int inst>
void* __default_alloc_template<threads,inst>::allocate(size_t n)
{
    obj* volatile*my_free_list;
    obj* result;
    if(n>static_cast<size_t>(__MAX_BYTES))                //���������ڴ����128�ֽڣ��͵��õ�һ��
    {                                                       //�ռ�������
        return malloc_alloc::allocate(n);
    }
    my_free_list=free_list+ FREELIST_INDEX(n);
    result=*my_free_list;
    if(0==result)
    {
        void *r=refill(ROUND_UP(n));                       //��������Ŀ����ڴ治�㣬���ڴ��Ҫ�ռ�
        return r;
    }
    *my_free_list=result->free_list_link;
    return result;
}

//�ڴ���գ�ָ��p����Ϊ��
template<bool threads,int inst>
void __default_alloc_template<threads,inst>::deallocate(void *p,size_t n)
{
    obj*q=static_cast<obj*>(p);
    obj* volatile *my_free_list;
    if(n>static_cast<size_t>(__MAX_BYTES))                //���n����128�ֽڣ��͵��õ�һ�������������ڴ�
    {
        malloc_alloc::deallocate(p,n);
        return ;
    }
    my_free_list=free_list + FREELIST_INDEX(n);             //���ڴ����ӵ����������ͷ
    q->free_list_link=*my_free_list;
    *my_free_list=q;
}

template<bool threads,int inst>
void * __default_alloc_template<threads,inst>::reallocate(  //�ڴ��ط���
        void *p,
        size_t old_sz,
        size_t new_sz)
{
    deallocate(p,old_sz);
    void*result=allocate(new_sz);
    return result;
}

template<bool threads,int inst>
void * __default_alloc_template<threads,inst>::refill(size_t n) //��������ʣ��ռ䲻�㣬���ڴ��Ҫ�ڴ�
{
    unsigned int nobjs=20;
    char* chunk=chunk_alloc(n,nobjs);
    obj*volatile *my_free_list;
    obj*result;
    obj* current_obj,*next_obj;
    int i;
    if(1==nobjs)                                                //���ֻ���䵽һ���ڴ�Ƭ����ֱ�ӷ��ظ��û�
    {
        return chunk;
    }
    my_free_list=free_list + FREELIST_INDEX(n);
    result=reinterpret_cast<obj*>(chunk);
    *my_free_list = next_obj = reinterpret_cast<obj*>(chunk+n);
    for(i=1;i<nobjs;++i)                                        //��������ڴ��������������
    {
        current_obj=next_obj;
        next_obj=reinterpret_cast<obj*>(reinterpret_cast<char*>(next_obj)+n);
        current_obj->free_list_link=next_obj;
    }
    current_obj->free_list_link=0;
    return result;
}

template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::
chunk_alloc(size_t size,unsigned int&nobjs)                   //���ڴ���з����ڴ�
{
    char* result;
    size_t total_bytes=size*nobjs;
    size_t bytes_left=end_free-start_free;

    if(bytes_left>=total_bytes)                                //����ڴ��ʣ��ռ乻�ã��Ͱ������
    {
        result=start_free;
        start_free+=total_bytes;
        return result;
    }
    else if(bytes_left>size)                                  //����ڴ��ʣ��ռ����size�����ж��ٷ������
    {
        nobjs=bytes_left/size;
        total_bytes= size*nobjs;
        result= start_free;
        start_free+=total_bytes;
        return result;
    }
    else
    {
        size_t bytes_to_get=2*total_bytes+ROUND_UP(heap_size>>4);
        if(bytes_left>0)                                         //��ʣ��Ŀռ������������
        {
            obj*volatile *my_free_list=free_list + FREELIST_INDEX(bytes_left);
            reinterpret_cast<obj*>(start_free)->free_list_link=*my_free_list;
            *my_free_list=reinterpret_cast<obj*>(start_free);
        }
        start_free=static_cast<char*>(malloc(bytes_to_get));     //��ϵͳҪ�ڴ�
        if(0==start_free)                                         //�ڴ����ʧ�ܣ�������������
        {                                                         //���Ƿ������������ڴ�
            int i;
            obj*volatile*my_free_list,*p;
            for(i=size;i<=__MAX_BYTES;i+=__ALIGN)
            {
                my_free_list=free_list+ FREELIST_INDEX(i);
                p=*my_free_list;
                if(0!=p)
                {
                    *my_free_list=p->free_list_link;
                    start_free=reinterpret_cast<char*>(p);
                    end_free=start_free+i;
                    return (chunk_alloc(size,nobjs));
                }
            }
            end_free=0;                                                           //�����������û��ʣ���ڴ�
            start_free=static_cast<char*>(malloc_alloc::allocate(bytes_to_get)); //�͵��õ�һ���ռ�������
        }
        heap_size+=bytes_to_get;
        end_free=start_free+bytes_to_get;
        return (chunk_alloc(size,nobjs));
    }
}
}//end of namespace stdExercise
#endif // STL_ALLOC_H_INCLUDED
