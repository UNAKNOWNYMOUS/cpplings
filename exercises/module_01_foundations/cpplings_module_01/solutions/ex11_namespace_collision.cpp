#include <iostream>

namespace physics {
    double gravity = 9.81;
}

namespace chemistry {
    double gravity = 6.674e-11;
}

int main() {
    std::cout << physics::gravity << "\n";
    std::cout << chemistry::gravity << "\n";
    return 0;
}
