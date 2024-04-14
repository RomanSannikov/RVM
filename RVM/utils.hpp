#pragma once

#include <concepts>

template<class T>
concept Integral = std::is_integral<T>::value;

template<class T>
concept Arithmetic = std::is_arithmetic<T>::value;

using stackType = int16_t;
