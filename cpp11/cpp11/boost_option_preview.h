#pragma once

#include <iostream>

#define BOOST_STATIC_CONSTANT(type, assignment) static const type assignment

typedef long long long_long_type;
typedef unsigned long long ulong_long_type;

union max_align
{
  char c;
  short s;
  int i;
  long l;
  long_long_type ll;
  float f;
  double d;
  long double ld;
 };

template <typename T>
struct alignment_of_hack
{
  char c;
  T t;
  alignment_of_hack();
};


template <unsigned A, unsigned S>
struct alignment_logic
{
  BOOST_STATIC_CONSTANT(std::size_t, value = A < S ? A : S);
};

template< typename T >
struct alignment_of_impl {

  BOOST_STATIC_CONSTANT(std::size_t, value =
    (alignment_logic<
      sizeof(alignment_of_hack<T>) - sizeof(T),
      __alignof(T)
    >::value));
};

template <class T>
struct alignment_of : std::integral_constant<std::size_t, alignment_of_impl<T>::value> {};

template <std::size_t Target, bool check> struct long_double_alignment { typedef long double type; };
template <std::size_t Target> struct long_double_alignment<Target, false> { typedef max_align type; };

template <std::size_t Target, bool check> struct double_alignment { typedef double type; };
template <std::size_t Target> struct double_alignment<Target, false> { typedef typename long_double_alignment<Target, alignment_of<long double>::value >= Target>::type type; };

template <std::size_t Target, bool check> struct long_long_alignment { typedef long_long_type type; };
template <std::size_t Target> struct long_long_alignment<Target, false> { typedef typename double_alignment<Target, alignment_of<double>::value >= Target>::type type; };

template <std::size_t Target, bool check> struct long_alignment { typedef long type; };
template <std::size_t Target> struct long_alignment<Target, false> { typedef typename long_long_alignment<Target, alignment_of<long_long_type>::value >= Target>::type type; };

template <std::size_t Target, bool check> struct int_alignment { typedef int type; };
template <std::size_t Target> struct int_alignment<Target, false> { typedef typename long_alignment<Target, alignment_of<long>::value >= Target>::type type; };

template <std::size_t Target, bool check> struct short_alignment { typedef short type; };
template <std::size_t Target> struct short_alignment<Target, false> { typedef typename int_alignment<Target, alignment_of<int>::value >= Target>::type type; };

template <std::size_t Target, bool check> struct char_alignment { typedef char type; };
template <std::size_t Target> struct char_alignment<Target, false> { typedef typename short_alignment<Target, alignment_of<short>::value >= Target>::type type; };

template <std::size_t Align>
struct type_with_alignment
{
  typedef typename char_alignment<Align, alignment_of<char>::value >= Align>::type type;
};

template<typename Ty>
class aligned_storage {
  public:

  union Dummy {
    char dummy[sizeof(Ty)];
    typename type_with_alignment< alignment_of<Ty>::value>::type aligner_;
  }dummy_;

};


void test() {
  
}