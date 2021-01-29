/* \cond I do not need this part in the doc*/

#include "vli/integer.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  size_t a[120];
  size_t b[120];

  for (int i = 0; i < 120; ++i) {
    a[i] = -1;
    b[i] = 0;
  }

  b[0] = 1;

  asm volatile("addq  %1 , %0 ; \n\t" : "+&r"(a[0]) : "rm"(b[0]));
  for (int i = 1; i < 120; ++i)
    asm volatile("adcq  %1 , %0 ; \n\t" : "+r"(a[i]) : "rm"(b[i]));

  for (int i = 0; i < 120; ++i)
    std::cout << a[i] << std::endl;
}

/* \endcond I do not need this part in the doc*/
