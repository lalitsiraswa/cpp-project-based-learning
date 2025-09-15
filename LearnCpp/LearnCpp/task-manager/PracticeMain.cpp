#include <iostream>
#include <fstream>
#include <sstream>
int main(){
    std::ifstream in("todo.txt");
    if(!in.is_open()){
        return 1;
    }
    std::string line;
    while (std::getline(in, line)){
        std::cout << line << std::endl;
    }
    in.close();
    std::ofstream out("todo.txt", std::ios::trunc);
    out << "1|Breakfast|Medium|1\n";
    out << "2|Lunch|Medium|1\n";
    out << "3|Dinner|Medium|1\n";
    out << "4|Sleep|Medium|0\n";
    out.close();
    std::istringstream iss("1|Breakfast|Medium|1|2|Lunch|Medium|1|3|Dinner|Medium|1|4|Sleep|Medium|0");
    std::string part;
    while(std::getline(iss, part, '|')){
        std::cout << part << " " << std::endl;
    }
    
    std::ostringstream oss;
    oss << "1|Breakfast|Medium|1";
    std::cout << oss.str() << std::endl;
    return 0;
}
