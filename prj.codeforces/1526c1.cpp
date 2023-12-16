#include <iostream>
#include <vector>
#include <queue>

int main() {
    int n;
    std::cin >> n;

    std::vector<long long> potions(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> potions[i];
    }

    std::priority_queue<long long> negativeHealths;
    long long health = 0;
    int count = 0;

    for (int i = 0; i < n; ++i) {
        // Если текущее здоровье позволяет, выпиваем настойку
        if (health + potions[i] >= 0) {
            health += potions[i];
            count++;
            if (potions[i] < 0) {
                // Если настойка отрицательная, кладем в очередь
                negativeHealths.push(-potions[i]);
            }
        }
        else if (!negativeHealths.empty() && (-potions[i] < negativeHealths.top())) {
            // Если текущая настойка более здоровая, чем самая вредная выпитая ранее,
            // то заменяем их друг на друга
            health += potions[i] + negativeHealths.top();
            negativeHealths.pop();
            negativeHealths.push(-potions[i]);
        }
    }

    std::cout << count << std::endl;
    return 0;
}