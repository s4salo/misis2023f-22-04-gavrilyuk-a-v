#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

int main() {
    int t;
    std::cin >> t;

    for (int tc = 0; tc < t; ++tc) {
        int n;
        std::cin >> n;

        std::unordered_map<std::string, int> words_count; // карта для подсчета вхождений слов
        std::vector<std::vector<std::string>> words(3, std::vector<std::string>(n));  // список слов каждого игрока

        for (int player = 0; player < 3; ++player) {
            for (int j = 0; j < n; ++j) {
                std::cin >> words[player][j];
                ++words_count[words[player][j]];
            }
        }

        std::vector<int> scores(3);

        for (int player = 0; player < 3; ++player) {
            for (const std::string& word : words[player]) {
                switch (words_count[word]) {
                case 1: scores[player] += 3; break;
                case 2: ++scores[player];    break;
                case 3: break; 
                }
            }
        }

        for (const auto score : scores) {
            std::cout << score << ' ';
        }

        std::cout << '\n';
    }

    return 0;
}
