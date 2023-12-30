#pragma once

#include <iostream>
#include <concepts>
#include <vector>

template<class T>
concept Integral = std::is_integral<T>::value;

class Logger {
public:
    static void printInstructions(const std::vector<Integral auto>& instructions)
	{
        #ifndef NDEBUG    
        for (const auto& i : instructions) std::cout << std::bitset<8>(i) << std::endl;
        #endif
    }

    static void printStack(const std::vector<Integral auto>& stack)
    {
        #ifndef NDEBUG
        std::cout << std::endl << "STACK: " << std::endl;
        for (auto i : stack) std::cout << std::bitset<8>(i) << std::endl;
        #endif
    }

    static void print(const std::string& str)
    {
        #ifndef NDEBUG
        std::cout << str << std::endl;
        #endif
    }
};