#include <iostream>
#include <tbb_loc/tbb.h>
int main() {
    tbb::parallel_invoke(
            []() { std::cout << " Hello " << std::endl; },
            []() { std::cout << " TBB! " << std::endl; }
    );
    return 0;
} 