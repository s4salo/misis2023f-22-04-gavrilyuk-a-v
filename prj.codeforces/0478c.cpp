#include <iostream>
#include <algorithm>

int main() {
    long long a, b, c;
    std::cin >> a >> b >> c;
    std::cout << std::min((a + b + c) / 3, std::min(a + b, std::min(b + c, a + c)));
}
