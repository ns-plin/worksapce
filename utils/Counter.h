#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <memory>

class CounterBase {
public:
    virtual ~CounterBase() {}
    virtual void printAll() const = 0;
};

template <typename T>
class Counter : public CounterBase {
private:
    std::unordered_map<T, int> counts;

public:
    void count(T item) {
        ++counts[item];
    }

    int getCount(T item) const {
        auto it = counts.find(item);
        return it == counts.end() ? 0 : it->second;
    }

    void printAll() const override {
        for (auto& entry : counts) {
            std::cout << "Item: " << entry.first << ", Count: " << entry.second << std::endl;
        }
    }
};

class MultiCounter {
private:
    std::unordered_map<std::string, CounterBase*> type_map;

public:
    template <typename T>
    void count(T item) {
        if (type_map.find(typeid(T).name()) == type_map.end()) {
            type_map[typeid(T).name()] = new Counter<T>;
        }
        static_cast<Counter<T>*>(type_map[typeid(T).name()])->count(item);
    }

    template <typename T>
    int getCount(T item) const {
        auto it = type_map.find(typeid(T).name());
        if (it != type_map.end()) {
            const Counter<T>* counter = static_cast<const Counter<T>*>(it->second);
            return counter->getCount(item);
        }
        return 0;
    }

    void printAll() const {
        std::cout << "MultiCounter Report:" << std::endl;
        for (auto& entry : type_map) {
            std::cout << "Type: " << entry.first << std::endl;
            entry.second->printAll();
        }
    }

    ~MultiCounter() {
        for (auto& entry : type_map) {
            delete entry.second;
        }
    }
};


#if 0
class MultiCounter {
private:
    //std::unordered_map<std::string, void*> type_map;
     std::unordered_map<std::string, std::unique_ptr<void>> type_map;

public:
    template <typename T>
    void count(T item) {
        if (type_map.find(typeid(T).name()) == type_map.end()) {
            type_map[typeid(T).name()] = new Counter<T>;
        }
        static_cast<Counter<T>*>(type_map[typeid(T).name()])->count(item);
    }

    template <typename T>
    int getCount(T item) const {
        if (type_map.find(typeid(T).name()) != type_map.end()) {
            const Counter<T>* counter = static_cast<const Counter<T>*>(type_map.at(typeid(T).name()));
            return counter->getCount(item);
        }
        return 0;
    }

    void printAll() const {
        std::cout << "MultiCounter Report:" << std::endl;
        for (auto& entry : type_map) {
            std::cout << "Type: " << entry.first << std::endl;
            static_cast<const Counter<void*>*>(entry.second)->printAll();
        }
    }

    ~MultiCounter() {
        // std::unordered_map<std::string, void*> type_map;
        //for (auto& entry : type_map) {
        //    delete entry.second;
        //}
    }
};
#endif

#if 0 // usage
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
#endif
