#include <iostream>
#include <string_view>

int main(int argc, char *argv[])
{
    using namespace std::literals;
    std::string_view s1 = "";
    std::string_view s2 = ""sv;
    std::cout<<"s1=\"\", and s2=\"\"sv"<<std::endl;
    std::cout<<"s1 "<< s1.compare(s2) <<" s2"<<std::endl;
    std::cout<<"is s1 empty?"<<s1.empty()<<std::endl;
    std::cout<<"is s2 empty?"<<s2.empty()<<std::endl;
    return 0;

}
