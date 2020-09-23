#pragma once

#include <iostream>
#include <type_traits>

#include<optional>

namespace mem {

template<class _Ty>
class optional {
public:
  optional()  = default;
  ~optional() = default;
  optional(const optional& right) {
    t= right.t;
  }
  optional(optional && right) {
    t = std::move(right).t;
  }
  explicit optional(const _Ty & right) {
    t = right;
  }

  explicit optional(_Ty&& right) {
    t = std::move(right);
  }

  template<class _Ty2>
  struct allow_convert : std::enable_if< std::negation_v< std::is_same<optional, std::_Remove_cvref_t<_Ty2>>>, int>{};

  template<class _Ty2>
  using allow_convert_t = typename allow_convert<_Ty2>::type;

  template<class _Ty2, allow_convert_t = 0 >
  explicit optional(_Ty2&& _Right) {
    this->_Assign(std::forward<_Ty2>(_Right));
  }

  template<class _Ty2, allow_convert_t = 0>
  explicit optional(optional<_Ty2>&& _Right) {
    this->_Assign(std::move(_Right.t));
  }

  template<class _Ty2, allow_convert_t = 0>
  explicit optional(const optional<_Ty2>&& _Right) {
    this->_Assign(_Right.t);
  }

  // _Ty2 != optional<_Ty1>
  //template<class _Ty2, std::enable_if_t< std::negation_v< std::is_same<optional, std::_Remove_cvref_t<_Ty2>>>, int> = 0  >
  //optional& operator=(_Ty2&& _Right) {
  //  constexpr bool is_same_type = std::is_same_v<optional, std::_Remove_cvref_t<_Ty2>>;
  //  std::cout << "is_same_type : " << is_same_type << std::endl;

  //  this->_Assign(std::forward<_Ty2>(_Right));
  //  return *this;
  //}

  // _Ty copy
  optional& operator=(const optional<_Ty>& _Right) {
    this->_Assign(_Right.t);
    return *this;
  }

  // _Ty move
  optional& operator=(optional<_Ty>&& _Right) {
    this->_Assign(std::move(_Right.t));
    return *this;
  }

  // _Ty2 move optional
  template<class _Ty2>
  optional& operator=(optional<_Ty2>&& _Right) {
    this->_Assign(_Right.t);
    return *this;
  }

  // _Ty2 copy optional
  template<class _Ty2>
  optional& operator=(const optional<_Ty2>& _Right) {
    this->_Assign(std::move(_Right.t));
    return *this;
  }

  const _Ty& value() const& { return t; }
  _Ty& value()& { return t; }
  _Ty&& value()&& { return std::move(t); }
  _Ty&& value() const&& { return std::move(t); }

  std::remove_reference_t<_Ty> t;

  template <class Ty2>
  void _Assign(Ty2&& _Right) {
    t = std::forward<Ty2>(_Right);
  }
};


template<class _Ty1>
void TestOverload(_Ty1&& abc) {
  optional<_Ty1> bb;
  bb.t = std::forward<_Ty1>(abc);
}

template<class _Ty1>
void TestOverload(optional<_Ty1> && abc) {
  optional<_Ty1> bb;
  bb.t = abc.t;
}

template<class _Ty1>
void TestOverload(const optional<_Ty1>& abc) {
  optional<_Ty1> bb;
  bb.t = abc.t;
}


void test() 
{
  //optional<std::string> str1("abcddd");
  //optional<std::string> str2 = "abcddd";
  //str1 = "abc";

  int c = 5;

  optional<int> int_a(3);
  optional<int> int_b(4.5);

  //int_a = int_b;

  //optional<float> float_a = 4.0;
  //float_a = int_a;
  //int_a = float_a;

  //float_a = c;

  //optional<std::string> str2;
  //str2 = str1;


  //str2 = str1;

  int k = 45;
  //TestOverload(k);
  //TestOverload(4);
 // TestOverload(str1);
 // TestOverload(std::move(str1));

  //optional<std::string> str2;
  //str2 = str1; // build Failed

}


}
