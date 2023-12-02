#include <iostream>
#include <vector>
#include <algorithm>

int main() {

    int n, t;
    std::cin >> n >> t;

    std::vector<int> books(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> books[i];
    }


    int left = 0, right = 0;
    int current_time = 0, max_books = 0;


    while (right < n) {
        // если текущая книга может быть прочитана, добавляем её время к общему времени
        if (current_time + books[right] <= t) {
            current_time += books[right];
            ++right;
        }
        else {
            // если книга не помещается в свободное время, убираем время чтения левой книги и сдвигаем левый указатель
            if (right > left) {
                current_time -= books[left];
                ++left;
            }
            else {
                // если книга сама по себе больше свободного времени, просто смещаем оба указателя
                ++left;
                ++right;
            }
        }

        max_books = std::max(max_books, right - left);
    }

    std::cout << max_books;
    return 0;
}