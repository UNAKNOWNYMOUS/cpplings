#include <climits>

int main() {
    int arr[5] = {1, 2, 3, 4, 5};

    int a = arr[2];      // Case 1
    int b = arr[10];     // Case 2

    int x = INT_MAX;
    int y = x + 1;       // Case 3

    int* p = nullptr;
    // *p = 42;          // Case 4

    int denominator = 0;
    int z = 10 / denominator; // Case 5

    return 0;
}
