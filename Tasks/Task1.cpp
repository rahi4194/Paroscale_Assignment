#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <random>
#include <atomic>

using namespace std;


mutex mtx;
condition_variable cv;
vector<int> dataVector;
atomic<bool> isProducing(true);
const int numItemsToProduce = 10;

void producer(int id) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < numItemsToProduce; ++i) {
        int num = dis(gen);
        {
            lock_guard<mutex> lock(mtx);
            dataVector.push_back(num);
            cout << "Producer " << id << " produced: " << num << endl;

            cv.notify_one(); 
        }
        this_thread::sleep_for(chrono::milliseconds(dis(gen)));
    }

    isProducing.store(false, memory_order_release);
    cv.notify_all();
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [] { return !dataVector.empty() || !isProducing.load(memory_order_acquire); });

        if (!isProducing.load(memory_order_acquire) && dataVector.empty()) {
            break; // No more data to consume
        }

        int num = dataVector.back();
        dataVector.pop_back();
        lock.unlock(); // Unlock before processing to allow producer to continue
        cout << "Consumer consumed: " << num << endl;
        cv.notify_all();
    }
}

int main() {
    const int numProducers = 2;

    vector<thread> producerThreads;

    for (int i = 0; i < numProducers; ++i) {
        producerThreads.emplace_back(producer, i);
    }

    thread consumerThread(consumer);

    for (auto& thread : producerThreads) {
        thread.join();
    }

    consumerThread.join();

    return 0;
}