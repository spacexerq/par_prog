#include <Windows.h>
#include <cstdlib>
#include <iostream>
#include <synchapi.h>
#include <unistd.h>

using namespace std;

template <typename T>
class Set
{
public:
    bool add(T value);
    bool remove(T value);
    bool contains(T value);
};

int main(){
    int num_rows, num_cols;
    cout << "Number of rows:";
    cin >> num_rows;
    cout << "Number of columns:";
    cin >> num_cols;

    HANDLE CreateThread{

    };
}
