#ifndef STL_FUNCTION_H_INCLUDED
#define STL_FUNCTION_H_INCLUDED

namespace stdExercise
{

template<typename Arg,typename Result>
struct unary_function
{
    typedef Arg argument_type;
    typedef Result result_type;
};

template<typename Arg1,typename Arg2,typename Result>
struct binary_function
{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

///...............................Relational................///
template<typename T>
struct equal_to:public binary_function<T,T,bool>
{
    bool operator() (const T&lhs,const T&rhs)const
    {
        return lhs==rhs;
    }
};

///...........identity select project ..................///
template<typename T>
struct identity:public unary_function<T,T>
{
    const T& operator() (const T&t)const {return t;}
};

}//end of namespace stdExercise
#endif // STL_FUNCTION_H_INCLUDED
