#pragma once
#include "OutStream.h"
#include <iostream>

namespace engine::core {
    class ConsoleOut : public OutStream {
    public:
        void write(const std::string& str) override {
            std::cout << str;
        }
    };
}