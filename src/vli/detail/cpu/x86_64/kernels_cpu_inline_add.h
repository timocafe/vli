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

#define VLI_ADDITION( z, n, unused) "movq " VLI_EIGHT_MULTIPLY(n)"(%[y]), %[tmp_register] \n\t" \
     BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adcq),BOOST_PP_STRINGIZE(addq))" %[tmp_register], " VLI_EIGHT_MULTIPLY(n)"(%[x]) \n\t" \

#define VLI_ADDITION2(z, n, unused) "movq " VLI_EIGHT_MULTIPLY(n)"(%[y],%[counter],8), %[tmp_register]\n\t" \
                                    "adcq %[tmp_register], " VLI_EIGHT_MULTIPLY(n)"(%[x], %[counter], 8)\n\t" \

#define VLI_ADDITION3(z, n, unused)  BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adcq %[constante2]),BOOST_PP_STRINGIZE(addq %[tmp_register]))", " VLI_EIGHT_MULTIPLY(n)"(%[x]) \n\t"

#define VLI_ADDITION4(n) BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adcq %[constante2]),BOOST_PP_STRINGIZE(addq %[tmp_register]))", (%[x], %[counter], 8)\n\t"

#define VLI_ADDITION5(z, n, unused) "adcq %[constante2], " VLI_EIGHT_MULTIPLY(n)"(%[x], %[counter], 8)\n\t"

#define VLI_ADDITION6(z, n, unused)                                      \
              "movq " VLI_EIGHT_MULTIPLY(n)"(%[y]), %[tmp_register] \n\t" \
              BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adcq),BOOST_PP_STRINGIZE(addq))" " VLI_EIGHT_MULTIPLY(n)"(%[w]), %[tmp_register] \n\t" \
              "movq %[tmp_register], " VLI_EIGHT_MULTIPLY(n)"(%[x]) \n\t" \




#define VLI_GENERATE_ADDITION(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,2), VLI_ADDITION, ~)

#define VLI_GENERATE_ADDITION2(m)  BOOST_PP_REPEAT(m, VLI_ADDITION2, ~)

#define VLI_GENERATE_ADDITION3(m)  BOOST_PP_REPEAT(BOOST_PP_ADD(m,2), VLI_ADDITION3, ~)

#define VLI_GENERATE_ADDITION5(m)  BOOST_PP_REPEAT(m, VLI_ADDITION5, ~)

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
#define FUNCTION_INLINE_add_nbits_nbits(z, m, unused)                                  \
template<std::size_t NumWords>                                                         \
struct helper_inline_add<NumWords,typename boost::enable_if_c< NumWords == BOOST_PP_ADD(m,2)>::type>{\
    static void inline_add(uint64_t* x, uint64_t const* y){              \
        uint64_t tmp_register;                                                  \
        __asm__ __volatile__ (                                                         \
            VLI_GENERATE_ADDITION(m)                                                   \
        : [tmp_register] "=&r" (tmp_register)                                          \
        : [x] "r" (x), [y] "r" (y)                                                     \
        : "memory", "cc");                                                             \
    };                                                                                 \
                                                                                       \
    static void inline_add(uint64_t* x, uint64_t const y){               \
        uint64_t tmp_register;                                                  \
        uint64_t tmp_register2;                                                 \
        __asm__ __volatile__ (                                                         \
            "movq  %[constante],   %[tmp_register] \n\t"                               \
            "movq  %[tmp_register],   %[constante2] \n\t"                               \
            "sarq $63, %[constante2] \n\t"                                             \
            VLI_GENERATE_ADDITION3(m)                                                  \
        : [tmp_register] "=&r" (tmp_register) , [constante2] "=&r" (tmp_register2)     \
        : [x] "r" (x), [constante] "r" (y)           \
        : "memory", "cc");                                                             \
    };                                                                                 \
                                                                                                    \
    static void inline_add_extend(uint64_t* x, uint64_t const* y, uint64_t const* w){ \
        uint64_t tmp1(y[NumWords-1]),tmp2(w[NumWords-1]); \
        uint64_t tmp_register; \
        __asm__ __volatile__ ( \
              "sarq $63, %4 \n\t" \
              "sarq $63, %5 \n\t" \
              VLI_GENERATE_ADDITION6(m) \
              "movq %4, %[tmp_register] \n\t" \
              "adcq %5, %[tmp_register] \n\t" \
              "movq %[tmp_register], " BOOST_PP_STRINGIZE(BOOST_PP_MUL(8,BOOST_PP_ADD(m,2)))"(%[x]) \n\t" \
        : [tmp_register] "=&r" (tmp_register) \
        : [x] "r" (x), [y] "r" (y),[w] "r" (w), "r" (tmp1), "r" (tmp2) \
        : "memory", "cc"); \
    }; \
};                                                                                     \

