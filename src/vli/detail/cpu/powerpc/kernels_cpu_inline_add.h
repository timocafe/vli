//
//  inline_add.h
//  vli
//
//  Created by Timothée Ewart on 22.01.13.
//
//

#ifndef vli_inline_add_h
#define vli_inline_add_h

//SFINAE  http://www.boost.org/doc/libs/1_52_0/libs/utility/enable_if.html


//Address operand syntax, I always forget
//
//There are up to 4 parameters of an address operand that are presented in the syntax displacement(base register, offset register, scalar multiplier). This is equivalent to [base register + displacement + offset register * scalar multiplier] in Intel syntax. Either or both of the numeric, and either of the register parameters may be omitted:
//movl    -4(%ebp, %edx, 4), %eax  # Full example: load *(ebp - 4 + (edx * 4)) into eax
//movl    -4(%ebp), %eax           # Typical example: load a stack variable into eax
//movl    (%ecx), %edx             # No offset: copy the target of a pointer into a register
//leal    8(,%eax,4), %eax         # Arithmetic: multiply eax by 4 and add 8
//leal    (%eax,%eax,2), %eax      # Arithmetic: multiply eax by 2 and add eax (i.e. multiply by 3)

// "movq (%[y],%[counter],8), %[tmp_register]\n\t" load *(y - 0 + counter*8)


#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/utility/enable_if.hpp>


namespace vli{
    namespace detail{


/* \cond I do not need this part in the doc*/


#define VLI_EIGHT_MULTIPLY(n)    BOOST_PP_STRINGIZE(BOOST_PP_MUL(n,8))

#define VLI_ADDITION( z, n, unused) \
                                    "ld %[tmp_register1],"VLI_EIGHT_MULTIPLY(n)"(%[x])   \n\t" \
                                    "ld %[tmp_register2],"VLI_EIGHT_MULTIPLY(n)"(%[y])   \n\t" \
                                     BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adde),BOOST_PP_STRINGIZE(addc))" %[tmp_register1], %[tmp_register1], %[tmp_register2]\n\t" \
                                    "std %[tmp_register1],"VLI_EIGHT_MULTIPLY(n)"(%[x])  \n\t" \

#define VLI_ADDITION3( z, n, unused) "ld %[tmp_register1],"VLI_EIGHT_MULTIPLY(BOOST_PP_ADD(n,1))"(%[x])   \n\t" \
                                     "adde %[tmp_register1], %[tmp_register1], %4\n\t" \
                                     "std %[tmp_register1],"VLI_EIGHT_MULTIPLY(BOOST_PP_ADD(n,1))"(%[x])  \n\t" \

#define VLI_ADDITION6(z, n, unused)                                      \
                                    "ld %[tmp_register1],"VLI_EIGHT_MULTIPLY(n)"(%[y])   \n\t" \
                                    "ld %[tmp_register2],"VLI_EIGHT_MULTIPLY(n)"(%[w])   \n\t" \
                                     BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adde),BOOST_PP_STRINGIZE(addc))" %[tmp_register1], %[tmp_register1], %[tmp_register2]\n\t" \
                                    "std %[tmp_register1],"VLI_EIGHT_MULTIPLY(n)"(%[x])  \n\t" \

#define VLI_GENERATE_ADDITION(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,2), VLI_ADDITION, ~)

#define VLI_GENERATE_ADDITION3(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,1), VLI_ADDITION3, ~)

#define VLI_GENERATE_ADDITION6(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,2), VLI_ADDITION6, ~)

template<std::size_t NumWords, typename range = void>
struct helper_inline_add;

/**
 \brief addition between two integer<n*128>, n*128 <= 512
 \return void
 \param x uint64_t* pointer of the first entry of the Integer container
 \param y uint64_t* pointer of the first entry of the Integer container
 This operator performs a += between two integer<n*128> The ASM solver is specific 
 and full unroll n belongs to the interval 1 to 8, 128-bit <= vli::integer<n*128>  <= 512-bit
*/
#define FUNCTION_INLINE_add_nbits_nbits(z, m, unused)                                                  \
template<std::size_t NumWords>                                                                         \
struct helper_inline_add<NumWords,typename boost::enable_if_c< NumWords == BOOST_PP_ADD(m,2)>::type>{  \
    static void inline inline_add(uint64_t* x, uint64_t const* y){                       \
        uint64_t tmp_register1, tmp_register2;                                                  \
        __asm__ __volatile__ (                                                                         \
            VLI_GENERATE_ADDITION(m)                                                                   \
        : [tmp_register1] "=&r" (tmp_register1), [tmp_register2] "=&r" (tmp_register2)                  \
        : [x] "b" (x), [y] "b" (y)                                                                     \
        : "memory", "cc");                                                                             \
    };                                                                                                 \
                                                                                                       \
    static void inline_add(uint64_t* x, boost::int64_t const y){                                \
        uint64_t tmp_register1, tmp_register2;                                                  \
        uint64_t constant(y >> 63);                                                             \
        __asm__ __volatile__ (                                                                         \
            "ld %[tmp_register1],     0(%[x])      \n\t"                                               \
            "addc  %[tmp_register1],  %[tmp_register1], %3 \n\t"                                       \
            "std %[tmp_register1],   0(%[x])  \n\t"                                                    \
            VLI_GENERATE_ADDITION3(m)                                                                  \
        : [tmp_register1] "=&r" (tmp_register1), [tmp_register2] "=r" (tmp_register2)                  \
        : [x] "b" (x), "r" (y), "r" (constant)                                                         \
        : "memory", "cc");                                                                             \
    };                                                                                                 \
                                                                                                       \
    static void inline_add_extend(uint64_t* x, uint64_t const* y, uint64_t const* w){ \
        uint64_t tmp1(y[NumWords-1]),tmp2(w[NumWords-1]); \
        uint64_t tmp_register1, tmp_register2;                                                  \
        __asm__ __volatile__ ( \
              "sradi %5, %5, 63 \n\t" \
              "sradi %6, %6, 63 \n\t" \
              VLI_GENERATE_ADDITION6(m) \
              "adde %[tmp_register1], %5, %6 \n\t" \
              "std %[tmp_register1], "BOOST_PP_STRINGIZE(BOOST_PP_MUL(8,BOOST_PP_ADD(m,2)))"(%[x]) \n\t" \
        : [tmp_register1] "=&r" (tmp_register1), [tmp_register2] "=&r" (tmp_register2)                  \
        : [x] "b" (x), [y] "b" (y),[w] "b" (w), "r" (tmp1), "r" (tmp2) \
        : "memory", "cc"); \
    }; \
};                                                                                                     \

BOOST_PP_REPEAT(7, FUNCTION_INLINE_add_nbits_nbits, ~) //unroll until 512, maybe test 1024

#undef FUNCTION_INLINE_add_nbits_nbits

#undef VLI_EIGHT_MULTIPLY
#undef VLI_ADDITION
#undef VLI_ADDITION3
#undef VLI_ADDITION6

#undef VLI_GENERATE_ADDITION
#undef VLI_GENERATE_ADDITION3
#undef VLI_GENERATE_ADDITION6

    }} // end namespace
#endif
