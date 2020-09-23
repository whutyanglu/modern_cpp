#include <iostream>
#include "const_expr.h"
#include "type_alias.h"
#include "optional_v.h"
#include "my_forward.h"
#include "boost_option_preview.h"
#include "cmm_optional.h"
#include "mem_special.h"
#include <optional>


void optional_test() {
  using pbx_base::optional;

  const std::string EQUAL_STR = "abc";

  { // copy 
    // assign
    std::cout << "\n  ======================== \n";
    optional<std::string> left;
    assert(!left.has_value() && !left);

    optional<std::string> right(EQUAL_STR);
    assert(right.has_value() && right);
    assert(right == EQUAL_STR);
    assert(left < right);
    assert(left <= right);
    assert(right > left);
    assert(right >= left);
    assert(left != right);

    left = right;
    assert(left.has_value() && left);
    assert(left == EQUAL_STR);
    assert(left == right); 
    assert(!(left != right));
    assert(left <= right);
    assert(right >= left);
  }


  {
    std::cout << "\n  ======================== \n";

    assert(EQUAL_STR >= "AAAAA");
    assert(EQUAL_STR > "AAAAA");

    // "abc" < "AAAAA"
    optional<std::string> left(pbx_base::make_optional<std::string>(5, 'A'));
    optional<std::string> right(pbx_base::make_optional<std::string>(EQUAL_STR));

    assert(left != right);
    assert(!(left == right));
    assert(left < right);
    assert(left <= right);

    assert(right > left);
    assert(right >= left);

    pbx_base::swap(left, right);

    assert(left != right);
    assert(!(left == right));

    assert(left > right);
    assert(left >= right);

    assert(right < left);
    assert(right <= left);

    right.reset();
    assert(!right.has_value() && !right);

    pbx_base::swap(left, right);
    assert(!left.has_value() && !left);
    assert(right.has_value() && right);
  }


  {
    std::cout << "\n  ======================== \n";

    optional<std::string> left = EQUAL_STR;
    assert(left == EQUAL_STR);
    assert(EQUAL_STR == left);
    assert(left != "A");
    assert("A" != left);

    assert(left <= "bbb");
    assert(left < "bbb");

    assert("bbb" > left);
    assert("bbb" >= left);

    // value or
    assert(left.value_or("bbb") == EQUAL_STR);

    optional<std::string> && left_rv = std::move(left);
    assert(left_rv.value_or("bbb") == EQUAL_STR);
    assert(!left.value().empty());

    std::string moved_value = std::move(left).value_or("bbb");
    assert(moved_value == EQUAL_STR);
    assert(left_rv.value().empty());
    assert(left.value().empty());
   
    left.reset();
    assert(left.value_or("bbb") == "bbb");

    std::string other_str("ccc");
    std::string lv_str = left.value_or(other_str);
    assert(!other_str.empty());
    std::string rv_str = left.value_or(std::move(other_str));
    assert(rv_str == "ccc" && other_str.empty());
  }

  {
     
  }

}





int main(int argc, const char* argv[]) {
  optional_test();
  return 0;
}
