#include <iostream>
#include <string>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <cmath>

using namespace::std;
/*using this guide*/
/*http://ccfit.nsu.ru/~kireev/lab4/lab4intr.htm*/


float dot_sse(const float* x, const float* y, const long& len) {
    float inner_prod = 0;
    __m128 xn, yn; //Declare two variables stored in the 128-bit special register of SSE
    __m128 acc = _mm_setzero_ps(); // Declare a variable stored in a 128-bit special register in SSE to store the result of X+Y, the initial value is 0
    float temp[4];//Store the parameters of the intermediate variables

    long i;
    for (i = 0; i + 4 <
                len; i += 4) {//128-bit special register, which can handle 4 groups of 32-bit variable operations at one time
        xn = _mm_loadu_ps(x +
                          i); // Load x to X (because 128 bits can store four 32-bit data, the default is to load 4 consecutive parameters at a time)
        yn = _mm_loadu_ps(y + i);//Same as above
        acc = _mm_add_ps(acc, _mm_mul_ps(xn,
                                         yn));//x*y, the sum of x1*y1 in each round, the sum of x2*y2, the sum of x3*y3, the sum of x4*y4, and the final result The four sums are placed in the 128-bit register of acc.
    }
    _mm_storeu_ps(&temp[0], acc); // Load 4 32-bit data in acc into memory
    inner_prod = temp[0] + temp[1] + temp[2] + temp[3];//dot multiplication sum

    // Just processed the dot multiplication of elements that are multiples of the first 4. If len is not a multiple of 4, then there is a little tail to deal with
    for (; i < len; ++i) {
        inner_prod += x[i] * y[i];//Continue to accumulate the product of the little tail
    }
    return inner_prod;//You're done
}

float dot(float* x, float* y, int n)
{
    float s = 0;
    for(int i=0;i<n;i++)
        s+=x[i]*y[i];
    return s;
}

bool compare(float a, float b){
    float err = 10e-8;
    if(abs(a-b)<err){
        return true;
    }else{
        return false;
    }
}

int main(){
    int scale_sample = 10000;
    srand(time(0));
    float x[scale_sample], y[scale_sample];
    for(int i = 0; i<scale_sample;i++){
        x[i]=int(rand()/10e3);
        y[i]=int(rand()/10e3);
    }
    float scal_pr = dot(x,y,scale_sample);
    float scal_pr_sse = dot_sse(x,y,scale_sample);
    cout << scal_pr << endl;
    cout << scal_pr_sse << endl;
    if(compare(scal_pr, scal_pr_sse)){
        cout << "Numerical mistake";
    }else{
        cout << "Production complete";
    }
}