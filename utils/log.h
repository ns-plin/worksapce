#include <iostream>
#include <sstream>

enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

class LOG {
public:
    LOG(LogLevel level) : m_level(level) {}

    ~LOG() {
        std::cout << m_stream.str() << std::endl;
    }

    template <typename T>
    LOG& operator<<(const T& data) {
        m_stream << data;
        return *this;
    }

private:
    LogLevel m_level;
    std::stringstream m_stream;
};

#define LOG(level) LOG(LogLevel::level) << __FILE__ << "::" << __LINE__ << ":[" << #level << "] "

