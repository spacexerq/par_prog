#include <iostream>
#include <string>
#include <vector>
#include <xmmintrin.h>
#include <math.h>

using namespace::std;


void normal(float* a, float* b, int N)
{
    for (int i = 0; i < N; ++i)
        b[i] = sqrt(a[i]);
}

int main(){

}