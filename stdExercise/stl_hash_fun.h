#ifndef STL_HASH_FUN_H_INCLUDED
#define STL_HASH_FUN_H_INCLUDED

namespace stdExercise
{
template<typename Key> struct hash{};

inline size_t __stl_hash_string(const char*s)
{
    unsigned long h=0;
    for(;*s;++s)
        h=5*h+*s;
    return static_cast<size_t>(h);
}

template<>
struct hash<char*>
{
    size_t operator()(const char*s)const{return __stl_hash_string(s);}
};

template<>
struct hash<const char*>
{
    size_t operator()(const char*s) const{return __stl_hash_string(s);}
};

template<>
struct hash<char>
{
    size_t operator()(char c) const{return static_cast<size_t>(c);}
};

template<>
struct hash<unsigned char>
{
    size_t operator()(unsigned char c) const{return static_cast<size_t>(c);}
};

template<>
struct hash<signed char>
{
    size_t operator()(unsigned char c) const{return static_cast<size_t>(c);}
};

template<>
struct hash<short>
{
    size_t operator()(short s)const{return static_cast<size_t>(s);}
};

template<>
struct hash<unsigned short>
{
    size_t operator()(unsigned short us)const{return static_cast<size_t>(us);}
};

template<>
struct hash<int>
{
    size_t operator()(int i)const{return static_cast<int>(i);}
};

template<>
struct hash<unsigned int>
{
    size_t operator()(unsigned int ui)const{return static_cast<size_t>(ui);}
};

template<>
struct hash<long>
{
    size_t operator()(long l)const{return static_cast<size_t>(l);}
};

template<>
struct hash<unsigned long>
{
    size_t operator()(unsigned long ul)const{return static_cast<size_t>(ul);}
};
}//end of namespace stdExercise

#endif // STL_HASH_FUN_H_INCLUDED
