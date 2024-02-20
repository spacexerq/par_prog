#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>


using namespace::std;

class Message {
public:
    void writer(int id) {
        for (int i = 0; i < 5; ++i) {
            unique_lock<shared_mutex> lock(mutex_);
            pool_.push_back(id * 10 + i);
            cout << "Writer " << id << " wrote " << (id * 10 + i) << endl;
            lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    void reader(int id) {
        for (int i = 0; i < 5; ++i) {
            shared_lock<shared_mutex> lock(mutex_);
            if (pool_.empty()) {
                cout << "No values" << endl;
            }
            else {
                int value = pool_.back();
                cout << "Reader " << id << " read " << value << endl;
                pool_.pop_back();
            }
            lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

private:
    vector<int> pool_;
    shared_mutex mutex_;
};

int main() {
    Message msg;
    vector<thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(&Message::writer, &msg, i);
    }
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(&Message::reader, &msg, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}