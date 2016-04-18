#include "ExtInt/ExtInt"
#include <iostream>

ExtInt a, b;

int main() {
    std::cin >> a >> b;
    auto c = a / b;
    std::cout << c << std::endl;
    return 0;
}
