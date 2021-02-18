#include<type_traits>

#include "lib.h"

int main() {
    using x1 = ego::variable<1>;
    
    static_assert(std::is_same_v<
        typename ego::diff<ego::expression<x1, x1, ego::tags::add>, x1>::type
        , ego::expression<ego::number<1.>, ego::number<1.>, ego::tags::add>
    >);
    
    static_assert(std::is_same_v<
        typename ego::diff<ego::expression<x1, x1, ego::tags::mul>, x1>::type
        , ego::expression<
            ego::expression<ego::number<1.>, x1, ego::tags::mul>
            , ego::expression<x1, ego::number<1.>, ego::tags::mul>
            , ego::tags::add>>);
    
    
    static_assert(std::is_same_v<ego::diff<ego::number<1.>, x1>::type, ego::number<0.>>);
    
    using expr1 = ego::expression<ego::number<1.>, ego::number<2.>, ego::tags::add>;
    static_assert(std::is_same_v<
                    typename ego::assign<
                        ego::expression<ego::number<1.>, x1, ego::tags::add>
                        , void , std::pair<x1, ego::number<2.>>>::type
                    , expr1>);
    
    static_assert(ego::solve<expr1>::value == 3.);
    
    using expr2 = ego::expression<ego::number<5.>, ego::expression<ego::number<2.>, ego::number<3.>, ego::tags::add>, ego::tags::mul>;

    static_assert(ego::solve<expr2>::value == 25.);

    using expr3 = ego::sin<x1>;
    using expr4 = ego::expression<ego::cos<x1>, ego::number<1.>, ego::tags::mul>;
    static_assert(std::is_same_v<expr4, typename ego::diff<expr3, x1>::type>);
}