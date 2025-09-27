#include <iostream>
#include <stdexcept>
#include <vector>

double safeDivide(double a, double b) {
    if(b == 0.0){
        throw std::invalid_argument("Division by zero not allowed!");
    }
    return a / b;
}

int main (){
    try {
        double x = 10.0;
        double y = 0.0;
        double result = safeDivide(x, y);
        std::cout << "Result = " << result << std::endl;
    }
    catch (const std::invalid_argument& e){
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
    catch (...){
        std::cerr << "Caught unknown exception!" << std::endl;
    }
    std::cout << "Program continues after handling exception!\n";
    return 0;
}
