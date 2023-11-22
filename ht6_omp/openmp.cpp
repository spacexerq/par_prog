#include <stdio.h>

using namespace::std;

int main() {
    #pragma omp parallel
        {
            int i = 1;
            printf_s("Hello from thread %d\n", i);
        }
}