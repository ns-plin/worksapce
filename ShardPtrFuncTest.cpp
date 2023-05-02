#include <iostream>
#include <memory>

#if 0 // correct
#include <iostream>
#include <memory>

class Foo {
public:
    Foo() { std::cout << "Foo created" << std::endl; }
    ~Foo() { std::cout << "Foo destroyed" << std::endl; }
};

void doSomething() {
    std::shared_ptr<Foo> p(new Foo);
    std::cout << "p's use count is " << p.use_count() << std::endl;
    {
        std::shared_ptr<Foo> q = p;
        std::cout << "p's use count is " << p.use_count() << std::endl;
    }
    std::cout << "p's use count is " << p.use_count() << std::endl;
    p.get();
}

int main() {
    doSomething();
    return 0;
}

#else //incorrect

#include <iostream>
#include <memory>

class Foo {
public:
    Foo() { std::cout << "Foo created" << std::endl; }
    ~Foo() { std::cout << "Foo destroyed" << std::endl; }
};

int main() {
    Foo* ptr = new Foo();   // 創建一個 Foo 對象，並得到其裸指針
    std::shared_ptr<Foo> p1(ptr);  // 創建一個 shared_ptr 對象 p1，管理對 ptr 的引用計數
    std::shared_ptr<Foo> p2(ptr);  // 創建另一個 shared_ptr 對象 p2，同樣管理對 ptr 的引用計數
    std::cout << "p1's use count is " << p1.use_count() << std::endl;  // 此時，p1 和 p2 的引用計數都為 1
    std::cout << "p2's use count is " << p2.use_count() << std::endl;
    return 0;
}

#endif
