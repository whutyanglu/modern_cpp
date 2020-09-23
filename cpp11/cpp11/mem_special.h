#pragma once

#include <iostream>
#include <type_traits>

namespace mem {

class B {};

class A {
public:

  A() { std::cout << " A::A() " << std::endl; }
  explicit A(int) { std::cout << " A::A(int) " << std::endl; }
  explicit A(int, std::string, float) { std::cout << " A::A(int, std::string, float) " << std::endl; }

  A(const A&) { std::cout << " A::A(const A & ) " << std::endl; }
  A(A&&) noexcept { std::cout << " A::A(A &&) " << std::endl; }

  A& operator=(const A&) { std::cout << " A& A::operator=(const A&) " << std::endl; return *this; }
  A& operator=(A&&) noexcept { std::cout << " A& A::operator=(A&&) " << std::endl;  return *this; }

  A& operator=(const B&) { std::cout << " A& A::operator=(const B&) " << std::endl; return *this; }
  A& operator=(B&&) noexcept { std::cout << " A&  A::operator=(B&&) " << std::endl; return *this; }

private:

};

template<class _Ty>
class optional {
public:

  template<typename _Ty2>
  optional& operator=(_Ty2&& _Right) {
    this->_Assign(std::forward<_Ty2>(_Right));
    return *this;
  }
  
  template<typename _Ty2=_Ty>
  optional& operator=(const optional<_Ty2>& _Right) {
    t = _Right.t;
    return *this;
  }
  
  template<typename _Ty2=_Ty>
  optional& operator=(optional<_Ty2>&& _Right) {
    _Assign(std::move(_Right.t));
    return *this;
  }

  _Ty t;


  template <typename Ty2>
  void _Assign(Ty2&& _Right) {
    t = std::forward<Ty2>(_Right);
  }

};


void test() {

{
  optional<A> a;

  B b;
  a = b;
  a = std::move(b);

  A aa;
  a = aa;
  a = std::move(aa);
 }

 {
    std::cout << "\n\n=======================\n\n";


  //  a = aa;
   // a = std::move(b);
 }
}

}
