#pragma once

#include <iostream>

size_t countMul = 0;
size_t countSum = 0;

template<class T>
struct Checker {
 // members
  T value;
 // function to string
  static std::string nameType() {
    return std::string("Checker<") + typeid(T).name() + ">";
  } 
  std::string name() const {
    return nameType() + "(" + std::to_string(value) + ")";
  }
 // constructors
  Checker(T value): value(value) {
    std::cout << "Constructor from " << typeid(T).name() <<" = " << value << '\n';
  }
  Checker(): value() {
    std::cout << "Default-construct " << nameType() << '\n';
  }
  Checker(const Checker& other): value(other.value) {
    std::cout << "Copy-construct " << name() << '\n';
  }
  Checker(Checker&& other): value(other.value) {
    std::cout << "Move-construct " << name() << '\n';
  }
  template<class U>
  Checker(const Checker<U>& other): value(other.value) {
    std::cout << "Copy-construct<" << typeid(U).name() << "> " << name() << '\n';
  }
  template<class U>
  Checker(Checker<U>&& other): value(other.value) {
    std::cout << "Move-construct<" << typeid(U).name() << "> " << name() << '\n';
  }
 // destructor
  ~Checker() {
    std::cout << "Destruct " << name() << '\n';
  }
 // operator =
  Checker& operator=(const T& in) & {
    value = in;
    std::cout << "Equate Checker<" << typeid(T).name() << "> value = " << value << '\n';
    return *this;
  }

  Checker& operator=(const Checker& other) & {
    value = other.value;
    std::cout << "Copy-equate Checker<" << typeid(T).name() << "> value = " << value << '\n';
    return *this;
  }

  Checker& operator=(Checker&& other) & {
    value = other.value;
    std::cout << "Move-equate Checker<" << typeid(T).name() << "> value = " << value << '\n';
    return *this;
  }
 // operators
  template<class U>
  Checker& operator*=(const Checker<U>& other) & {
    std::cout << name() << " *= " << other.name() << '\n';
    value *= other.value;
    return *this;
  }

  Checker& operator*=(const T& val) & {
    std::cout << name() << " *= " << val << '\n';
    value *= val;
    return *this;
  }

  template<class U>
  Checker& operator+=(const Checker<U>& other) & {
    std::cout << name() << " += " << other.name() << '\n';
    value += other.value;
    return *this;
  }

  Checker& operator+=(const T& val) & {
    std::cout << name() << " += " << val << '\n';
    value += val;
    return *this;
  }

  template<class U>
  Checker& operator/=(const Checker<U>& other) & {
    std::cout << name() << " /= " << other.name() << '\n';
    value /= other.value;
    return *this;
  }

  Checker& operator/=(const T& val) & {
    std::cout << name() << " /= " << val << '\n';
    value /= val;
    return *this;
  }

  template<class U>
  Checker& operator-=(const Checker<U>& other) & {
    std::cout << name() << " -= " << other.name() << '\n';
    value -= other.value;
    return *this;
  }

  Checker& operator-=(const T& val) & {
    std::cout << name() << " -= " << val << '\n';
    value -= val;
    return *this;
  }

  operator T() const {
    std::cout << name() << " translate to T\n";
    return value;
  }
};

template<class T>
Checker(T value) -> Checker<T>;

template<class T, class U>
auto operator+(const Checker<T>& left, const Checker<U>& right) {
  std::cout << left.name() << " + " << right.name() << '\n';
  ++ countSum;
  return Checker(left.value + right.value);
}

template<class T, class U>
auto operator-(const Checker<T>& left, const Checker<U>& right) {
  std::cout << left.name() << " - " << right.name() << '\n';
  return Checker(left.value - right.value);
}


template<class T, class U>
auto operator*(const Checker<T>& left, const Checker<U>& right) {
  std::cout << left.name() << " * " << right.name() << '\n';
  ++ countMul;
  return Checker(left.value * right.value);
}

template<class T, class U>
auto operator/(const Checker<T>& left, const Checker<U>& right) {
  std::cout << left.name() << " / " << right.name() << '\n';
  return Checker(left.value / right.value);
}
