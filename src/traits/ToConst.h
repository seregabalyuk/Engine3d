#pragma once

namespace traits {
  template<class T>
  const T& toConst(const T& val) {
    return val;
  } 
} // namespace traits