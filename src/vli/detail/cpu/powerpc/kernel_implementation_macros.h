/*
*Very Large Integer Library, License - Version 1.0 - May 3rd, 2012
*
*Timothee Ewart - University of Geneva, 
*Andreas Hehn - Swiss Federal Institute of technology Zurich.
*
*Permission is hereby granted, free of charge, to any person or organization
*obtaining a copy of the software and accompanying documentation covered by
*this license (the  " Software") to use, reproduce, display, distribute,
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
*THE SOFTWARE IS PROVIDED  " AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
*SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
*FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
*ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*DEALINGS IN THE SOFTWARE.
*/

#include  "vli/detail/cpu/kernel_macros.h"


//#include <boost/preprocessor/punctuation/comma_if.hpp>
//#include <boost/preprocessor/comparison/greater_equal.hpp>

//macro to get the correct name of the register
#define R(n)            BOOST_PP_STRINGIZE(BOOST_PP_ADD(14,n)) // give register starts from r8 
#define SAVER(n,offset) BOOST_PP_STRINGIZE(BOOST_PP_ADD(BOOST_PP_SUB(26,offset),n))  // for clother
#define RCLOTHER(n)     BOOST_PP_STRINGIZE(BOOST_PP_CAT(r, BOOST_PP_ADD(14,n)))  // for clother
#define RCLOTHER2(n)    BOOST_PP_STRINGIZE(BOOST_PP_CAT(r, BOOST_PP_SUB(23,n)))  // for clother
#define RMULCLOTHER(n)  BOOST_PP_STRINGIZE(BOOST_PP_CAT(r, BOOST_PP_ADD(16,n)))  // for clother
#define PPS(m,n)        BOOST_PP_STRINGIZE(BOOST_PP_MUL(BOOST_PP_MUL(m,n),8)) // m*n*8, 8 because long int, if one day SoA today AoS

// move ASM operator to get the data from the mem
#define LOAD_register_r3(z, n, unused)      "  ld " R(n) "," PPS(VLI_AOS,n)"(3) \n" /* load 0x??(%%r3) */ 
#define LOAD_register_r5(z, n, unused)      "  ld " R(n) "," PPS(VLI_AOS,n)"(5) \n" /* load 0x??(%%r3) */ 
#define LOAD_register_r4(z, n, OffSet)      "  ld " R(BOOST_PP_ADD(n,OffSet))"," PPS(VLI_AOS,n)"(4) \n" /* load 0x??(%%r3) */ 
// addition ASM operators
#define  ADC_register(z, n, OffSet)    " adde "R(BOOST_PP_ADD(n,1))","R(BOOST_PP_ADD(n,1))","R(BOOST_PP_ADD(n,BOOST_PP_ADD(OffSet,1)))" \n" /* adcq rsi + rdi + CB  */     
#define  ADC0_register(z, n, unused)   " addze "R(BOOST_PP_ADD(n,3))","R(BOOST_PP_ADD(n,3))" \n" /* adcq rsi + rdi + CB  */     
#define  ADC00_register(z, n, OffSet)  " adde "R(BOOST_PP_ADD(n,1))","R(BOOST_PP_ADD(n,1))",6 \n" /* adcq rsi + rdi + CB  */     

// substraction ASM operators 
#define SUBC_register(z, n, OffSet)    " subfe "R(BOOST_PP_ADD(n,1))","R(BOOST_PP_ADD(n,BOOST_PP_ADD(OffSet,1)))","R(BOOST_PP_ADD(n,1))" \n" /* adcq rsi + rdi + CB  */     
#define SUB00_register(z, n, OffSet)   " subfe "R(BOOST_PP_ADD(n,1))",6,"R(BOOST_PP_ADD(n,1))" \n" /* adcq rsi + rdi + CB  */     
// multiplication VLI<n*64> *= 64 bits, note : results are saved in to r8, r9, r10 .... thus for the first iteration I move direclty inside
#define  MUL_register(z, n, unused)  " ld     "R(0)", "PPS(VLI_AOS,BOOST_PP_ADD(1,n))"(3) \n"             \
                                     " mulld   5, "R(0)","R(1)" \n "                                  \
                                     " addc   "R(BOOST_PP_ADD(3,n))", 5, "R(BOOST_PP_ADD(3,n))"  \n"  \
                                     " mulhdu  5, "R(0)","R(1)" \n "                                  \
                                     " addze  "R(BOOST_PP_ADD(4,n))", 5 \n"                       \

