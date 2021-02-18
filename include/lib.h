#include <type_traits>
#include <utility>

// #include "gcem.hpp"

namespace ego {
    
namespace tags {

struct add {};
struct mul {};

} // ego::tags
    
template<double val>
struct number
{
    static constexpr auto value = val;
};

template<class T, class U, class Op>
struct expression 
{
    using op = Op;
    using lh = T;
    using rh = U;
};

template<unsigned Idx>
struct variable { static constexpr unsigned index = Idx; };

template<class T>
struct is_number : std::false_type {};

template<double N>
struct is_number<number<N>> : std::true_type {};

template<class T>
constexpr bool is_number_v = is_number<T>::value;

template<class T>
struct is_expression : std::false_type {};

template<class T, class U, class Op>
struct is_expression<expression<T, U, Op>> : std::true_type {};

template<class T>
constexpr bool is_expression_v = is_expression<T>::value;

template<class T>
struct is_variable : std::false_type {};

template<unsigned Idx>
struct is_variable<variable<Idx>> : std::true_type {};

template<class T>
constexpr bool is_variable_v = is_variable<T>::value;

template<class T, class Enabler=void>
struct is_function : std::false_type {};

template<template<class> class T, class U>
struct is_function<T<U>, std::void_t<typename T<U>::value_type, typename T<U>::diff_type>> : std::true_type {};

template<class T>
constexpr bool is_function_v = is_function<T>::value;

template<class T> struct cos;
template<class T>
struct sin {
    using value_type = T;
    using diff_type = cos<T>; 
};

template<class T>
struct cos {
    using value_value = T;
    using diff_type = ego::expression<ego::number<-1.>, sin<T>, ego::tags::mul>; 
};

template<class Expr, class Variable, class Enabler = void>
struct diff
{};

template<class Expr, class Variable>
struct diff<Expr, Variable, std::enable_if_t<is_number_v<Expr>>>
{
    using type = number<0.>;
};

template<class Expr, class Variable>
struct diff<Expr, Variable, std::enable_if_t<is_variable_v<Expr>>>
{
    using type = std::conditional_t<std::is_same_v<Expr, Variable>, number<1.0>, number<0.0>>;
};

template<class Expr, class Variable>
struct diff<Expr, Variable, std::enable_if_t<is_expression_v<Expr> && std::is_same_v<typename Expr::op, tags::add>>>
{
    using type = expression<typename diff<typename Expr::lh, Variable>::type, typename diff<typename Expr::rh, Variable>::type, tags::add>;
};
    
template<class Expr, class Variable>
struct diff<Expr, Variable, std::enable_if_t<is_expression_v<Expr> && std::is_same_v<typename Expr::op, tags::mul>>>
{
    using type = expression<
                    expression<typename diff<typename Expr::lh, Variable>::type, typename Expr::rh, tags::mul>
                    , expression<typename Expr::lh, typename diff<typename Expr::rh, Variable>::type, tags::mul>, tags::add>;
};

template<class Expr, class Variable>
struct diff<Expr, Variable, std::enable_if_t<is_function_v<Expr>>>
{
    using type = expression<typename Expr::diff_type, typename diff<typename Expr::value_type, Variable>::type, tags::mul>;
};

template<class Variable, class Pair, class... Pairs>
struct assign_inner
{
    using type = std::conditional_t<std::is_same_v<typename Pair::first_type, Variable>, typename Pair::second_type, typename assign_inner<Variable, Pairs...>::type>;
};

template<class Variable, class Pair>
struct assign_inner<Variable, Pair>
{
    using type = std::conditional_t<std::is_same_v<typename Pair::first_type, Variable>, typename Pair::second_type, Variable>;
};

template<class Expr, class Enabler, class... Pairs>
struct assign;

template<class Expr, class... Pairs>
struct assign<Expr, std::enable_if_t<is_expression_v<Expr>>, Pairs...>
{
    using type = expression<typename assign<typename Expr::lh, void, Pairs...>::type, typename assign<typename Expr::rh, void, Pairs...>::type, typename Expr::op>;
};

template<class Expr, class... Pairs>
struct assign<Expr, std::enable_if_t<is_variable_v<Expr>>, Pairs...>
{
    using type = typename assign_inner<Expr, Pairs...>::type;
};

template<class Expr, class... Pairs>
struct assign<Expr, std::enable_if_t<is_number_v<Expr>>, Pairs...>
{
    using type = Expr;
};

template<class Expr, class Enabler=void>
struct solve
{};

template<class Expr>
struct solve<Expr, std::enable_if_t<is_number_v<Expr>>>
{
    static constexpr auto value = Expr::value;
};

template<class Expr>
struct solve<Expr, std::enable_if_t<std::is_same_v<typename Expr::op, tags::add>>>
{
    static constexpr auto value = solve<typename Expr::lh>::value + solve<typename Expr::rh>::value;
};

template<class Expr>
struct solve<Expr, std::enable_if_t<std::is_same_v<typename Expr::op, tags::mul>>>
{
    static constexpr auto value = solve<typename Expr::lh>::value * solve<typename Expr::rh>::value;
};

} // ego