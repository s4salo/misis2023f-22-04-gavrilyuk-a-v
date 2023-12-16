#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string x;
    std::cin >> x;

    for (size_t i = 0; i < x.size(); ++i) {
        // Переменная 'digit' представляет текущую цифру в числе.
        int digit = x[i] - '0';

        // Если первая цифра равна 9, то пропускаем ее, чтобы число не начиналось с 0.
        if (i == 0 && digit == 9) {
            continue;
        }

        // Если инверсия делает число меньше, заменяем эту цифру.
        if (9 - digit < digit) {
            x[i] = '0' + (9 - digit);
        }
    }

    std::cout << x << std::endl;

    return 0;
}