// negate for 2CM method, combine with ADC0_register macro
#define NOT_register(z, n, unused)   " not "R(BOOST_PP_ADD(n,2))" , "R(BOOST_PP_ADD(n,2))"                                      \n" /* start C2M negate */ 
// movi ASM operators to set up the data into the mem
#define STORE_register_r3(z, n, unused)      " std "R(n)","PPS(VLI_AOS,n)"(3) \n" /* load 0x??(%%r3) */ 
#define STORE_register_r4(z, n, OffSet)      " std "R(BOOST_PP_ADD(n,OffSet))","PPS(VLI_AOS,n)"(4) \n" /* load 0x??(%%r3) */ 
#define STORE_register_r3mul(z, n, unused)      " std "R(BOOST_PP_ADD(n,2))","PPS(VLI_AOS,n)"(3) \n" /* load 0x??(%%r3) */ 
#define STORE_register_r3mul2(z, n, offset)      " std "SAVER(n,offset)","PPS(VLI_AOS,n)"(3) \n" /* load 0x??(%%r3) */ 
// generate the list of registers clother
#define CLOTHER_register(z, n, unused) RCLOTHER(n), /*  " r8","r9", ... */
#define CLOTHER_register2(z, n, unused) RCLOTHER2(n), /*  " r8","r9", ... */
#define CLOTHER_MUL_register(z, n, unused) RMULCLOTHER(n), /*  " r8","r9", ... */
// for the multiplication
#define NUM1(MAX,n) BOOST_PP_SUB(23,BOOST_PP_SUB(BOOST_PP_SUB(MAX,2),n))
#define Radc0(MAX,n) BOOST_PP_STRINGIZE(BOOST_PP_ADD(MAX,n)) // give register start at r15, r12, .... reverse order  
#define ADC0_register_mulnton(z, n, nbegin)  " addze "Radc0(nbegin,n)","Radc0(nbegin,n)"      \n" /* adcq 0 + rdi + CB    */     
#define Rr(Max,n) BOOST_PP_STRINGIZE(BOOST_PP_CAT(%%r, BOOST_PP_ADD(BOOST_PP_SUB(15,Max),n))) // give register start at r8  
#define SAVEr_register(z, n, MAX)  " movq "Rr(MAX,BOOST_PP_ADD(n,1))", "PPS(VLI_AOS,n)"(%%rdi)    \n" /* save 0x??(%%rdi) */     
#define PPSr1(Max,n) BOOST_PP_STRINGIZE( BOOST_PP_MUL(BOOST_PP_SUB(Max,n),8)) // m*n*8, 8 because long int

#define Rr2(z, n, unused)  BOOST_PP_STRINGIZE(BOOST_PP_CAT(r, BOOST_PP_SUB(15,n))), // give register start at r8  

#define Rax(MAX,n) BOOST_PP_STRINGIZE(BOOST_PP_SUB(23,BOOST_PP_SUB(MAX,n))) // give register start at r15, r12, .... reverse order  
#define Rdx(MAX,n) BOOST_PP_STRINGIZE(BOOST_PP_SUB(23,BOOST_PP_SUB(BOOST_PP_SUB(MAX,1),n))) // give register start at r15, r12, .... reverse order  

// BOOST_PP_IF(n, " addc "Rax(niteration,n)","Rax(niteration,n)", 14 \n","addi. "Rax(niteration,n)",14,0 \n") 
// this is really ununderstable so be carefull if you modify 
#define MULNTON1(z, n, niteration) \
                 " ld 6,"PPS(1,n)"(3) \n" \
                 " mulld   14,5,6 \n"  \
                BOOST_PP_IF(BOOST_PP_SUB(niteration,n), " mulhdu   15,5,6 \n", /*nothing*/) \
                BOOST_PP_IF(n,BOOST_PP_IF(BOOST_PP_SUB(niteration,n)  , " addc "Rax(niteration,n)","Rax(niteration,n)", 14 \n","add "Rax(niteration,n)","Rax(niteration,n)", 14 \n"), "addi "Rax(niteration,n)",14,0 \n") \
                BOOST_PP_IF(n,BOOST_PP_IF(BOOST_PP_EQUAL(niteration,n), , " adde "Rdx(niteration,n)","Rdx(niteration,n)",15 \n"), BOOST_PP_IF(BOOST_PP_EQUAL(niteration,n), ,"addc "Rdx(niteration,n)","Rdx(niteration,n)",15 \n")) \
                BOOST_PP_REPEAT(BOOST_PP_SUB(niteration,BOOST_PP_ADD(n,1)), ADC0_register_mulnton, NUM1(niteration,n)) 

#define MULNTON0(z, n, MAX) \
                 " ld 5,"PPSr1(MAX,n)"(4) \n" \
                BOOST_PP_REPEAT(BOOST_PP_ADD(n,1), MULNTON1, n) 
