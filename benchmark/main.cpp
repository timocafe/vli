//system and boost
#include <boost/mpl/for_each.hpp>
#include "boost/lexical_cast.hpp"
#include <gmpxx.h>
#include <iomanip> 
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>

#include "vli/integer.hpp"
#include "vli/polynomial.hpp"
#include "vli/vector.hpp"

#include "misc.hpp"
#include "tools.h"

#define Size_vec 512

//The order __ORDER__ is passed now by cmake, see cmakelist of the main
using vli::polynomial;
using vli::vector;
typedef vli::integer<128> vli_type_cpu_128;
typedef vli::integer<192> vli_type_cpu_192;
typedef vli::integer<256> vli_type_cpu_256;
/*  -------------------------------------------------------------------- 128 bits ---------------------------------------------------------------------------------- */
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_each<__ORDER__>, vli::var<'x'>  >polynomial_type_each_x_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>  >polynomial_type_each_xy_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>  >polynomial_type_each_xyz_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>, vli::var<'w'>  >polynomial_type_each_xyzw_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_combined<__ORDER__>, vli::var<'x'> > polynomial_type_combined_x_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'> > polynomial_type_combined_xy_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'> > polynomial_type_combined_xyz_128;
typedef vli::polynomial< vli_type_cpu_128, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>, vli::var<'w'> > polynomial_type_combined_xyzw_128;
/*  -------------------------------------------------------------------- 192 bits ---------------------------------------------------------------------------------- */
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_each<__ORDER__>, vli::var<'x'>  >polynomial_type_each_x_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>  >polynomial_type_each_xy_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>  >polynomial_type_each_xyz_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>, vli::var<'w'>  >polynomial_type_each_xyzw_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_combined<__ORDER__>, vli::var<'x'> > polynomial_type_combined_x_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'> > polynomial_type_combined_xy_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'> > polynomial_type_combined_xyz_192;
typedef vli::polynomial< vli_type_cpu_192, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>, vli::var<'w'> > polynomial_type_combined_xyzw_192;
/*  -------------------------------------------------------------------- 256 bits ---------------------------------------------------------------------------------- */
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_each<__ORDER__>, vli::var<'x'>  >polynomial_type_each_x_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>  >polynomial_type_each_xy_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>  >polynomial_type_each_xyz_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_each<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>, vli::var<'w'>  >polynomial_type_each_xyzw_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_combined<__ORDER__>, vli::var<'x'> > polynomial_type_combined_x_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'> > polynomial_type_combined_xy_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'> > polynomial_type_combined_xyz_256;
typedef vli::polynomial< vli_type_cpu_256, vli::max_order_combined<__ORDER__>, vli::var<'x'>, vli::var<'y'>, vli::var<'z'>, vli::var<'w'> > polynomial_type_combined_xyzw_256;

typedef boost::mpl::vector<
                            polynomial_type_each_xyz_128,
                            polynomial_type_each_xy_128,
                            polynomial_type_each_x_128
//                            polynomial_type_each_xyzw_128// buffer can be too large cpu/gpu, be cautious
                          > polynomial_list_128_each;

typedef boost::mpl::vector<
                            polynomial_type_combined_xyzw_128,
                            polynomial_type_combined_xyz_128,
                            polynomial_type_combined_xy_128,
                            polynomial_type_combined_x_128 
                          > polynomial_list_128_combined;

typedef boost::mpl::vector<
//                         polynomial_type_each_xyzw_192
                           polynomial_type_each_xyz_192,
                           polynomial_type_each_xy_192,
                           polynomial_type_each_x_192 
                          > polynomial_list_192_each;

typedef boost::mpl::vector<
                           polynomial_type_combined_xyzw_192,
                           polynomial_type_combined_xyz_192,
                           polynomial_type_combined_xy_192,
                           polynomial_type_combined_x_192
                          > polynomial_list_192_combined;

typedef boost::mpl::vector<
                       //   polynomial_type_each_xyzw_256// buffer can be too large cpu/gpu, be cautious
                            polynomial_type_each_xyz_256,
                            polynomial_type_each_xy_256,
                            polynomial_type_each_x_256
                          > polynomial_list_256_each;