BOOST_PP_REPEAT(7, FUNCTION_INLINE_add_nbits_nbits, ~) //unroll until 512, maybe test 1024

#undef FUNCTION_INLINE_add_nbits_nbits


/**
 \brief addition between two integer<n*128>, n*128 > 512
 \return void
 \param x uint64_t* pointer of the first entry of the Integer container
 \param y uint64_t* pointer of the first entry of the Integer container
 This operator performs a += between two integer<n*128> The ASM solver is specific
 and full unroll n belongs to the interval 1 to 8, 512-bit <= vli::integer<n*128>  */
#define FUNCTION_INLINE_add_nbits_nbits(z, m, unused)                                     \
template<std::size_t NumWords>                                                                   \
struct helper_inline_add<NumWords,typename boost::enable_if_c<  ((NumWords>8) && (NumWords%8 == m )) >::type>{ \
    static void inline_add(uint64_t* x, uint64_t const* y){                 \
        uint64_t tmp_register, tmp_register2;                                      \
        __asm__ __volatile__ (                                                            \
            "clc\n\t"                                                                     \
            "1:\n\t"  /* OK I unroll until 8 maybe 16 a day */                            \
            "movq (%[y],%[counter],8), %[tmp_register]\n\t"                               \
            "adcq %[tmp_register], (%[x], %[counter], 8)\n\t"                             \
                                                                                          \
            "movq 8(%[y],%[counter],8), %[tmp_register]\n\t"                              \
            "adcq %[tmp_register], 8(%[x], %[counter], 8)\n\t"                            \
                                                                                          \
            "movq 16(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "adcq %[tmp_register], 16(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 24(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "adcq %[tmp_register], 24(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 32(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "adcq %[tmp_register], 32(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 40(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "adcq %[tmp_register], 40(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 48(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "adcq %[tmp_register], 48(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "movq 56(%[y],%[counter],8), %[tmp_register]\n\t"                             \
            "adcq %[tmp_register], 56(%[x], %[counter], 8)\n\t"                           \
                                                                                          \
            "leaq 7(,%[counter],1), %[counter]\n\t" /* lea does not change zero flag so counter += 7 */  \
            "incq %[counter]\n\t" /* change the zero flag */                              \
            "jnz 1b\n\t" /* check if I can leav the loop */                               \
            VLI_GENERATE_ADDITION2(m) /* finish the unroll by hand */                     \
        : [tmp_register] "=&r" (tmp_register), "=r" (tmp_register2)                       \
        : [x] "r" (x+(NumWords-m)), [y] "r" (y+(NumWords-m)), [counter] "1" (-(NumWords-m))\
        : "memory", "cc");                                                                \
    };                                                                                    \
    /* TUNE ME PLEASE */                                                                  \
static void inline_add(uint64_t* x, uint64_t const y){                      \
    uint64_t tmp_register,tmp_register3;                                           \
    uint64_t tmp_register2(y >> 63);                                               \
    __asm__ __volatile__ (    /* TO TUNE */                                               \
        "movq %[constante],    %[tmp_register] \n\t"                                      \
        "addq %[tmp_register],  (%[x], %[counter], 8)\n\t"                                \
        "incq %[counter]\n\t" /* change the zero flag */                                  \
        "1:\n\t"                                                                          \
        "adcq %[constante2],    (%[x], %[counter], 8)\n\t"                                \
        "incq %[counter]\n\t" /* change the zero flag */                                  \
        "jnz 1b\n\t" /* check if I can leav the loop */                                   \
        : [tmp_register] "=&r" (tmp_register), "=r" (tmp_register3)                       \
        : [x] "r" (x+(NumWords)), [constante] "r" (y), [constante2] "r" (tmp_register2), [counter] "1" (-(NumWords))\
        : "memory", "cc");                                                                \
    };                                                                                    \
};                                                                                        \

BOOST_PP_REPEAT(8, FUNCTION_INLINE_add_nbits_nbits, ~) //unroll until 512, maybe test 1024

#undef FUNCTION_INLINE_add_nbits_nbits

#undef VLI_EIGHT_MULTIPLY
#undef VLI_ADDITION
#undef VLI_ADDITION2
#undef VLI_ADDITION3
#undef VLI_ADDITION4
#undef VLI_ADDITION5

#undef VLI_GENERATE_ADDITION
#undef VLI_GENERATE_ADDITION2
#undef VLI_GENERATE_ADDITION3
#undef VLI_GENERATE_ADDITION5

/* \endcond I do not need this part in the doc*/

    }} // end namespace
#endif
