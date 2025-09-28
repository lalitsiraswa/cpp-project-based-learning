#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;
bool done{false};

void producer() {
    for(int i = 1; i <= 5; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
            std::cout << "Produced " << i << std::endl;
        }
        cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_one();
}

void consumer() {
    while (true){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !q.empty() || done;});
        while(!q.empty()){
            std::cout << "Consumed " << q.front() << std::endl;
            q.pop();
        }
        if(done) break;
    }
}

int main(){
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    
    std::cout << "Processing finished!" << std::endl;
    return 0;
}
