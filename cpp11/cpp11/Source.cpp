#include <iostream>
#include "const_expr.h"
#include "type_alias.h"
#include "optional_v.h"
#include "my_forward.h"


int main(int argc, const char* argv[]) {
  std::cout << "Hello C++11 " << std::endl;
  
  const_expr::test();
  my_option::test();
  my_forward::test();

  return 0;
}