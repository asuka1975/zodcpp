#include <iostream>

#include "zod.hpp"

using int0to64 = zod<int>::min<0>::max<64>;
using string20 = zod<std::string>::max<20>;
using email = zod<std::string>::email;
using email10 = zod<std::string>::email::max<10>;

int main() {
    int0to64 v = 60;
    string20 s = "12345678901";
    email eml = "test@example.co.jp";
    email10 e2 = "test@co.jp";
    std::cout << v.value << std::endl;

    std::cout << "Hello, world" << std::endl;
}