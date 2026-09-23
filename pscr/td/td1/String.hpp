#pragma once
#include <cstddef>
namespace p1{
class String
{
private:
    const char* str;
public:
    String(const char* mot);
    ~String();
    size_t length() const;
};
}