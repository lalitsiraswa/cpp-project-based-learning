#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx1, mtx2;
int value1 = 0, value2 = 0;

void safeIncrement() {
    for(int i = 0; i < 100000; i++){
        std::scoped_lock<std::mutex, std::mutex> lock(mtx1, mtx2); // Optional: <std::mutex, std::mutex>
        value1++;
        value2++;
    }
}

int main(){
    std::thread t1(safeIncrement);
    std::thread t2(safeIncrement);
    
    t1.join();
    t2.join();
    
    std::cout << "Value1 = " << value1 << ", value2 = " << value2 << std::endl;
    return 0;
}
