#include <iostream>
#include <random>
#include <numeric>

#include "vli/integer.hpp"
#include "vli/polynomial.hpp"
#include "vli/vector.hpp"

int main(int argc, char * argv[]){
   typedef vli::integer<512> integer;
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> dis(0,std::numeric_limits<uint64_t>::max());
   auto rand = std::bind(dis, std::ref(gen));

   std::vector<integer> v(16);

   std::generate(v.begin(), v.end(), [&rand](){integer a;
                                               std::generate(a.begin(), a.end(), rand);
                                               return a;}
                 );

   std::vector<integer> vc(v);
   std::shuffle(vc.begin(),vc.end(), gen);

   std::ostream_iterator<integer> out_it (std::cout << std::hex,"\n");
    
   std::sort(v.begin(),v.end());
   //nice print
   std::copy(v.begin(),v.end(),out_it);
}
