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
class optional {
 public:

  typedef optional<Ty>                   this_type;
  typedef typename remove_refernce<Ty>::type value_type;

   ~optional() { 
    reset(); 
    std::cout << "~cmm_optional()" << "\n";
   }

   optional() 
   : has_value_(false)
   , value_() { std::cout << "cmm_optional()" << "\n"; 
   }
   optional(const value_type& _right) 
   : has_value_(true)
   , value_(_right) { 
    std::cout << "cmm_optional::cmm_optional(const value_type& _right)" << "\n";
   }
   optional(const this_type & _right) {
     has_value_ = _right.has_value_;
     if (has_value_) {
      value_ = _right.value_;
     }
     std::cout << "cmm_optional(const this_type & _right)" << "\n";
   }

   this_type& operator = (const this_type & _right) {
     if (_right.has_value_) {
       has_value_ = true;
       value_ = _right.value_;
     }
     else {
       reset();
     }

     std::cout << "this_type& operator = (const this_type & _right)" << "\n";
     return *this;
   }

   this_type& operator = (const value_type& _right) {
    has_value_ = true;
    value_ = _right;

    std::cout << "this_type& operator = (const value_type& _right)" << "\n";
    return *this;
   }

 #if defined HAS_CXX11
   optional(value_type && _right) 
   : has_value_(true)
   , value_(std::move(_right)) { 
     std::cout << "cmm_optional(value_type && _right)" << "\n"; 
   }

   optional(this_type && _right) {
     has_value_ = _right.has_value_;
     if (has_value_) {
       value_ = std::move(_right.value_);
     }

     std::cout << "cmm_optional(this_type && _right)" << "\n";
   }

   template<typename ...Types>
   optional(optional_place_t, Types &&... args) : has_value_(true), value_(std::forward<Types>(args)... ) {
     std::cout << "cmm_optional(optional_place_t, Types &&... args)" << "\n";
   }

   this_type& operator = (value_type && _right) {
     has_value_ = true;
     value_ = std::move(_right);
     std::cout << "this_type& operator = (value_type && _right)" << "\n";
     return *this;
   }

   this_type& operator = (this_type && _right) noexcept {
     std::cout << "this_type& operator = (this_type && _right)" << "\n";
     if (_right.has_value_) {
      has_value_ = true;
      value_ = std::move(_right).value_;
     }
     else {
       reset();
     }
     return *this;
   }

#endif


  // Observers
  const value_type* operator-> () const { 
    return &value_; 
  }
  value_type* operator->() {
    return &value_;
  }

  const value_type& operator* () const& {
    return value_;
  }
  value_type& operator* () & {
    return value_;
  }
#if defined HAS_CXX11 
  value_type&& operator* () && {
    return std::move(this->value_);
  }
  const value_type&& operator* () const&& {
    return std::move(this->value_);
  }
#endif

  explicit operator bool() const noexcept {
    return this->has_value_;
  }
  bool has_value() const noexcept {
    return this->has_value_;
  }
  const value_type& value() const &{
    if (!this->has_value_) throw optional_bad_access;
    return this->value_;
  }
  
  value_type& value() & {
    if (!this->has_value_) throw optional_bad_access;
    return this->value_;
  }

  #ifdef HAS_CXX11
  value_type&& value() const && {
    if (!this->has_value_) throw optional_bad_access;
    return std::move(this->value_);
  }
  value_type&& value() && {
    if (!this->has_value_) throw optional_bad_access;
    return std::move(this->value_);
  }
  #endif

#ifdef HAS_CXX11
  value_type value_or(Ty && _right) const & {
    if (this->has_value_) {
      return this->value_;
    }

    return static_cast<value_type>(std::forward<Ty>(_right));
  }

  value_type value_or(Ty&& _right) && {
    if (this->has_value_) {
      return std::move(this->value_);
    }

    return static_cast<value_type>(std::forward<Ty>(_right));
  }

#endif

  // Modifiers
  void reset() {
    has_value_ = false;
    std::cout << "reset()\n";
  }
  
 private:
   value_type value_;
   bool has_value_;
};


// Relationship operators
template<typename Ty1, typename Ty2>
bool operator==(const optional<Ty1> &left, const optional<Ty2> &right) {
  const bool left_has_value = left.has_value();
  return left_has_value == right.has_value() && (left_has_value || *left == *right);
}

template<typename Ty1, typename Ty2>
bool operator!=(const optional<Ty1> &left, const optional<Ty2>& right) {
  const bool left_has_value = left.has_value();
  return left_has_value != right.has_value() || (left_has_value && *left != *right);
}


template<typename Ty1, typename Ty2>
bool operator<(const optional<Ty1> &left, const optional<Ty2>& right) {
  
  return right.has_value() && ();
}












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

{
  A aa(5);
  optional<A> a1(aa);               // copy value
  optional<A> a2(std::move(aa));    // move value 
}

 std::cout << "====================\n";
{
  A bb(7);
  optional<A> a3;                   // Construct
  a3 = bb;                              // Assign copy
  a3 = std::move(bb);                   // Assign move
 }
 
  std::cout << "====================\n";
 {
  optional<A> a;
  optional<A> b(optional_place, 4);
  optional<A> c(optional_place, 4, "5", (float)6.0);
  optional<A> d;

  a = b;
  std::cout << "===\n";

  a = std::move(c);

  std::cout << "===\n";
  a = d;
  
  std::cout << "\n===22222222222222222222\n";
  //B bb;
  //A aa;
  //aa = bb;
  //aa = std::move(bb);

  //c = bb;
  //c = std::move(bb);

  //cmm_optional<B> d2;
  //c = d2;

  //c = std::move(bb);

  std::cout << "\n=============== END ============= \n";
 }



  //cmm_optional<A> a;
  //cmm_optional<A> b(optional_place, 4);
  //cmm_optional<A> c(optional_place, 4, "5", 6.0);

  //cmm_optional<A> d(b);


  //cmm_optional<B> b;
}


}; // pbx_base