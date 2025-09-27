#include <iostream>
#include <thread>

int main(){
    std::thread worker([] {
        std::cout << "Hello from lambda thread!" << std::endl;
    });
    worker.join();
    return 0;
}
