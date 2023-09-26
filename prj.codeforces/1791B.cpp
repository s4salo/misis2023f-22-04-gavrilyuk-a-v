#include <iostream>
#include <string>

class Point {
public:
    int x;
    int y;
};


int main() {
    Point alex;
    int t;
    std::cin >> t;
    std::string s;
    for (int i = 0; i < t; i++) {
        int n;
        std::cin >> n;
        bool gotCandy = false;
        alex.x = 0;
        alex.y = 0;
        std::cin >> s;
        
        for (int j = 0; j < n; j++) {
       
            if (s[j] == 'L') alex.x--;
            else if (s[j] == 'R') alex.x++;
            else if (s[j] == 'U') alex.y++;
            else if (s[j] == 'D') alex.y--;
            
            if (alex.x==1 && alex.y==1) {
                std::cout << "YES\n";
                gotCandy = true;
                break;
            }
        }
        if (gotCandy == false) std::cout << "NO\n";

    }

    return 0;
}