typedef boost::mpl::vector<
                            polynomial_type_combined_xyzw_256,
                            polynomial_type_combined_xyz_256,
                            polynomial_type_combined_xy_256,
                            polynomial_type_combined_x_256
                          > polynomial_list_256_combined;


   struct test_case {

   template <typename Polynomial>
   void operator()(Polynomial const&) {
       std::cout.precision(5);
       //GMP polys give by class traits
       typedef typename vli::polynomial_multiply_type_gmp<Polynomial>::type Polynomial_gmp;
       typedef typename vli::polynomial_multiply_type_gmp<Polynomial>::type_res Polynomial_gmp_res;
       typedef vli::vector<Polynomial_gmp> vector_polynomial_gmp;
       typedef vli::vector<Polynomial_gmp_res> vector_polynomial_gmp_res;
       //VLI poly
       typedef typename vli::polynomial_multiply_result_type<Polynomial>::type Polynomial_res;
       typedef vli::vector<Polynomial> vector_polynomial;
       typedef vli::vector<Polynomial_res> vector_polynomial_res;
       //VLI polys
       vector_polynomial v1(Size_vec),v2(Size_vec);
       Polynomial_res p1_res, p2_res;
       //GMP polys
       vector_polynomial_gmp v1_gmp(Size_vec),v2_gmp(Size_vec);
       Polynomial_gmp_res p_gmp_res;

       tools::fill_vector_random(v1);
       tools::fill_vector_random(v2);

       tools::converter(v1,v1_gmp);
       tools::converter(v2,v2_gmp);

       std::chrono::time_point<std::chrono::system_clock> start, end;
       start = std::chrono::system_clock::now();
           p_gmp_res = vli::detail::inner_product_cpu(v1_gmp,v2_gmp);
       end = std::chrono::system_clock::now();
       std::chrono::duration<double> elapsed_seconds_gmp = end-start;

       start = std::chrono::system_clock::now();
           p1_res = vli::detail::inner_product_cpu(v1,v2);
       end = std::chrono::system_clock::now();
       std::chrono::duration<double> elapsed_seconds_vli = end-start;

       #ifdef VLI_USE_GPU
       Timer t1("GPU ");
       t1.begin();
           p2_res =  vli::detail::inner_product_gpu_helper<Polynomial>::inner_product_gpu(v1,v2);
       t1.end();
       #endif

       if(tools::equal<Polynomial>(p1_res,p_gmp_res))
               std::cout << "  OK, cpu/gmp " << elapsed_seconds_vli.count() ;
       #ifdef VLI_USE_GPU
               if(p1_res == p2_res)
                   std::cout << " gpu "  t1.get_time() ;
               else
                   std::cout << " gpu no ok";
       #endif
               std::cout << " gmp " <<  elapsed_seconds_gmp.count() ;
               std::cout.precision(2);
       #ifdef VLI_USE_GPU
       std::cout << " G vli: "  << elapsed_seconds_gmp.count()/elapsed_seconds_vli.count();
//               tool::timescheduler::save(tgmp.get_time(),t0.get_time(),t1.get_time());
       #else
        std::cout << " G vli: "  << elapsed_seconds_gmp.count()/elapsed_seconds_vli.count();
       #endif

       }
   };


int main(int argc, char* argv[]) {
       std::cout << " -------ASCII ART ^_^' --------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -------Size vector : " << Size_vec  << ", Order " << __ORDER__ << std::endl;
       std::cout << " -----  Max_Order_Each --------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  3 variable --------------------------------- 2 variables --------------------------------- 1 variables ----------------------------------------------------------------- " << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  256bits * 256bits = 512 bits ------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       boost::mpl::for_each<polynomial_list_256_each>(test_case());
       std::cout << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  192bits * 192bits = 384 bits ------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       boost::mpl::for_each<polynomial_list_192_each>(test_case());
       std::cout << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  128bits * 128bits = 256 bits ------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       boost::mpl::for_each<polynomial_list_128_each>(test_case());
       std::cout << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  Max__Order_Combined ---------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  4 variable --------------------------------- 3 variables --------------------------------- 2 variables --------------------------------- 1 variables ------------------- " << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  256bits * 256bits = 512 bits ------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       boost::mpl::for_each<polynomial_list_256_combined>(test_case());
       std::cout << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  192bits * 192bits = 384 bits ------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       boost::mpl::for_each<polynomial_list_192_combined>(test_case());
       std::cout << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       std::cout << " -----  128bits * 128bits = 256 bits ------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       boost::mpl::for_each<polynomial_list_128_combined>(test_case());
       std::cout << std::endl;
       std::cout << " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- " << std::endl;
       return 0;
}
