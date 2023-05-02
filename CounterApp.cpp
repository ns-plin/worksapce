#include<utils/Counter.h>

int main() {
    Counter<int> intCounter;
    intCounter.count(1);
    intCounter.count(2);
    intCounter.count(2);
    intCounter.count(3);
    intCounter.printAll(); // 输出：1: 1, 2: 2, 3: 1

    Counter<std::string> stringCounter;
    stringCounter.count("hello");
    stringCounter.count("world");
    stringCounter.count("world");
    stringCounter.count("world");
    stringCounter.count("goodbye");
    stringCounter.printAll(); // 输出：goodbye: 1, hello: 1, world: 3

    MultiCounter mc;

    mc.count<int>(1);
    mc.count<int>(2);
    mc.count<double>(3.14);
    mc.count<std::string>("hello");
    mc.count<std::string>("world");
    mc.count<char>('a');

    mc.printAll();

    return 0;
}
