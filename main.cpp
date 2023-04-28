#include <iostream>
#include <string>
#include <vector>

#include "zod.hpp"

using int0to64 = zod<int>::min<0>::max<64>;
using string10 = zod<std::string>::max<10>;
using email = zod<std::string>::email;
using email10 = zod<std::string>::email::max<10>;
using url = zod<std::string>::url;


int main() {
    constexpr int0to64 v = 50;
    constexpr std::size_t n = string10("10").value.size();
    email eml = "test@example.co.jp";
    email10 e2 = "test@co.jp";
    url u = "https://github.com";

    std::cout << v.value << std::endl;
}