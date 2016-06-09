/*
 *Very Large Integer Library, License - Version 1.0 - May 3rd, 2012
 *
 *Timothee Ewart - University of Geneva,
 *Andreas Hehn - Swiss Federal Institute of technology Zurich.
 *
 *Permission is hereby granted, free of charge, to any person or organization
 *obtaining a copy of the software and accompanying documentation covered by
 *this license (the "Software") to use, reproduce, display, distribute,
 *execute, and transmit the Software, and to prepare derivative works of the
 *Software, and to permit third-parties to whom the Software is furnished to
 *do so, all subject to the following:
 *
 *The copyright notices in the Software and this entire statement, including
 *the above license grant, this restriction and the following disclaimer,
 *must be included in all copies of the Software, in whole or in part, and
 *all derivative works of the Software, unless such copies or derivative
 *works are solely in the form of machine-executable object code generated by
 *a source language processor.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 *SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 *FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 *ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *DEALINGS IN THE SOFTWARE.
 */

#ifndef vli_inline_sub_h
#define vli_inline_sub_h

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
/* \cond I do not need this part in the doc*/

namespace vli{
    namespace detail{

#define VLI_EIGHT_MULTIPLY(n)    BOOST_PP_STRINGIZE(BOOST_PP_MUL(n,8))

#define VLI_SUBTRACTION( z, n, unused) "movq " VLI_EIGHT_MULTIPLY(n)"(%[y]), %[tmp_register] \n\t" \
     BOOST_PP_IF(n,BOOST_PP_STRINGIZE(sbbq),BOOST_PP_STRINGIZE(subq))" %[tmp_register], " VLI_EIGHT_MULTIPLY(n)"(%[x]) \n\t" \

#define VLI_SUBTRACTION2(z, n, unused) "movq " VLI_EIGHT_MULTIPLY(n)"(%[y],%[counter],8), %[tmp_register]\n\t" \
    "sbbq %[tmp_register], " VLI_EIGHT_MULTIPLY(n)"(%[x], %[counter], 8)\n\t" \

#define VLI_SUBTRACTION3(z, n, unused) \
     BOOST_PP_IF(n,BOOST_PP_STRINGIZE(sbbq %[constante2]),BOOST_PP_STRINGIZE(subq %[tmp_register]))", " VLI_EIGHT_MULTIPLY(n)"(%[x]) \n\t"

#define VLI_GENERATE_SUBTRACTION(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,2), VLI_SUBTRACTION, ~)

#define VLI_GENERATE_SUBTRACTION2(m)  BOOST_PP_REPEAT(m, VLI_SUBTRACTION2, ~)

#define VLI_GENERATE_SUBTRACTION3(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,2), VLI_SUBTRACTION3, ~)

template<std::size_t n, typename range = void>
struct helper_inline_sub;

/**
 \brief subtraction between two integer<n*128>
 \return void
 \param x uint64_t* pointer of the first entry of the Integer container
 \param y uint64_t* pointer of the first entry of the Integer container
 This operator performs a += between two integer<n*128> The ASM solver is specific and full unroll
 n belongs to the interval 1 to 8, 128-bit <= vli::integer<n*128>  <= 5120-bit
*/
#define FUNCTION_INLINE_sub_nbits_nbits(z, m, unused)                                  \
template<std::size_t NumWords>                                                                \
struct helper_inline_sub<NumWords,typename boost::enable_if_c< NumWords == BOOST_PP_ADD(m,2)>::type>{\
    static void inline_sub(uint64_t* x, uint64_t const* y){              \
        uint64_t tmp_register;                                                  \
            __asm__ __volatile__ (                                                     \
            VLI_GENERATE_SUBTRACTION(m)                                                \
            : [tmp_register] "=&r" (tmp_register)                                      \
            : [x] "r" (x), [y] "r" (y)                                                 \
            : "memory", "cc");                                                         \
    };                                                                                 \
                                                                                       \
        static void inline_sub(uint64_t* x, boost::int64_t const y){            \
        uint64_t tmp_register;                                                  \
        uint64_t tmp_register2;                                                 \
        __asm__ __volatile__ (                                                         \
            "movq  %[constante],   %[tmp_register] \n\t"                               \
            "movq  %[tmp_register],   %[constante2] \n\t"                               \
            "sarq $63, %[constante2] \n\t" \
            VLI_GENERATE_SUBTRACTION3(m)                                               \
        : [tmp_register] "=&r" (tmp_register), [constante2] "=&r" (tmp_register2)      \
        : [x] "r" (x), [constante] "r" (y)                                             \
        : "memory", "cc");                                                             \
    };                                                                                 \
};                                                                                     \

BOOST_PP_REPEAT(7, FUNCTION_INLINE_sub_nbits_nbits, ~) //unroll until 512, maybe test 1024

#undef FUNCTION_INLINE_sub_nbits_nbits


/**
 \brief subtraction between two integer<n*128>, n*128 > 512
 \return void
 \param x uint64_t* pointer of the first entry of the Integer container
 \param y uint64_t* pointer of the first entry of the Integer container
 This operator performs a -= between two integer<n*128> The ASM solver is generic
 and full unroll n belongs to the interval 1 to 8, 512-bit <= vli::integer<n*128>  */
#define FUNCTION_INLINE_sub_nbits_nbits(z, m, unused)                                     \
template<std::size_t n>                                                                   \
struct helper_inline_sub<n,typename boost::enable_if_c<  ((n>8) && (n%8 == m )) >::type>{ \
    static void inline_sub(uint64_t* x, uint64_t const* y){                 \
        uint64_t tmp_register, tmp_register2;                                      \
        __asm__ __volatile__ (                                                           \
            "clc\n\t"                                                                     \
            "1:\n\t"  /* OK I unroll until 8 maybe 16 a day */                            \
            "movq (%[y],%[counter],8), %[tmp_register]\n\t"                               \
            "sbbq %[tmp_register], (%[x], %[counter], 8)\n\t"                             \
                                                                                          \
            "movq 8(%[y],%[counter],8), %[tmp_register]\n\t"                              \
            "sbbq %[tmp_register], 8(%[x], %[counter], 8)\n\t"                            \
                                                                                          \
            "movq 16(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "sbbq %[tmp_register], 16(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 24(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "sbbq %[tmp_register], 24(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 32(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "sbbq %[tmp_register], 32(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 40(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "sbbq %[tmp_register], 40(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 48(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "sbbq %[tmp_register], 48(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 56(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "sbbq %[tmp_register], 56(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "leaq 7(,%[counter],1), %[counter]\n\t" /* lea does not change zero flag so counter += 7 */  \
            "incq %[counter]\n\t" /* change the zero flag */                              \
            "jnz 1b\n\t" /* check if I can leav the loop */                               \
            VLI_GENERATE_SUBTRACTION2(m) /* finish the unroll by hand */                  \
        : [tmp_register] "=&r" (tmp_register), "=r" (tmp_register2)                       \
        : [x] "r" (x+(n-m)), [y] "r" (y+(n-m)), [counter] "1" (-(n-m))                    \
        : "memory", "cc");                                                                \
    };                                                                                    \
};                                                                                        \

BOOST_PP_REPEAT(8, FUNCTION_INLINE_sub_nbits_nbits, ~) //unroll until 512, maybe test 1024

#undef FUNCTION_INLINE_sub_nbits_nbits

#undef VLI_EIGHT_MULTIPLY
#undef VLI_SUBTRACTION
#undef VLI_SUBTRACTION2
#undef VLI_SUBTRACTION3

#undef VLI_GENERATE_SUBTRACTION
#undef VLI_GENERATE_SUBTRACTION2
#undef VLI_GENERATE_SUBTRACTION3

/* \endcond I do not need this part in the doc*/

    }} //end namespace

#endif
