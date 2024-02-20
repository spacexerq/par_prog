#include <stdio.h>
#include <omp.h>
#include <iostream>


using namespace::std;

int main() {
    int n = 10;
//    omp_set_num_threads(n);
    #pragma omp parallel
    {
        int i = omp_get_thread_num();
        printf_s("Greetings from thread %d\n", i);
//        cout << 10 << 25 << 20 << "\n";
    }
    return 0;
}