#include <iostream> 
#include <string>

int main() {
	int weight;
	int k = 0;
	std::cin >> weight;
	for (int i = 1; i < weight; i++) {
		if (((weight - i) % 2 == 0) && (i % 2 == 0)) {
			k++;
		}
	}
	if (k > 0) {
		std::cout << "YES";
	}
	else {
		std::cout << "NO";
	}
	return 0;
}
