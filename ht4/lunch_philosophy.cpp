#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <array>
#include <vector>

std::mutex cout_message;

class Fork {
public:
    Fork (){};
private:
    std::mutex mutex_;
    friend class Philosopher;
};

class Philosopher {
public:
    Philosopher(int id, Fork& left_fork, Fork& right_fork) : id_(id), left_fork_(left_fork), right_fork_(right_fork) {}

    void dine() {
        while (true) {
            think();
            eat();
        }
    }

private:
    int id_;
    Fork& left_fork_;
    Fork& right_fork_;

    void think() {
        {
            std::lock_guard<std::mutex> lock(cout_message);
            std::cout << "Philosopher " << id_ << " is thinking" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    void eat() {
        // std::cout << "Philosopher " << id_ << " is hungry and trying to grab forks" << std::endl;
        std::scoped_lock lock(left_fork_.mutex_, right_fork_.mutex_);
        {
            std::lock_guard<std::mutex> lock(cout_message);
            std::cout << "Philosopher " << id_ << " is eating" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            std::cout << "Philosopher " << id_ << " has finished eating" << std::endl;
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

int main() {
    const int num_philosophers = 5;
    std::array<Fork, num_philosophers> forks;
    std::thread threads[num_philosophers];

    // create philosophers
    for (int i = 0; i < num_philosophers; ++i) {
        int left_fork_idx = i;
        int right_fork_idx = (i + 1) % num_philosophers;
        threads[i] = std::thread([i, &forks]() {Philosopher(i, forks[i], forks[(i + 1) % num_philosophers]).dine();});
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}