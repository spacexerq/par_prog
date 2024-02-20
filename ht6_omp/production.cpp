#include <omp.h>
#include <thread>
#include <vector>
#include <iostream>


std::vector<int> dotProductOMP(const int& vecLen, const int& vectorsNum){
    std::vector<int> result;
    std::vector<int> vectors;
    vectors.resize(vecLen * vectorsNum);
    double startTime = omp_get_wtime();
#pragma omp parallel shared(vectors)
    {
#pragma omp sections
        {
            //load vectors
#pragma omp section
            {

                for (int i = 0; i < vectorsNum; i ++) {
                    // #pragma omp parallel for
                    {
                        for (int j = 0; j < vecLen; j ++) {
                            vectors[i * vecLen + j] = i + j;
                        }
                    }
                }
            }
#pragma omp section
            {
                //calculate dot product for each pair
                for (int i = 0; i < vectorsNum; i++) {
                    for (int j = 0; j < vectorsNum; j++) {
                        int prod = 0;
                        for (int c = 0; c < vecLen; c++) {
                            prod += vectors[i * vecLen + c] * vectors[j * vecLen + c];
                        }
                        result.push_back(prod);
                    }
                }
            }
        }
    }
    double duration = omp_get_wtime() - startTime;
    std::cout << "OMP elapsed time is " << duration << " seconds" << std::endl;
    return result;
}

std::vector<int> dotProduct(const int& vecLen, const int& vectorsNum){
    std::vector<int> result;
    std::vector<int> vectors;
    vectors.resize(vecLen * vectorsNum);
    double startTime = omp_get_wtime();
    //load vectors
    for (int i = 0; i < vectorsNum; i ++) {
        for (int j = 0; j < vecLen; j ++) {
            vectors[i * vecLen + j] = i + j;
        }

    }
    //calculate dot product for each pair
    for (int i = 0; i < vectorsNum; i++) {
        for (int j = 0; j < vectorsNum; j++) {
            int prod = 0;
            for (int c = 0; c < vecLen; c++) {
                prod += vectors[i * vecLen + c] * vectors[j * vecLen + c];
            }
            result.push_back(prod);
        }
    }
    double duration = omp_get_wtime() - startTime;
    std::cout << "standard elapsed time is " << duration << " seconds" << std::endl;
    return result;
}

double OMPdot2(const size_t& vecSize){
    double startTime = omp_get_wtime();
    long result = 0;
    std::vector<int> vec1, vec2;
    vec1.resize(vecSize);
    vec2.resize(vecSize);
#pragma omp parallel shared(vec1, vec2)
    {
#pragma omp sections
        {
#pragma omp section
            {
                for (size_t i = 0; i < vecSize; i++) {
                    vec1[i] = i;
                    vec2[i] = 2*i;
                }
            }
#pragma omp section
            {
                for (size_t i = 0; i < vecSize; i++) {
                    result += vec1[i] * vec2[i];
                }
            }
        }
    }
    auto dur = omp_get_wtime() - startTime;
    return dur;
}

double linearDot2(const size_t& vecSize){
    double startTime = omp_get_wtime();
    long result = 0;
    std::vector<int> vec1, vec2;
    vec1.resize(vecSize);
    vec2.resize(vecSize);

    for (size_t i = 0; i < vecSize; i++) {
        vec1[i] = i;
        vec2[i] = 2*i;
    }

    for (size_t i = 0; i < vecSize; i++) {
        result += vec1[i] * vec2[i];
    }
    // std::cout << "Linear time is: " << omp_get_wtime() - startTime << " seconds\n";
    auto dur = omp_get_wtime() - startTime;
    return dur;
}

int main(){
    size_t vecLen = 100000;
    omp_set_num_threads(2);
    double ompTime = 0;
    double linTime = 0;
    int expNum = 1000;
    for (int i = 0; i < expNum; i++) {
        ompTime += OMPdot2(vecLen);
        linTime += linearDot2(vecLen);
    }
    ompTime /= expNum;
    linTime /= expNum;
    std::cout << "Linear time is: " << linTime << " seconds\n";
    std::cout << "OMP time is: " << ompTime << " seconds\n";
    return 0;
}