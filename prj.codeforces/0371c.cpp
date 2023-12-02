#include <iostream>
#include <string>
#include <algorithm>


long long howMuchToSpend(long long hamburgers, long long b_count, long long s_count, long long c_count,
    long long nb, long long ns, long long nc, long long pb, long long ps, long long pc) {
    return std::max(0LL, (b_count * hamburgers - nb) * pb)
        + std::max(0LL, (s_count * hamburgers - ns) * ps)
        + std::max(0LL, (c_count * hamburgers - nc) * pc);
}

int main() {
    std::string recipe;
    std::cin >> recipe;

    // Считаем количество каждого ингредиента в рецепте
    long long b_count = count(recipe.begin(), recipe.end(), 'B');
    long long s_count = count(recipe.begin(), recipe.end(), 'S');
    long long c_count = count(recipe.begin(), recipe.end(), 'C');

    // Ингредиенты и цены в магазине
    long long nb, ns, nc, pb, ps, pc;
    std::cin >> nb >> ns >> nc >> pb >> ps >> pc;

    // Деньги, которые может потратить Поликарп
    long long r;
    std::cin >> r;

    // Найдем максимальное количество гамбургеров
    long long left = 0;
    long long right = 1e13;
    long long result = 0;

    while (left <= right) {
        long long mid = left + (right - left) / 2;
        long long cost = howMuchToSpend(mid, b_count, s_count, c_count, nb, ns, nc, pb, ps, pc);
        if (cost <= r) {
            result = mid;
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    std::cout << result << std::endl;

    return 0;
}