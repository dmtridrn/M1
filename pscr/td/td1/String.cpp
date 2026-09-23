#include "String.hpp"
#include <ostream>
namespace p1{
    String::String(const char* mot):str(mot){}

    std::ostream& operator<<(std::ostream& os, const String& s) {
        return os << s.str;
    }

}

