#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

int main() {
    int n;
    std::cin >> n;

    std::unordered_map<int, long long> cnt;
    int max_value = 0;
    for (int i = 0; i < n; ++i) {
        int ai;
        std::cin >> ai;
        ++cnt[ai];
        max_value = std::max(max_value, ai);
    }

    std::vector<long long> dp(max_value + 1, 0);

    dp[1] = 1LL * cnt[1];
    for (int i = 2; i <= max_value; ++i) {

        dp[i] = std::max(dp[i - 1], dp[i - 2] + 1LL * i * cnt[i]);
    }
    std::cout << dp[max_value] << std::endl;

    return 0;
}