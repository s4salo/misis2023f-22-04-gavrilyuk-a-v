#include <iostream>
#include <cmath>

int main() {
    double n, m, a;
    std::cin >> n >> m >> a;

    long long tiles = ceil(n / a) * ceil(m / a);

    std::cout << tiles;

    return 0;
}