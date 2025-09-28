#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;
bool done{false};

void producer(int id, int count){
    for(int i = 1; i <= count; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*id));
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i + (id * 1000));
            std::cout << "[Producer " << id << "] Produced " << (i + id * 1000) << std::endl;
        }
        cv.notify_one(); // Wake one waiting consumer
    }
    // Last producer signals completion later (see main)
}

void consumer(int id){
    while(true){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {return !q.empty() || done;});
        while(!q.empty()){
            int item = q.front();
            q.pop();
            lock.unlock(); // Release lock while processing (important!)
            std::cout << "    [Consumer " << id << "] Consumed " << item << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate processing
            lock.lock(); // Re-lock to check queue again
        }
        if(done && q.empty()){
            break;
        }
    }
}

int main(){
    const int numProducers = 3;
    const int numConsumers = 2;
    const int itemsPerProducer = 5;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    // Launch multiple consumers
    for(int i = 1; i <= numConsumers; i++){
        consumers.emplace_back(consumer, i);
    }
    
    // Launch multiple producers
    for(int i = 1; i <= numProducers; i++){
        producers.emplace_back(producer, i, itemsPerProducer);
    }
    
    // Wait for all producers to finish
    for(auto& p : producers){
        p.join();
    }
    
    // Signal to consumers that prodution is done
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    
    // Wake up all consumers so they can exit
    cv.notify_all();
    
    // Wait for all consumers to finish
    for(auto& c : consumers){
        c.join();
    }
    
    std::cout << "All work completed!" << std::endl;
    return 0;
}
