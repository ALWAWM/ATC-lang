#include <iostream>
#include <vector>
#include <string>
template<typename T>
T add_func(T a,T b){
    return a + b;
}
int main(int args,char* argv[]){
    auto add2 = add_func(std::stoi(std::string(1,'9')),2);
    std::cout<<add2;
    return 0;
}
