#include <iostream>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <cmath>
#include <chrono>

using namespace::std;
/*using this guide*/
/*http://ccfit.nsu.ru/~kireev/lab4/lab4intr.htm*/


float dot_sse(float* x, float* y, int& len) {
    float inner_prod = 0;
    __m128 xn, yn;
    __m128 prod = _mm_setzero_ps();
    float temp[4];
    int i = 0;
    for (i = 0; i + 4 < len; i += 4) // 128/4 = 32 bit
    {
        xn = _mm_loadu_ps(x+i);
        yn = _mm_loadu_ps(y+i);
        __m128 temp_sum = _mm_mul_ps(xn, yn);
        prod = _mm_add_ps(prod, temp_sum);
    }
    _mm_storeu_ps(&temp[0], prod);
    inner_prod = temp[0] + temp[1] + temp[2] + temp[3];

    for (; i < len; ++i) {
        inner_prod += x[i] * y[i];
    }
    return inner_prod;
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
    int scale_sample = 100000;
    srand(time(0));
    float x[scale_sample], y[scale_sample];
    for(int i = 0; i<scale_sample;i++){
        x[i]=int(rand()/10e3);
        y[i]=int(rand()/10e3);
    }
    auto start_default = chrono::steady_clock::now();
    float scal_pr = dot(x,y,scale_sample);
    auto end_default = chrono::steady_clock::now();
    auto start_sse = chrono::steady_clock::now();
    float scal_pr_sse = dot_sse(x,y,scale_sample);
    auto end_sse = chrono::steady_clock::now();
    //cout << scal_pr << endl;
    //cout << scal_pr_sse << endl;
    auto seconds_default = end_default - start_default;
    auto seconds_sse = end_sse - start_sse;
    if(not compare(scal_pr, scal_pr_sse)){
        cout << "Numerical mistake" << endl;
        return -1;
    }else{
        cout << "Production complete" << endl;
    }
    cout << "Time spent for " << scale_sample << " vector length is:" << endl;
    cout << "By SSE:" << chrono::duration<double>(seconds_sse).count() << endl;
    cout << "By default:" << chrono::duration<double>(seconds_default).count() << endl;
}