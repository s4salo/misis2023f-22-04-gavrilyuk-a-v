#include <iostream>
#include <string>

int main() {
    int t;
    std::cin >> t;

    for (int _ = 0; _ < t; ++_) {
        int n;
        std::cin >> n;
        std::string sn = std::to_string(n);

        int first_digit = sn[0] - '0';
        
        int length_of_number = sn.length();

        int created_number = 0;
        for (int j = 0; j < length_of_number; ++j) {
            created_number = 10 * created_number + first_digit;
        }

        if (created_number <= n) {
            std::cout << (length_of_number - 2) * 9 + first_digit + 9 << std::endl;
        }
        else {
            std::cout << (length_of_number - 2) * 9 + first_digit + 8 << std::endl;
        }
    }

    return 0;
}
