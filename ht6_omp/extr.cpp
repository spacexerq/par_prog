#include <numeric>
#include <omp.h>
#include <iostream>
#include <vector>

template<typename T>
T findMax(const std::vector<T>& vec){
    T maxVal = vec[0];
    double startTime = omp_get_wtime();
#pragma omp parallel for shared(vec)
    {
        for (int i = 0; i < vec.size(); i++) {
#pragma omp critical
            {
                if (maxVal < vec[i]) {
                    maxVal = vec[i];
                }
            }
        }
    }
    double endTime = omp_get_wtime();
    std::cout << "Elapsed time is " << endTime - startTime << " seconds" << std::endl;
    return maxVal;
}

int main(){
    int num_threads = 10;
    omp_set_num_threads(num_threads);
    std::vector<int> vec(10000000);
    std::iota(vec.begin(), vec.end(), 1);
    int maxVal = findMax(vec);
    std::cout << maxVal << std::endl;
    return 0;
}