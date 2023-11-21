#include <iostream>
#include <string>

bool check(int x) {
    return (x % 8 == 0);
}

int main() {
    std::string s;
    std::cin >> s;
    int n = s.length();

    for (int i = 0; i < n; ++i) {

        if (check(s[i] - '0')) {
            std::cout << "YES" << std::endl;
            std::cout << s[i] << std::endl;
            return 0;
        }
        for (int j = i + 1; j < n; ++j) {

            int num2 = (s[i] - '0') * 10 + (s[j] - '0');
            if (check(num2)) {
                std::cout << "YES" << std::endl;
                std::cout << num2 << std::endl;
                return 0;
            }
            for (int k = j + 1; k < n; ++k) {

                int num3 = (s[i] - '0') * 100 + (s[j] - '0') * 10 + (s[k] - '0');
                if (check(num3)) {
                    std::cout << "YES" << std::endl;
                    std::cout << num3 << std::endl;
                    return 0;
                }
            }
        }
    }

    std::cout << "NO" << std::endl;
    return 0;
}