#pragma once

#include <iostream>
#include <type_traits>

namespace pbx_base {

class optional_bad_access : public std::exception {
public:
  optional_bad_access() : std::exception("optional bad access") {}
};

inline void _Throw_bad_option_access() {
  throw (optional_bad_access{});
}

struct optional_place_t { 
optional_place_t () = default;
} optional_place;

template<typename Ty, typename std::enable_if<!std::is_pointer<Ty>::value, int>::type = 0, 
                      typename std::enable_if<!std::is_reference<Ty>::value, int>::type = 0>
class optional {
 public:

  typedef optional<Ty> this_type;
  typedef typename std::remove_reference<Ty>::type value_type;

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

   this_type& operator= (const this_type & _right) {
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

   this_type& operator= (const value_type& _right) {
    has_value_ = true;
    value_ = _right;

    std::cout << "this_type& operator = (const value_type& _right)" << "\n";
    return *this;
   }

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

  value_type&& operator* () && {
    return std::move(this->value_);
  }
  const value_type&& operator* () const&& {
    return std::move(this->value_);
  }

  explicit operator bool() const noexcept {
    return this->has_value_;
  }
  bool has_value() const noexcept {
    return this->has_value_;
  }
  const value_type& value() const &{
    if (!this->has_value_) _Throw_bad_option_access();
    return this->value_;
  }
  
  value_type& value() & {
    if (!this->has_value_) _Throw_bad_option_access();
    return this->value_;
  }

  value_type&& value() const && {
    if (!this->has_value_) _Throw_bad_option_access();
    return std::move(this->value_);
  }
  value_type&& value() && {
    if (!this->has_value_) _Throw_bad_option_access();
    return std::move(this->value_);
  }

  template<class Ty2>
  value_type value_or(Ty2 && _right) const& {
    if (this->has_value_) {
      return this->value_;
    }

    return static_cast<value_type>(std::forward<Ty2>(_right));
  }


  template<class Ty2>
  value_type value_or(Ty2&& _right) && {
    if (this->has_value_) {
      return std::move(this->value_);
    }

    return static_cast<value_type>(std::forward<Ty2>(_right));
  }

  void swap(this_type & right) {
    const bool engaged = this->has_value_;
    if (engaged == right.has_value_) {
      if (engaged) {
        std::swap(**this, *right);
      }
    }
    else {
      this_type & source = engaged ? *this : right;
      this_type & target = engaged ? right : *this;
      target = std::move(source);
      source.reset();
    }
  }

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
  return left_has_value == right.has_value() && (!left_has_value || *left == *right);
}

template<typename Ty1, typename Ty2>
bool operator!=(const optional<Ty1> &left, const optional<Ty2>& right) {
  const bool left_has_value = left.has_value();
  return left_has_value != right.has_value() || (left_has_value && *left != *right);
}

template<typename Ty1, typename Ty2>
bool operator<(const optional<Ty1> &left, const optional<Ty2>& right) {
  return right.has_value() && (!left.has_value() || *left < *right);
}

template<typename Ty1, typename Ty2>
bool operator>(const optional<Ty1>& left, const optional<Ty2>& right) {
  return left.has_value() && (!right.has_value() || *left > *right);
}

template<typename Ty1, typename Ty2>
bool operator<=(const optional<Ty1>& left, const optional<Ty2>& right) {
  return !left.has_value() || (right.has_value() && *left <= * right);
}

template<typename Ty1, typename Ty2>
bool operator>=(const optional<Ty1>& left, const optional<Ty2>& right) {
  return !right.has_value() || (left.has_value() && *left >= *right);
}

// comparision with T - 
template<typename Ty1, typename Ty2>
bool operator==(const optional<Ty1>& left, const Ty2& right) {
  return left.has_value() && *left == right;
}

template<typename Ty1, typename Ty2>
bool operator==(const Ty1& left, const optional<Ty2>& right) {
  return right.has_value() && left == *right;
}

template<typename Ty1, typename Ty2>
bool operator!=(const optional<Ty1>& left, const Ty2& right) {
  return !left.has_value() || *left != right;
}

template<typename Ty1, typename Ty2>
bool operator!=(const Ty1& left, const optional<Ty2>& right) {
  return !right.has_value() || left != *right;
}

template<typename Ty1, typename Ty2>
bool operator<(const optional<Ty1>& left, const Ty2& right) {
  return !left.has_value() || *left < right;
}

template<typename Ty1, typename Ty2>
bool operator<(const Ty1& left, const optional<Ty2>& right) {
  return right.has_value() && left < *right;
}

template<typename Ty1, typename Ty2>
bool operator>(const optional<Ty1>& left, const Ty2& right) {
  return left.has_value() && *left > right;
}

template<typename Ty1, typename Ty2>
bool operator>(const Ty1& left, const optional<Ty2>& right) {
  return !right.has_value() || left > *right;
}

template<typename Ty1, typename Ty2>
bool operator>=(const optional<Ty1>& left, const Ty2& right) {
  return left.has_value() && *left >= right;
}

template<typename Ty1, typename Ty2>
bool operator>=(const Ty1& left, const optional<Ty2>& right) {
  return !right.has_value() || left >= * right;
}

template<typename Ty1, typename Ty2>
bool operator<=(const optional<Ty1>& left, const Ty2& right) {
  return !left.has_value() || *left <= right;
}

template<typename Ty1, typename Ty2>
bool operator<=(const Ty1& left, const optional<Ty2>& right) {
  return right.has_value() && left <= *right;
}

template<typename Ty>
optional<Ty> make_optional(Ty&& value) {
  return optional<Ty>(std::forward<Ty>(value));
}

template<typename Ty, typename ...Types>
optional<Ty> make_optional(Types&&... args) {
  return optional<Ty>{optional_place, std::forward<Types>(args)...};
}

template<typename Ty>
void swap(optional<Ty> & left, optional<Ty> & right) {
  left.swap(right);
}

}; // pbx_base