#include <iostream>

#include <type_traits>
#include <utility>



struct MyClass {
  void function() {}
};


int main() {
  // Проверка
  std::cout << has_function<MyClass>::value << '\n';
}