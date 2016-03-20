#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#include "stl_alloc.h"
#include "stl_construct.h"
#include <climits>
#include "stl_uninitialized.h"

namespace stdExercise
{
# ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<0,0> alloc;
#endif //! __USE_MALLOC

template<typename T,typename Alloc=alloc>
class simple_alloc
{
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
public:
    static pointer allocate(size_type n)
    {
        return 0 == n ? 0:static_cast<pointer>(alloc::allocate(n*sizeof(value_type)));
    }
    static pointer allocate()
    {
        return static_cast<pointer >(alloc::allocate(sizeof(value_type)));
    }
    static void deallocate(pointer p,size_type n)
    {
        if(0!=n)
            alloc::deallocate(p,n*sizeof(value_type));
    }
    static void deallocate(pointer p)
    {
        alloc::deallocate(p,sizeof(value_type));
    }
    static void construct(pointer p,const_reference value)
    {
        construct(p,value);
    }
    static void destroy(pointer p)
    {
        destroy(p);
    }
    static pointer address(reference x)
    {
        return static_cast<pointer>(&x);
    }
    static const_pointer const_address(const_reference x)
    {
        return static_cast<const_pointer>(&x);
    }
    size_type max_size() const
    {
        return static_cast<size_type>(UINT_MAX/sizeof(value_type));
    }
};//end of template class simple_alloc
}//end of stdExercise
#endif // MEMORY_H_INCLUDED
