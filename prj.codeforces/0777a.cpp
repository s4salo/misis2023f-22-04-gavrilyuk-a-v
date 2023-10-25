#include<iostream>
#include <vector>
int main() {
    int n, x;
    std::cin >> n >> x;
    std::vector<int> b(3);
    b[x] = 1;

    for (int i = n % 6; i > 0; i--) {
        if (i % 2 == 1) {
            std::swap(b[0], b[1]);
        }
        else {
            std::swap(b[1], b[2]);
        }
    }

    int ans;
    if (b[0] == 1) {
        ans = 0;
    }
    else if (b[1] == 1) {
        ans = 1;
    }
    else {
        ans = 2;
    }

    std::cout << ans << "\n";
    return 0;
}
