#include <iostream>
#include "const_expr.h"
#include "type_alias.h"
#include "optional_v.h"
#include "my_forward.h"
#include "boost_option_preview.h"
#include "cmm_optional.h"

#include <optional>

struct test_align {
  int i;
  char c;
  void * p;

  long long ii;
};


struct alignas(8) test_align_f {
  char sz[125];
};


struct test_align_f2 {
  char sz[15];
};

#pragma pack(show)
#pragma pack(push, r1, 1)
#pragma pack(show)

struct AlignTest {
  virtual void f() {}
  char ch;
  int i;
  double ff;
};

#pragma pack(pop, r1, 8)   // n = 2 , stack popped
#pragma pack(show)

int main(int argc, const char* argv[]) {
  std::cout << "Hello C++11 " << std::endl;


  pbx_base::test();

  pbx_base::cmm_optional<int> a;

  //constexpr size_t _size_of_t      = sizeof(AlignTest);
  //constexpr size_t _alignment_of_t = __alignof(AlignTest);
  //constexpr size_t _size_of_hack   = sizeof(alignment_of_hack<AlignTest>);

  //static_assert(_size_of_hack - _size_of_t == _alignment_of_t, "aaa");

  //std::cout << "align of float : " << __alignof(float) << ", size of float : " << sizeof(float) << "\n";;
  //std::cout << "align of Dummy : " << __alignof(aligned_storage<float>::Dummy) << "\n";
  //std::cout << "size of Dummy :  " << sizeof(aligned_storage<float>::Dummy) << "\n";

  //aligned_storage<std::string> al_string;
  //al_string.dummy_.aligner_;

  //aligned_storage<float> al_float;
  //al_float.dummy_.aligner_;

  //aligned_storage<short> al_short;
  //al_short.dummy_.aligner_;

  //aligned_storage<test_align> al_test_align;
  //al_test_align.dummy_.aligner_;

  //aligned_storage<test_align_f> al_test_align_f;
  //constexpr size_t AS = sizeof(decltype(al_test_align_f.dummy_.aligner_));

  //constexpr size_t FS = alignment_of<test_align_f>::value;

  //constexpr size_t A = sizeof(alignment_of_hack<test_align_f>);
  //constexpr size_t S = sizeof(test_align_f);
  //constexpr size_t ALOGIN_S = __alignof(test_align_f);
  //constexpr size_t SIZEOF_S = sizeof(al_test_align_f.dummy_);
  //constexpr size_t ALIGNF_S = __alignof(al_test_align_f.dummy_);

  //std::cout << "align of test_align_f : " << __alignof(test_align_f) << ", size of test_align_f : " << sizeof(test_align_f) << "\n";
  //std::cout << "align of Dummy : " << __alignof(aligned_storage<test_align_f>::Dummy) << "\n";
  //std::cout << "size of Dummy :  " << sizeof(aligned_storage<test_align_f>::Dummy) << "\n";
  //
  //aligned_storage<test_align_f2> al_test_align_f2;
  //al_test_align_f2.dummy_.aligner_;

  //constexpr size_t ALIGNF_S2 = __alignof(al_test_align_f2.dummy_);

  //std::cout << "align of test_align_f : " << __alignof(test_align_f2) << ", size of test_align_f2 : " << sizeof(test_align_f2) << "\n";
  //std::cout << "align of Dummy : " << __alignof(aligned_storage<test_align_f2>::Dummy) << "\n";
  //std::cout << "size of Dummy :  " << sizeof(aligned_storage<test_align_f2>::Dummy) << "\n";

  ////const_expr::test();
  ////my_option::test();
  ////my_forward::test();

  //alignment_of_hack<test_align_f> ksss;

  return 0;
}