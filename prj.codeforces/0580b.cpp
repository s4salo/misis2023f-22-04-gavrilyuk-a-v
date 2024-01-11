#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n, d;
    std::cin >> n >> d;
    std::vector<std::pair<int, int>> friends(n);
    for (int i = 0; i < n; i++) {
        std::cin >> friends[i].first >> friends[i].second;
    }

    std::sort(friends.begin(), friends.end());

    long long maxFriendship = 0;
    long long currentFriendship = 0;
    int left = 0;

    for (int right = 0; right < n; right++) {
        currentFriendship += friends[right].second;
        // ѕока разница в доходе между текущим другом и самым бедным в группе слишком велика, исключаем последнего из подсчета
        while (friends[right].first - friends[left].first >= d) {
            currentFriendship -= friends[left].second;
            left++;
        }
        maxFriendship = std::max(maxFriendship, currentFriendship);
    }

    std::cout << maxFriendship << std::endl;
    return 0;
}