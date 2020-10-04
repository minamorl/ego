#include <boost/xpressive/xpressive.hpp>

#include "lib.h"

std::string greeting::greeting() {
    using namespace boost::xpressive;

    std::string s = "Hello, world!";
    sregex rex = (s1= +_w) >> ", " >> (s2= +_w) >> "!";
    smatch what;
    if (regex_match(s, what, rex)) {
        return what[1];
    }
    return "Fatal.";
}