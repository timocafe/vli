#include <iostream>

#include "vli/integer.hpp"
#include "vli/polynomial.hpp"
#include "vli/vector.hpp"

int main(int argc, char * argv[]){

   typedef vli::integer<128> integer_type_cpu_128;
   typedef vli::polynomial< integer_type_cpu_128, vli::max_order_each<10>, vli::var<'x'> > polynomial_type_each_x_128;
   typedef vli::vector<polynomial_type_each_x_128> vector_polynomial_type_each_x_128;

   vector_polynomial_type_each_x_128 v(128);

   int n = {0};
  // std::generate(v.begin(), v.end(), [&n]{ return n++; });

    vli::integer<128> a;
    std::cout << a << std::endl;
    std::cout << std::hex << a <<  std::endl;
}
