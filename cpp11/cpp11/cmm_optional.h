#pragma once

#include <iostream>
#include <type_traits>

#define HAS_CXX11 
#define HAS_RVALUE_REFERENCE 

namespace pbx_base {

class optional_bad_access : public std::exception {
public:
  optional_bad_access() : std::exception("optional bad access") {}
};

struct optional_place_t { } optional_place;

#if defined HAS_CXX11
template<class T>
using remove_refernce = std::remove_reference<T>;
#else
template <typename Ty>
struct remove_refernce {
  typedef Ty type;
};

template <typename Ty>
struct remove_refernce<Ty&> {
  typedef Ty type;
};

#endif


#if defined HAS_CXX11
template<typename Ty, typename std::enable_if<!std::is_pointer<Ty>::value, int>::type = 0, 
                      typename std::enable_if<!std::is_reference<Ty>::value, int>::type = 0>
#else
template<typename Ty>
#endif
class cmm_optional {
 public:

  typedef cmm_optional<Ty>     this_type;
 // typedef Ty                   value_type;
  typedef typename remove_refernce<Ty>::type value_type;

   ~cmm_optional() {reset();}
   cmm_optional() : has_value_(false), value_() {}
   cmm_optional(const value_type& _right) : has_value_(true), value_(_right) {}
   cmm_optional(const this_type & _right) {
     has_value_ = _right.has_value_;
     if (has_value_) {
      value_ = _right.value_;
     }
   }

   this_type& operator = (const this_type & _right) {
     if (_right.has_value_) {
       has_value_ = true;
       value_ = _right.value_;
     }
     else {
       has_value_ = false;
     }
     return *this;
   }

   this_type& operator = (const value_type& _right) {
    has_value_ = true;
    value_ = _right;
    return *this;
   }
   
 #if defined HAS_CXX11
   cmm_optional(value_type && _right) : has_value_(true), value_(std::move(_right)) {}

   cmm_optional(this_type && _right) {
     has_value_ = _right.has_value_;
     if (has_value_) {
       value_ = std::move(_right.value_);
     }
   }

   template<typename ...Types>
   cmm_optional(optional_place_t, Types &&... args) : has_value_(true), value_(std::forward<Types>(args)... ) {}

   this_type& operator = (value_type && _right) {\
     _Assign(_right);
     return *this;
   }

   this_type& operator = (this_type && _right) {
     _Assign_from(_right);
     return *this;
   }

#endif

  void reset() {
    has_value_ = false;
  }

  const value_type& operator* () const { 
    if (!has_value_) throw optional_bad_access;
    
    return value_;
  }

  
 private:
  template<typename Ty2>
  void _Assign(Ty2 && _Right) {
    this->value_ = std::forward<Ty2>(_Right);
    this->has_value_ = true;
  }

  template<typename Self>
  void _Assign_from(Self && _Right) {
    if(_Right.has_value_) {
      this->has_value_ = true;
      this->value_ = _Assign(_Right.value);
    }
    else {
      reset();
    }
  }

 private:
   // typename remove_refernce<Ty>::type value_;
   value_type value_;
   bool has_value_;
};

class B {};

class A {
  public:

  A() { std::cout << " A::A() " << std::endl; }
  explicit A(int) { std::cout << " A::A(int) " << std::endl;  }
  explicit A(int, std::string, float) { std::cout << " A::A(int, std::string, float) " << std::endl;  }

  A(const A & ) { std::cout << " A::A(const A & ) " << std::endl; }
  A(A &&) noexcept { std::cout << " A::A(A &&) " << std::endl; }

  A& operator=(const A&) { std::cout << " A& A::operator=(const A&) " << std::endl; return *this;}
  A& operator=(A&&) noexcept { std::cout << " A& A::operator=(A&&) " << std::endl;  return *this;}

  A& operator=(const B&) { std::cout << " A& A::operator=(const B&) " << std::endl; return *this; }
  A& operator=(B&&) noexcept { std::cout << " A&  A::operator=(B&&) " << std::endl; return *this; }

 private:

};


void test() {

  A aa(5);
  cmm_optional<A> a1(aa);
  cmm_optional<A> a2(std::move(aa));



  //cmm_optional<A> a;
  //cmm_optional<A> b(optional_place, 4);
  //cmm_optional<A> c(optional_place, 4, "5", 6.0);

  //cmm_optional<A> d(b);


  //cmm_optional<B> b;
}


}; // pbx_base