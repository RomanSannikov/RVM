#include "instruction.hpp"


template <typename T> T add(const T& a, const T& b) { return b + a; }

template <typename T> T sub(const T& a, const T& b) { return b - a; }

template <typename T> T mul(const T& a, const T& b) { return b * a;  }

template <typename T> T div(const T& a, const T& b) { return b / a;  }

// Todo: loading and saving

template <typename T> T eq(const T& a, const T& b) { return a == b; }

template <typename T> void jmp(const T& newLoc, T& currentLoc) { currentLoc = newLoc; }

// Todo: jumps

template <typename T> T gr(const T& a, const T& b) { return b > a; }

template <typename T> T ls(const T& a, const T& b) { return b < a; }

template <typename T> T op_and(const T& a, const T& b) { return b and a; }

template <typename T> T op_or(const T& a, const T& b) { return b or a; }

template <typename T> T op_nand(const T& a, const T& b) { return ~(b and a); }

template <typename T> T op_xor(const T& a, const T& b) { return b xor a; }

template <typename T> T op_not(const T& a) { return ~a; }

template <typename T> void pushn(std::vector<T>& stack, const T& a) { stack.push_back(); }

template <typename T> void pushs(std::vector<T>& stack, const const std::string& str) 
{ for (const auto& i : str) stack.push_back(i); }

template <typename T> void popn(std::vector<T>& stack) { stack.pop_back(); }

template <typename T> void pops(std::vector<T>& stack)
{ for (const auto& i = stack.end(); *i != 0; --i) stack.push_back(i); }

// Todo: allocations