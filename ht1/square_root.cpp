#include <iostream>
#include <xmmintrin.h>
#include <cmath>
#include <chrono>

using namespace::std;


void default_sqrt(float* a, float* b, int N)
{
    for (int i = 0; i < N; ++i)
        b[i] = sqrt(a[i]);
}

void sse_sqrt(float* a, float* b, int N){
    int i = 0;
    for (i = 0; i + 4 < N; i += 4) // 128/4 = 32 bit
    {
        __m128 x = _mm_load_ps(&a[i]);
        __m128 res = _mm_sqrt_ps((__m128) x);
        _mm_store_ps(&b[i], res);
    }
    for (; i < N; ++i) {
        b[i] = sqrt(a[i]);
    }
}

bool compare(float* a, float* b, int N){
    for(int i = 0; i<N; i++){
        if(a[i]!=b[i]){
            return false;
        }
    }
    return true;
}

int main(){
    int scale_sample = 100004;
    srand(time(0));
    float x[scale_sample], res_sse[scale_sample], res_def[scale_sample];
    for(int i = 0; i<scale_sample;i++){
        x[i]=int(rand()/10e3);
    }
    auto start_default = chrono::steady_clock::now();
    default_sqrt(x,res_def,scale_sample);
    auto end_default = chrono::steady_clock::now();
    auto start_sse = chrono::steady_clock::now();
    sse_sqrt(x,res_sse,scale_sample);
    auto end_sse = chrono::steady_clock::now();
    //cout << scal_pr << endl;
    //cout << scal_pr_sse << endl;
    auto seconds_default = end_default - start_default;
    auto seconds_sse = end_sse - start_sse;
    if(not compare(res_sse, res_def, scale_sample)){
        cout << "Numerical mistake" << endl;
        return -1;
    }else{
        cout << "Calculation complete" << endl;
    }
    cout << "Time spent for " << scale_sample << " vector length is:" << endl;
    cout << "By SSE:" << chrono::duration<double>(seconds_sse).count() << endl;
    cout << "By default:" << chrono::duration<double>(seconds_default).count() << endl;
}