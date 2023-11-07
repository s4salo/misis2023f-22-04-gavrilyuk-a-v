#include <iostream>
#include <vector>

struct Vector {
    int x, y, z;
};

int main() {
    int n;
    std::cin >> n;

    std::vector<Vector> forces(n);

    for (int i = 0; i < n; ++i)
        std::cin >> forces[i].x >> forces[i].y >> forces[i].z;

    int sum_x = 0, sum_y = 0, sum_z = 0;
    for (int i = 0; i < n; ++i) {
        sum_x += forces[i].x;
        sum_y += forces[i].y;
        sum_z += forces[i].z;
    }

    if (sum_x == 0 && sum_y == 0 && sum_z == 0)
        std::cout << "YES" << std::endl; 
    else
        std::cout << "NO" << std::endl;  
    return 0;
}