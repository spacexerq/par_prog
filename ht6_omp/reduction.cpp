#include <iostream>
#include <omp.h>
#include <vector>
#include <numeric>
#include <cassert>

template<typename T>
T reductionAtomic(const std::vector<T>& vec){
    T sum = 0;
    double startTime = omp_get_wtime();
#pragma parallel shared(vec)
    {
        for (int i = 0; i < vec.size(); i++) {
#pragma atomic
            {
                sum += vec[i];
            }
        }
    }
    double endTime = omp_get_wtime();
    std::cout << "Atomic elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

template<typename T>
T reductionCritical(const std::vector<T>& vec){
    T sum = 0;
    double startTime = omp_get_wtime();
#pragma omp parallel shared(vec)
    {
        T localSum = 0;
#pragma omp for
        {
            for (int i = 0; i < vec.size(); i++) {
                localSum += vec[i];
            }
        }
#pragma omp critical
        {
            sum += localSum;
        }
    }
    double endTime = omp_get_wtime();
    std::cout << "Critical elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

template<typename T>
T reductionLock(const std::vector<T>& vec){
    T sum = 0;
    double startTime = omp_get_wtime();
    omp_lock_t lock;
    omp_init_lock(&lock);
#pragma omp parallel shared(vec)
    {
        T localSum = 0;
#pragma omp for
        {
            for (int i = 0; i < vec.size(); i++) {
                localSum += vec[i];
            }
        }
        omp_set_lock(&lock);
        sum += localSum;
        omp_unset_lock(&lock);
    }
    double endTime = omp_get_wtime();
    std::cout << "Lock elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

template <typename T>
T reductionFor(const std::vector<T>& vec){
    T sum = 0;
    double startTime = omp_get_wtime();
#pragma parallel for shared(vec) reduction(+ : sum)
    {
        for (int i = 0; i < vec.size(); i++) {
            sum += vec[i];
        }
    }
    double endTime = omp_get_wtime();
    std::cout << "For elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

template<typename T>
T naiveSum(const std::vector<T>& vec){
    T sum = 0;
    double startTime = omp_get_wtime();
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    double endTime = omp_get_wtime();
    std::cout << "Naive sum elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return sum;
}

int main(){
    int num_threads = 2;
    omp_set_num_threads(num_threads);
    std::vector<int> vec(100000000);
    std::iota(vec.begin(), vec.end(), 1);
    auto atomic = reductionAtomic(vec);
    auto critical = reductionCritical(vec);
    auto lock = reductionLock(vec);
    auto For = reductionFor(vec);
    auto naive = naiveSum(vec);
    return 0;
}