#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace::std;


class Message{

public:
    void set(const int &val){
        lock_guard<mutex> lock(mtx);
        pool.push_back(val);
        cout << "Set " << val << " from " << this_thread::get_id() << endl;
    }

    int get(){
        lock_guard<mutex> lock(mtx);
        if (pool.empty()) {
            return -1;
        }

        int val = this->pool.back();
        pool.pop_back();
        cout << "Got " << val << " from " << this_thread::get_id() << endl;
        return val;
    }

private:
    mutex mtx;
    vector<int> pool;
};

Message pack;

int main(){

    thread t1([](){pack.set(1);});
    thread t2([](){pack.get();});

    t1.join();
    t2.join();
    t2.detach();
    t1.detach();
    t2.join();
    return 0;
}