#include <iostream>
#include <cassert>
#include <type_traits>

namespace my_option {
// If cxx17 supported, use std::optional
template <typename T>
struct RemoveConst {
  typedef T type;
};

template <typename T>
struct RemoveConst<const T&> {
  typedef T type;
};

template<typename Ty>
class OptionalField {
public:
  OptionalField() : has_value(false), dummy{} {
    std::cout << "[OptionalField()] invoked !!!\n";
  }

  OptionalField(const Ty& right) {  // From empty to value-has state
    std::cout << "[OptionalField::OptionalField(const Ty& right)] invoked !!!\n";
    this->has_value = false;
    Construct(right);
  }

  ~OptionalField() {
    std::cout << "[OptionalField::~OptionalField] invoked !!! has value : " << this->has_value << " \n";
    if (this->has_value) {
      this->value.~Ty(); // How about trivially destructor ? e.g. build-in types ?
    }
  }

  void Reset() { 
    if (this->has_value)
      this->value.~Ty();
      
    this->has_value = false;
  }

private:
  Ty& Construct(const Ty& right) {
    std::cout << "[OptionalField::Construct] \n";
    ::new(&this->value) Ty(right);
    this->has_value = true;

    return this->value;
  }

private:
  union {
    char dummy;
    typename RemoveConst<Ty>::type value; // Remove const
  };

  bool has_value;
 };


struct TyTrivialDestructor {
  int a;
  TyTrivialDestructor() = default;
  explicit TyTrivialDestructor(int i) { std::cout << "TyTest::TyTest(int i)\n";  a = i;}

  ~TyTrivialDestructor() {
    std::cout << "TyTest::~TyTest()\n";
  }
};

struct TyDefaultDestructor {
  TyDefaultDestructor() = default;
  explicit TyDefaultDestructor(int i1 ) : i (i1){}

  ~TyDefaultDestructor() = default;


  int i;
};


union union_test {
  char dummy;
  TyDefaultDestructor tt;

};


struct MyClass {
  //MyClass(int a) { std::cout << "MyClass::MyClass(int a);";}
  MyClass() = default;
  MyClass(int && a) noexcept  : a_(std::move(a)) { std::cout << "MyClass::MyClass(int && a);\n";}
  MyClass(const int& a) noexcept  : a_(a)  { std::cout << "MyClass::MyClass(const int & a);\n";}
  MyClass(int a, int b, char c) noexcept  : a_(a) { std::cout << "MyClass::MyClass(int a, int b, char c);\n"; }
  
  MyClass(const MyClass & right) noexcept : a_(right.a_) { std::cout << "MyClass::(const MyClass & right);\n"; }
  MyClass(MyClass && right) noexcept : a_(std::move(right.a_)) { std::cout << "MyClass(MyClass && right)\n"; }

  MyClass & operator=(const MyClass& right) noexcept {
    std::cout << "MyClass::operator=(const MyClass& right);\n";
    a_ = right.a_;
  }

  MyClass& operator=(MyClass && right) noexcept  {
    std::cout << "MyClass::operator=(MyClass&& right);\n";
    a_ = right.a_;
  }


  int a_;
};


template <class Ty>
class mytest{
  public:
    explicit mytest() {}

    mytest(Ty &&s ) : t(s){}
    mytest(const Ty& s) : t(s) {}

    template <class Ty2=Ty>
    explicit mytest(const mytest<Ty2> & right) {
      std::cout << "template <class Ty2=Ty>\n mytest(const mytest<Ty2> & right)\n";
    }

    template <class Ty2 = Ty>
    explicit mytest(mytest<Ty2>&& right) {
      std::cout << "template <class Ty2=Ty>\n mytest(mytest<Ty2> && right)\n";
    }

    mytest& operator=(const mytest& right) {
      std::cout << "\n mytest& operator=(const mytest& right)\n";
      t = right.t;
      return *this;
    }

    mytest& operator=(mytest && right) {
      std::cout << "\n mytest& operator=(mytest&& right)\n";
      t = right.t;
      return *this;
    }

    template <class Ty2 = Ty>
    mytest& operator=(const mytest<Ty2>& right) {
      std::cout << "template <class Ty2=Ty>\n mytest& operator=(const mytest<Ty2>& right)\n";
      t = right.t;
      return *this;
    }

    template <class Ty2 = Ty>
    mytest& operator=(mytest<Ty2>&& right) {
      std::cout << "template <class Ty2=Ty>\n mytest& operator=(mytest<Ty2>&& right)\n";
      t = right.t;
      return *this;
    }

    Ty t;
};


struct optional_place_t {
  optional_place_t() = default;
};

constexpr optional_place_t optional_place;


template<typename Ty, typename std::enable_if<!std::is_pointer<Ty>::value, int>::type = 0, 
                      typename std::enable_if<!std::is_reference<Ty>::value, int>::type = 0 >
class my_optional {

public:
  my_optional() noexcept : has_value(false), value(nullptr) {
    std::cout << "\nmy_optional::my_optional()\n";
  }

