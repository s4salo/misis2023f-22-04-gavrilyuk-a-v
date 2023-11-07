#include <iostream>
using namespace std;
int main() {
    int n, m, ans = 0;
    cin >> n >> m;
    while (n < m) {
        if (m % (n * 2) == 0)
            n *= 2;
        else n *= 3;
        ans++;
    }
    cout << (n == m ? ans : -1);
}