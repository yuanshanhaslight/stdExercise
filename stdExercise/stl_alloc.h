// 疑问：第二级配置器如何释放内存?

#ifndef STL_ALLOC_H_INCLUDED
#define STL_ALLOC_H_INCLUDED

#if !defined(__HTROW_BAD_ALLOC)
#include <iostream>
#define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl; exit(1)
#endif

#include <cstdlib>

namespace stdExercise
{

//第一级空间配置器，分配 >128bytes 大小的内存
template<int inst>
class __malloc_alloc_template
{
private:
    //内存不足时的处理例程
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

//函数指针，指向内存不足时调用的处理例程
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

//第二级空间配置器，分配 <=128bytes 大小的内存
enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES /__ALIGN };

template<bool threads,int inst >
class __default_alloc_template
{
private:
    static size_t ROUND_UP(size_t bytes)                //将bytes提升至8的倍数
    {
        return (bytes+__ALIGN-1)&~(__ALIGN-1);
    }
private:
    union obj                                           //链表节点，用来连接内存片段
    {
        obj* free_list_link;
        char client_data[1];
    };
private:
    static obj* volatile free_list[__NFREELISTS];      //保存16个自由链表
    static size_t FREELIST_INDEX(size_t bytes)         //计算bytes所属自由链表的表头指针的索引
    {
        return (bytes-1)/__ALIGN;
    }

    static void *refill(size_t n);                     //自由链表的空余内存不足时，向内存池要内存

    static char *chunk_alloc(size_t size,unsigned int &nobjs);// 分配内存块

    //内存池状态指示器
    static char *start_free;                            //内存池剩余空间的起始位置
    static char *end_free;                              //内存池剩余空间的末尾
    static size_t heap_size;                            //内存池所分配的内存的总的大小
public:
    static void *allocate(size_t n);
    static void deallocate(void *p,size_t n);
    static void *reallocate(void *p,size_t old_sz,size_t new_sz);

};//end of template class __default_alloc_template

template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::start_free=0;  //初始化内存池状态指示器

template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::end_free=0;    //初始化内存池状态指示器

template<bool threads,int inst>
size_t __default_alloc_template<threads,inst>::heap_size=0;   //初始化内存池状态指示器

template<bool threads,int inst>
typename __default_alloc_template<threads,inst>::obj * volatile
__default_alloc_template<threads,inst>::free_list[__NFREELISTS]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//分配内存，n必须大于0
template<bool threads,int inst>
void* __default_alloc_template<threads,inst>::allocate(size_t n)
{
    obj* volatile*my_free_list;
    obj* result;
    if(n>static_cast<size_t>(__MAX_BYTES))                //如果分配的内存大于128字节，就调用第一级
    {                                                       //空间配置器
        return malloc_alloc::allocate(n);
    }
    my_free_list=free_list+ FREELIST_INDEX(n);
    result=*my_free_list;
    if(0==result)
    {
        void *r=refill(ROUND_UP(n));                       //自由链表的空余内存不足，向内存池要空间
        return r;
    }
    *my_free_list=result->free_list_link;
    return result;
}

//内存回收，指针p不能为空
template<bool threads,int inst>
void __default_alloc_template<threads,inst>::deallocate(void *p,size_t n)
{
    obj*q=static_cast<obj*>(p);
    obj* volatile *my_free_list;
    if(n>static_cast<size_t>(__MAX_BYTES))                //如果n大于128字节，就调用第一级配置器回收内存
    {
        malloc_alloc::deallocate(p,n);
        return ;
    }
    my_free_list=free_list + FREELIST_INDEX(n);             //将内存块添加到自由链表表头
    q->free_list_link=*my_free_list;
    *my_free_list=q;
}

template<bool threads,int inst>
void * __default_alloc_template<threads,inst>::reallocate(  //内存重分配
        void *p,
        size_t old_sz,
        size_t new_sz)
{
    deallocate(p,old_sz);
    void*result=allocate(new_sz);
    return result;
}

template<bool threads,int inst>
void * __default_alloc_template<threads,inst>::refill(size_t n) //自由链表剩余空间不足，向内存池要内存
{
    unsigned int nobjs=20;
    char* chunk=chunk_alloc(n,nobjs);
    obj*volatile *my_free_list;
    obj*result;
    obj* current_obj,*next_obj;
    int i;
    if(1==nobjs)                                                //如果只分配到一个内存片，就直接返回给用户
    {
        return chunk;
    }
    my_free_list=free_list + FREELIST_INDEX(n);
    result=reinterpret_cast<obj*>(chunk);
    *my_free_list = next_obj = reinterpret_cast<obj*>(chunk+n);
    for(i=1;i<nobjs;++i)                                        //将多余的内存放入自由链表中
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
chunk_alloc(size_t size,unsigned int&nobjs)                   //从内存池中分配内存
{
    char* result;
    size_t total_bytes=size*nobjs;
    size_t bytes_left=end_free-start_free;

    if(bytes_left>=total_bytes)                                //如果内存池剩余空间够用，就按需分配
    {
        result=start_free;
        start_free+=total_bytes;
        return result;
    }
    else if(bytes_left>size)                                  //如果内存池剩余空间大于size，就有多少分配多少
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
        if(bytes_left>0)                                         //将剩余的空间放入自由链表
        {
            obj*volatile *my_free_list=free_list + FREELIST_INDEX(bytes_left);
            reinterpret_cast<obj*>(start_free)->free_list_link=*my_free_list;
            *my_free_list=reinterpret_cast<obj*>(start_free);
        }
        start_free=static_cast<char*>(malloc(bytes_to_get));     //向系统要内存
        if(0==start_free)                                         //内存分配失败，则检查自由链表
        {                                                         //看是否还有其他空余内存
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
            end_free=0;                                                           //如果自由链表没有剩余内存
            start_free=static_cast<char*>(malloc_alloc::allocate(bytes_to_get)); //就调用第一级空间配置器
        }
        heap_size+=bytes_to_get;
        end_free=start_free+bytes_to_get;
        return (chunk_alloc(size,nobjs));
    }
}
}//end of namespace stdExercise
#endif // STL_ALLOC_H_INCLUDED
