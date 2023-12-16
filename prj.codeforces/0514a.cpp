#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string x;
    std::cin >> x;

    for (size_t i = 0; i < x.size(); ++i) {
        // ���������� 'digit' ������������ ������� ����� � �����.
        int digit = x[i] - '0';

        // ���� ������ ����� ����� 9, �� ���������� ��, ����� ����� �� ���������� � 0.
        if (i == 0 && digit == 9) {
            continue;
        }

        // ���� �������� ������ ����� ������, �������� ��� �����.
        if (9 - digit < digit) {
            x[i] = '0' + (9 - digit);
        }
    }

    std::cout << x << std::endl;

    return 0;
}