  my_optional(const Ty& right) noexcept : has_value(true), value(new Ty(right)) {
    std::cout << "\nmy_optional(const Ty& right)\n";
  }
  
  // move constructor
  my_optional(my_optional && right) noexcept {
    std::cout << "\n move constructor \n";

    _Construct(std::move(right));
  }

  // copy constructor 
  my_optional(const my_optional& right) noexcept {
    std::cout << "\n copy constructor \n";
    _Construct(right);
  }
  
  // other constructor
  template<class Ty2>
  my_optional(my_optional<Ty2>&& right) noexcept {
    _Construct(right);
  }

  template<typename ...Types>
  explicit my_optional(optional_place_t,  Types && ... args) noexcept : has_value(true), value(new Ty(std::forward<Types>(args)...)) {
    std::cout << "\n Variadic template constructor \n";
  }

  template<typename Ty2>
  my_optional& operator=(const my_optional<Ty2>& right) {
    std::cout << "\n other template copy assign \n";
    return _Construct(right);
  }

  template<typename Ty2>
  my_optional& operator=(my_optional<Ty2>&& right) noexcept {
    std::cout << "\n other template move assign \n";
    return _Construct(std::move(right));
  }

  my_optional& operator=(my_optional && right) noexcept {
    std::cout << " move assign \n";
    return _Construct(std::move(right));
  }

  my_optional& operator=(const my_optional& right) noexcept {
    std::cout << " copy assign \n";
    return _Construct(right);
  }

  ~my_optional() noexcept {
    Reset();
  }
  
  const Ty& operator*() const { 
    if (this->has_value) return *value; 
  }

 private:
 template<class Ty2>
 my_optional& _Construct(my_optional<Ty2>&& right) {
  std::cout << "\nmy_optional move _Construct \n ";
  if (right.has_value) {
    this->has_value = true;
    this->value = right.value;

    right.has_value = false;
    right.value = nullptr;
  }

  return *this;
 }

 template<class Ty2>
 my_optional& _Construct(const my_optional<Ty2>& right) {
   std::cout << " \nmy_optional copy _Construct \n ";
   if (right.has_value) {
     this->has_value = true;
     this->value = new Ty(*right);
   }

   return *this;
 }

 template<class Ty2>
 my_optional& _Assign(my_optional<Ty2>&& right) {
   std::cout << "\n my_optional move _Assign \n ";
   if (right.has_value) {
     this->has_value = true;
     this->value = right.value;

     right.has_value = false;
     right.value = nullptr;
   }
   else {
     Reset();
   }

   return *this;
 }

 template<class Ty2>
 my_optional& _Assign(const my_optional<Ty2>& right) {
   std::cout << " \n my_optional copy _Assign \n ";
   if (right.has_value) {
     this->has_value = true;
     this->value = new Ty(*right);
   }

   return *this;
 }

 // Reset to default
 void Reset() noexcept {
   assert(!!has_value == !!value); // Make sure 

   if (has_value && value)
     delete value;

   has_value = false;
   value = nullptr;
 }

 // Detach value, value has been attached to other
 void Detach() noexcept {
  assert(has_value && value);

  has_value = false;
  value = nullptr;
 }

 template<typename Ty2>
 void Attach(const Ty2 * _other) noexcept {
  assert(!has_value && !value);

  has_value = true;
  value = _other;
 }

private:
  bool has_value = false;
  Ty* value = nullptr;
};

void test() {
  std::cout << "\nmy_option test begin == \n\n";

  //my_optional<std::string> mop_str4("efg"); // construct from Ty
  //my_optional<std::string> mop_str5(std::move(mop_str4)); // move constructor 
  //my_optional<std::string> mop_str6(mop_str5); // copy constructor

  //my_optional<std::string> mop_str7(optional_place, 7, 'a');
  //std::cout << "mop_str7 : " << *mop_str7;

  //my_optional<std::string> mop_str8 = mop_str7;
  //std::cout << "mop_str8 : " << *mop_str8;
  //my_optional<std::string> mop_str9 = std::move(mop_str8); // copy constructor
  //std::cout << "mop_str9 : " << *mop_str9;


  my_optional<std::string> s1, s2;


  std::cout << "\nmy_option test end == \n\n";
}


void optionfield_test() {
  //std::cout << "\n my_option test \n";
//union_test utst;
//utst.dummy = 'a';

//utst.tt = TyTest();
//OptionalField<TyTrivialDestructor> option_tt(TyTrivialDestructor(4));
//OptionalField<TyDefaultDestructor> option_t2(2);

//OptionalField<int> opt_int(4);
//

//OptionalField<int> a;
//OptionalField<int> a1(3);

//char location_test[4];
//int *k = new (location_test) int(0xffff);

//k->~int();
}


};