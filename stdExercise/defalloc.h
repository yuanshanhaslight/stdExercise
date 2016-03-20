#ifndef DEFALLOC_H_INCLUDED
#define DEFALLOC_H_INCLUDED
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

template<typename T>
inline T* allocate(ptrdiff_t size ,T*)
{
    std::set_new_handler(0);
    T*tmp=static_cast<T*>(::operator new(static_cast<size_t>(size*sizeof(T))));
    if(tmp==0)
    {
        std::cerr<<"out of memory"<<std::endl;
        exit(1);
    }
    return tmp;
}

template<typename T>
void deallocate(T*buffer)
{
    ::operator delete(buffer);
}

template<typename T>
class allocator
{
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    pointer allocate(size_type n)
    {
        return ::allocate(static_cast<difference_type>(n),static_cast<pointer>(0));
    }
    void deallocate(pointer p)
    {
        ::deallocate(p);
    }
    pointer address(reference x)
    {
        return static_cast<pointer>(&x);
    }
    const_pointer address(const_reference x)
    {
        return static_cast<const_pointer>(&x);
    }
    size_type init_page_size()
    {
        return std::max(static_cast<size_type>(1),
                         static_cast<size_type>(4096/sizeof(T)));
    }
    size_type max_size()const
    {
        return std::max(static_cast<size_type>(1),
                        static_cast<size_type>(UINT_MAX/sizeof(T)));
    }

};

#endif // DEFALLOC_H_INCLUDED
