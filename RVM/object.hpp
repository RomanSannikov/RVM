#pragma once

template <typename T>
class Object {
public:
	Object() = default;

	void set(const T& value) { this->value = value; }
	T get() { return this->value; }

private:
	T value;
};