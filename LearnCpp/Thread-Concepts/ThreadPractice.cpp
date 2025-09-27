#include <iostream>
#include <thread>

void taskFunction(int id){
    std::cout << "Task " << id << " is running on thread " << std::this_thread::get_id() << std::endl;
}

int main(){
    std::thread t1(taskFunction, 1);
    std::thread t2(taskFunction, 2);
    t1.join();
    t2.join();
    std::cout << "Main thread finished!" << std::endl;
    return 0;
}
