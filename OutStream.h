#pragma once
#include <string>
#include <sstream>

namespace engine::core {
    class OutStream {
    public:
        virtual ~OutStream() = default;

        template<typename T>
        OutStream& operator<<(const T& val) {
            write(val);
            return *this;
        }

        virtual void write(const std::string& str) = 0;

    protected:
        template<typename T>
        void write(const T& val) {
            std::ostringstream oss;
            oss << val;
            write(oss.str());
        }
    };
}