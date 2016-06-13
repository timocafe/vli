/* \cond I do not need this part in the doc*/

    #include <iostream>
    #include "vli/integer.hpp"
    #include "vli/polynomial.hpp"
    #include "vli/vector.hpp"

    int main(int argc, char * argv[]){
      vli::integer<128> a(1);
      std::cout << std::hex << a << std::endl;
    }

/* \endcond I do not need this part in the doc*/

