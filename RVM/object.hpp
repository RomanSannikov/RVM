#pragma once

#include <concepts>

#include "utils.hpp"

enum ObjectType { INT = 0, DOUBLE, REF };

class BaseObject {
public:
	explicit BaseObject(const ObjectType type) : c_type(type) {}

	ObjectType getType() const { return this->c_type; }

private:
	const ObjectType c_type;
};

template <Arithmetic T>
class Object : public BaseObject {
public:
	explicit Object(const ObjectType type) : BaseObject(type) {}

	void setValue(const T& value) { this->value = value; }
	T getValue() const { return this->value; }

private:
	T value = 0;
};
