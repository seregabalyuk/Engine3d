#include <iostream>

void f(float&) {
  std::cout << "non const\n"; 
}

void f(const float&) {
  std::cout << "const\n"; 
}

template<class T>
const T& to_const(const T& val) {
  return val;
}

int main() {
  f(0.f);
  float t = 10;
  f(t);
  f(to_const(t));
  f(to_const(0.f));
}