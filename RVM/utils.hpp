#pragma once

#include <concepts>
#include <vector>

template<class T>
concept Integral = std::is_integral<T>::value;

template<class T>
concept Arithmetic = std::is_arithmetic<T>::value;

using stackType = int16_t;

template <typename T>
class Stack : private std::vector<T>
{
public:
	auto operator-(int value) const { if (std::vector<T>::size() - value < 0) throw ExecutorError("Access to a value out of range"); return std::vector<T>::size() - value; }
	void operator--() { std::vector<T>::pop_back(); }
	void operator--(int value) { if (value < 1) throw ExecutorError("Incorrect value passed to stack decrement"); for(int i = 0; i < value; i++) std::vector<T>::pop_back(); }
	auto& operator+=(auto& value) { std::vector<T>::push_back(value); return std::vector<T>::back(); }
	const auto& operator+=(const auto& value) { std::vector<T>::push_back(value); return std::vector<T>::back(); }
	constexpr auto& operator[](int value) { return std::vector<T>::at(value >= 0 ? value : std::vector<T>::size() + value); }
	constexpr const auto& operator[](int value) const { return std::vector<T>::at(value >= 0 ? value : std::vector<T>::size() + value); }

	auto size() const noexcept { return std::vector<T>::size(); };
	auto begin() noexcept { return std::vector<T>::begin(); }
	auto begin() const noexcept { return std::vector<T>::begin(); }
	auto cbegin() const noexcept { return std::vector<T>::cbegin(); }
	constexpr auto crbegin() const noexcept { return std::vector<T>::crbegin(); }
	auto end() noexcept { return std::vector<T>::end(); }
	auto end() const noexcept { return std::vector<T>::end(); }
	constexpr auto cend() const noexcept { return std::vector<T>::cend(); }
	constexpr auto crend() const noexcept { return std::vector<T>::crend(); }

	friend class std::back_insert_iterator<Stack<T>>;
};
