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

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/comparison/equal.hpp>

//macro to get the correct name of the register
#define R(n)        BOOST_PP_STRINGIZE(BOOST_PP_CAT(%%r, BOOST_PP_ADD(8,n))) // give register starts from r8 
#define RCLOTHER(n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(r, BOOST_PP_ADD(8,n)))  // for clother
#define PPS(m,n)    BOOST_PP_STRINGIZE(BOOST_PP_MUL(BOOST_PP_MUL(m,n),8)) // m*n*8, 8 because long int, if one day SoA today AoS

// move ASM operator to get the data from the mem
#define LOAD_register(z, n, unused)     "movq " PPS(VLI_AOS,n)"(%%rdi)                 ," R(n) " \n" /* load 0x??(%%rdi) */
#define LOAD_register_rdx(z, n, unused) "movq " PPS(VLI_AOS,n)"(%%rdx)                 ," R(n) " \n" /* load 0x??(%%rdi) */
// addition ASM operators
#define ADC0_register(z, n, unused) "adcq $0x0                                ," R(BOOST_PP_ADD(n,1))" \n" /* adcq 0 + rdi + CB    */     

#define Addition( z, n, unused) "movq " PPS(VLI_AOS,n)"(%%rsi), %%rax \n" \
                                "" BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adcq),BOOST_PP_STRINGIZE(addq))" %%rax, " PPS(VLI_AOS,n)"(%%rdi) \n" \

#define Addition2( z, n, unused) "adcq %%rcx              , " PPS(VLI_AOS,BOOST_PP_ADD(n,1))"(%%rdi) \n" \

#define Addition3( z, n, unused) "movq " PPS(VLI_AOS,n)"(%%rdx), %%rax \n" \
                                 "" BOOST_PP_IF(n,BOOST_PP_STRINGIZE(adcq),BOOST_PP_STRINGIZE(addq))" " PPS(VLI_AOS,n)"(%%rsi), %%rax \n" \
                                 "movq %%rax              , " PPS(VLI_AOS,n)"(%%rdi) \n" 

// substraction ASM operators 
#define Substraction( z, n, unused) "movq " PPS(VLI_AOS,n)"(%%rsi), %%rax \n" \
                                    "" BOOST_PP_IF(n,BOOST_PP_STRINGIZE(sbbq),BOOST_PP_STRINGIZE(subq))" %%rax, " PPS(VLI_AOS,n)"(%%rdi) \n" \

#define Substraction2( z, n, unused) "sbbq %%rcx         , " PPS(VLI_AOS,BOOST_PP_ADD(n,1))"(%%rdi) \n" 

// multiplication VLI<n*64> *= 64 bits, note : results are saved in to r8, r9, r10 .... thus for the first iteration I move direclty inside
#define  MUL_register(z, n, unused) "mulq " PPS(1,BOOST_PP_ADD(n,1))"(%%rdi)             \n" /* mulq r??*rax */                \
                                    "addq %%rax            ," R(BOOST_PP_ADD(n,1))"      \n" /* add hia?b? + loa?b? */         \
                                    "movq %%rdx            ," R(BOOST_PP_ADD(n,2))"      \n" /* save the hi into rcx */        \
                                    "adcq $0               ," R(BOOST_PP_ADD(n,2))"      \n" /* perhaps carry */               \
                                    "movq %%rbx            ,%%rax                       \n" /* reload rax(a0) from the rbx */ \
// negate for 2CM method, combine with ADC0_register macro
#define NOT_register(z, n, unused)  "notq " R(n)"                                        \n" /* start C2M negate */ 
// movi ASM operators to set up the data into the mem
#define SAVE_register(z, n, unused) "movq " R(n)"           ," PPS(VLI_AOS,n)"(%%rdi)         \n" /* save 0x??(%%rdi) */     
// generate the list of registers clother
#define CLOTHER_register(z, n, unused) RCLOTHER(n), /* "r8","r9", ... */
// for the multiplication
#define Rax(MAX,n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(%%r, BOOST_PP_SUB(15,BOOST_PP_SUB(MAX,n)))) // give register start at r15, r12, .... reverse order  
#define Rdx(MAX,n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(%%r, BOOST_PP_SUB(15,BOOST_PP_SUB(BOOST_PP_SUB(MAX,1),n)))) // give register start at r15, r12, .... reverse order  
#define NUM1(MAX,n) BOOST_PP_SUB(15,BOOST_PP_SUB(BOOST_PP_SUB(MAX,2),n))
#define Radc0(MAX,n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(%%r, BOOST_PP_ADD(MAX,n))) // give register start at r15, r12, .... reverse order  
#define ADC0_register_mulnton(z, n, nbegin) "adcq $0x0, " Radc0(nbegin,n)"      \n" /* adcq 0 + rdi + CB    */     
#define Rr(Max,n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(%%r, BOOST_PP_ADD(BOOST_PP_SUB(15,Max),n))) // give register start at r8  
#define SAVEr_register(z, n, MAX) "movq " Rr(MAX,BOOST_PP_ADD(n,1))", " PPS(VLI_AOS,n)"(%%rdi)    \n" /* save 0x??(%%rdi) */     
#define PPSr1(Max,n) BOOST_PP_STRINGIZE( BOOST_PP_MUL(BOOST_PP_SUB(Max,n),8)) // m*n*8, 8 because long int

#define Rr2(z, n, unused)  BOOST_PP_STRINGIZE(BOOST_PP_CAT(r, BOOST_PP_SUB(15,n))), // give register start at r8  

#define MULNTON1(z, n, niteration) \
                BOOST_PP_IF(n,"movq %%rbx, %%rax \n",) \
                "mulq " PPS(1,n)"(%%rdi) \n" \
                BOOST_PP_IF(n,"addq %%rax, " Rax(niteration,n)" \n","movq %%rax, " Rax(niteration,n)" \n") \
                BOOST_PP_IF(n,BOOST_PP_IF(BOOST_PP_EQUAL(niteration,n), ,"adcq %%rdx," Rdx(niteration,n)" \n"), BOOST_PP_IF(BOOST_PP_EQUAL(niteration,n), ,"addq %%rdx," Rdx(niteration,n)" \n")) \
                BOOST_PP_REPEAT(BOOST_PP_SUB(niteration,BOOST_PP_ADD(n,1)), ADC0_register_mulnton, NUM1(niteration,n)) 

#define MULNTON0(z, n, MAX) \
                "movq " PPSr1(MAX,n)"(%%rsi), %%rax \n" \
                BOOST_PP_IF(n,"movq %%rax, %%rbx \n", ) \
                BOOST_PP_REPEAT(BOOST_PP_ADD(n,1), MULNTON1, n) 
