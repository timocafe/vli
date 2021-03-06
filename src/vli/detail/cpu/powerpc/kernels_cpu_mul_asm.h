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
#ifndef KERNELS_CPU_POWERPC_MUL_ASM_H
#define KERNELS_CPU_POWERPC_MUL_ASM_H

// to check :  g++ -E -P -I /BOOST_PATH/include/ -I ../.. vli_number_cpu_function_hooks.hpp | sed  "s/n/;\\`echo -e '\n\r'`/g"  
namespace vli {
    namespace detail {
                     //new functions type : VLI<n*64> *= long int;
                    template <std::size_t NumWords>
                    void mul( uint64_t * x, uint64_t const b);

                     #define FUNCTION_mul_nbits_64bits(z, n, unused) \
                         template<> \
                         void mul<n+2>(uint64_t* x, uint64_t const b){           \
                         asm(                                                                                       \
                             "xor   6,6,6 \n" \
                             "ld    "R(0)", 0(3)   \n" /* x arg */                                                     \
                             "mr    "R(1)", 4   \n" /* y arg */                                                     \
                             "cmpi 0,0,"R(1)",0                  \n" /* rax is negative ? */             \
                             "bgt 0, "NAME_CONDITIONAL_MUL_NBITS_64BITS(n)"   \n" /* if statements begins */          \
                             "neg   "R(1)","R(1)"                              \n" /* negate the number */             \
                             "addi 6,6,1 \n" \
                             " "NAME_CONDITIONAL_MUL_NBITS_64BITS(n)" :     \n" /* putain de : */                   \
                             "mulld   "R(2)","R(0)","R(1)" \n" /*low part of the product */                           \
                             "mulhdu  "R(3)","R(0)","R(1)" \n" /*low part of the product */                           \
                             BOOST_PP_REPEAT(n, MUL_register, ~)               /* mul algo */                      \
                             "ld      "R(0)", "PPS(1,BOOST_PP_ADD(1,n))"(3) \n"                                       \
                             "mulld   5     ,"R(0)","R(1)" \n "                                                          \
                             "add  "R(BOOST_PP_ADD(3,n))", 5, "R(BOOST_PP_ADD(3,n))"  \n"  \
                             "cmpi 0,0,6,0                  \n" /* rax is negative ? */             \
                             "beq 0, "NAME_RES_CONDITIONAL_MUL_NBITS_64BITS(n)" \n" /* not equal ZF = 0, negate*/       \
                              BOOST_PP_REPEAT(BOOST_PP_ADD(n,2), NOT_register, ~) /* if for final sign */           \
                             "addic. 16,16, 1                 \n  " /* 2cm add 1 */                   \
                              BOOST_PP_REPEAT(BOOST_PP_ADD(n,1), ADC0_register, ~)/* propagate carry bit */         \
                             " "NAME_RES_CONDITIONAL_MUL_NBITS_64BITS(n)" : \n"   /* end final if */                \
                              BOOST_PP_REPEAT(BOOST_PP_ADD(n,2), STORE_register_r3mul,~)                                  \
                              : : :"r5","r6",BOOST_PP_REPEAT(BOOST_PP_ADD(n,4), CLOTHER_register, ~) "memory"   /* clother register*/      \
                             ); \
                         } \
 
                      BOOST_PP_REPEAT(7, FUNCTION_mul_nbits_64bits, ~) // 7 -> expand until 512 !
                      #undef FUNCTION_mul_nbits_64bits

                      // remark same than x86, look cpu_x86 for details
                       template <std::size_t NumWords>
                       void mul( uint64_t * x, uint64_t const* y);

                       #define BOOST_PP_LOCAL_MACRO(n) \
                          template<> \
                          void mul<n>(uint64_t* x, uint64_t const* y){ \
                         asm(                                                                                       \
                              BOOST_PP_REPEAT(n, MULNTON0, BOOST_PP_SUB(n,1))                                             \
                              BOOST_PP_REPEAT(n, STORE_register_r3mul2,BOOST_PP_ADD(n,2) )                     \
                              : : :"r5","r6","r14","r15",BOOST_PP_REPEAT(n, CLOTHER_register2,n ) "memory"   /* clother register*/      \
                             ); \
                         } \
                     
                       #define BOOST_PP_LOCAL_LIMITS (2, 8)

                       #include BOOST_PP_LOCAL_ITERATE() // the repetition, expand 128 -> 512
                       
                       #undef BOOST_PP_LOCAL_MACRO
                       #undef BOOST_PP_LOCAL_LIMITS

                       template <std::size_t NumWords>
                       void mul( uint64_t * x, uint64_t const* y, uint64_t const* z);
                    
                       template<>
                       void mul<1>(uint64_t* x/* %%rdi */, uint64_t const* y/* %%rsi */, uint64_t const* z/* %%rdx -> rcx */){
                          asm( 
                              "ld 14, 0(4)     \n"                   
                              "ld 15, 0(5)     \n"                   
                              "mulld  16,14,15 \n"  
                              "std 16 ,0(3)    \n"
                              "mulhdu 16,14,15 \n"  
                              "std 16 ,8(3)    \n"
                              : : :"r14","r15","r16"
                              );
                       }
          
                       template<>
                       void mul<2>(uint64_t* x/* %%rdi */, uint64_t const* y/* %%rsi */, uint64_t const* z/* %%rdx -> rbx */){
                          asm( 
                              "ld 14, 0(4)              \n"                   
                              "ld 15, 8(4)              \n"                   
                              "ld 16, 0(5)              \n"                   
                              "ld 17, 8(5)              \n"                   
                              "xor 22, 22, 22               \n"                   
                              "xor 23, 23, 23               \n"                   
                              "xor 18, 18, 18               \n"                   
                              "xor 19, 19, 19               \n"                   
                              "sradi 6,15,63   \n" 
                              "cmpi 0,0,6,0            \n"
                              "beq 0, _Negativea_256_128_  \n"
                              "not 14, 14               \n"
                              "not 15, 15               \n"
                              "addic. 14, 14, 1         \n"
                              "addze 15, 15             \n"
                              "addi 18, 18, 1          \n"
                              "_Negativea_256_128_ :    \n"               
                              "sradi 7,17,63   \n" 
                              "cmpi 0,0,7,0            \n"
                              "beq 0, _Negativeb_256_128_  \n"
                              "not 16, 16               \n"
                              "not 17, 17               \n"
                              "addic. 16, 16, 1         \n"
                              "addze  17, 17            \n"
                              "addi  19, 19, 1         \n"
                              "_Negativeb_256_128_ :    \n"               
                              /*----------------------- a0 * b0, a0 * b1 start ------------------------*/ 
                              "mulld 20 ,14,16          \n" 
                              "mulhdu 21 ,14,16          \n" 
                              "mulld 6  ,15,16          \n" 
                              "mulhdu 7  ,15,16          \n" 
                              "addc  21 ,21,6           \n"
                              "addze 22 ,7              \n"
                              /*----------------------- a0 * b0, a0 * b1 end --------------------------*/ 
                              /*----------------------- a1 * b0, a1 * b1 start ------------------------*/ 
                              "mulld 6 ,14,17          \n" 
                              "mulhdu 7 ,14,17          \n" 
                              "addc  21 ,21,6          \n"
                              "adde  22 ,22,7          \n"
                              "addze 23 ,23            \n"
                              "mulld 6 ,15,17          \n" 
                              "mulhdu 7 ,15,17          \n" 
                              "addc  22 ,22,6           \n"
                              "adde 23 ,23,7          \n"
                              /*----------------------- a1 * b0, a1 * b1 end   ------------------------*/ 
                              "xor 18, 19, 18               \n"                   
                              "cmpi 0,0,18,0            \n"
                              "beq 0, _IsNegativeResult_256_128_  \n"     
                              "not 20  , 20             \n"
                              "not 21  , 21             \n"
                              "not 22  , 22             \n"
                              "not 23  ,23              \n"
                              "addic. 20, 20, 1         \n"
                              "addze  21, 21            \n"
                              "addze  22, 22            \n"
                              "addze  23, 23            \n"
                              "_IsNegativeResult_256_128_ :  \n"       
                              "std 20, 0(3)             \n"
                              "std 21, 8(3)             \n"
                              "std 22, 16(3)             \n"
                              "std 23, 24(3)             \n"
                              : : : "r6","r7","r14","r15","r16","r17","r18","r19","r20","r21","r22","r23","memory"   
                             );

                       }
         
                      template<>
                      void mul<3>(uint64_t* x/* %%rdi */, uint64_t const* y/* %%rsi */, uint64_t const* z/* %%rdx -> rbx */){
                          asm( 
                              "ld 14, 0(4)              \n"                   
                              "ld 15, 8(4)              \n"                   
                              "ld 16, 16(4)              \n"                   
                              "ld 17, 0(5)              \n"                   
                              "ld 18, 8(5)              \n"                   
                              "ld 19, 16(5)              \n"                   
                              "xor 9, 9, 9               \n"                   
                              "xor 8, 8, 8               \n"                   
                              "xor 23, 23, 23               \n"                   
                              "xor 24, 24, 24               \n"                   
                              "xor 25, 25, 25               \n"                   
                              "sradi 6,16,63   \n" 
                              "cmpi 0,0,6,0            \n"
                              "beq 0, _Negativea_384_192_  \n"
                              "not 14, 14               \n"
                              "not 15, 15               \n"
                              "not 16, 16               \n"
                              "addic. 14, 14, 1         \n"
                              "addze 15, 15             \n"
                              "addze 16, 16             \n"
                              "addi 8, 8, 1          \n"
                              "_Negativea_384_192_ :    \n"               
                              "sradi 7,19,63   \n" 
                              "cmpi 0,0,7,0            \n"
                              "beq 0, _Negativeb_384_192_  \n"
                              "not 17, 17               \n"
                              "not 18, 18               \n"
                              "not 19, 19               \n"
                              "addic. 17, 17, 1         \n"
                              "addze  18, 18            \n"
                              "addze  19, 19            \n"
                              "addi  9, 9, 1         \n"
                              "_Negativeb_384_192_ :    \n"               
                              /*----------------------- start ------------------------*/ 
                              "mulld 20 ,14,17          \n" 
                              "mulhdu 21 ,14,17          \n" 
                              "mulld 6  ,15,17          \n" 
                              "mulhdu 7  ,15,17          \n" 
                              "addc  21 ,21,6           \n"
                              "addze 22 ,7              \n"
                              "mulld 6  ,16,17          \n" 
                              "mulhdu 7  ,16,17          \n" 
                              "addc  22 ,22,6           \n"
                              "addze 23 ,7              \n"
                              /*-----------------------  end --------------------------*/ 
                              /*-----------------------  start ------------------------*/ 
                              "mulld 6 ,14,18          \n" 
                              "mulhdu 7 ,14,18          \n" 
                              "addc  21 ,21,6          \n"
                              "adde  22 ,22,7          \n"
                              "addze 23 ,23            \n"
                              "mulld 6 ,15,18          \n" 
                              "mulhdu 7 ,15,18          \n" 
                              "addc  22 ,22,6           \n"
                              "adde 23 ,23,7          \n"
                              "addze 24 ,24            \n"
                              "mulld 6 ,16,18          \n" 
                              "mulhdu 7 ,16,18          \n" 
                              "addc  23 ,23,6           \n"
                              "adde 24 ,24,7          \n"
                              "addze 25 ,25            \n"
                              /*-----------------------  end   ------------------------*/ 
                              /*-----------------------  start ------------------------*/ 
                              "mulld 6 ,14,19          \n" 
                              "mulhdu 7 ,14,19          \n" 
                              "addc  22 ,22,6          \n"
                              "adde  23 ,23,7          \n"
                              "addze 24 ,24            \n"
                              "addze 25 ,25            \n"
                              "mulld 6 ,15,19          \n" 
                              "mulhdu 7 ,15,19          \n" 
                              "addc  23 ,23,6           \n"
                              "adde 24 ,24,7          \n"
                              "addze 25 ,25            \n"
                              "mulld 6 ,16,19          \n" 
                              "mulhdu 7 ,16,19          \n" 
                              "addc 24 ,24,6           \n"
                              "adde 25 ,25,7          \n"
                              /*----------------------- a1 * b0, a1 * b1 end   ------------------------*/ 
                              "xor 8, 8, 9               \n"                   
                              "cmpi 0,0,8,0            \n"
                              "beq 0, _IsNegativeResult_384_192_  \n"     
                              "not 20  ,20             \n"
                              "not 21  ,21             \n"
                              "not 22  ,22             \n"
                              "not 23  ,23              \n"
                              "not 24  ,24              \n"
                              "not 25  ,25              \n"
                              "addic. 20, 20, 1         \n"
                              "addze  21, 21            \n"
                              "addze  22, 22            \n"
                              "addze  23, 23            \n"
                              "addze  24, 24            \n"
                              "addze  25, 25            \n"
                              "_IsNegativeResult_384_192_ :  \n"       
                              "std 20, 0(3)             \n"
                              "std 21, 8(3)             \n"
                              "std 22, 16(3)             \n"
                              "std 23, 24(3)             \n"
                              "std 24, 32(3)             \n"
                              "std 25, 40(3)             \n"
                              : : : "r6","r7","r8","r9","r14","r15","r16","r17","r18","r19","r20","r21","r22","r23","r24","r25","memory"   
                              );
                            }

                     template<>
                     void mul<4>(uint64_t* x/* %%rdi */, uint64_t const* y/* %%rsi */, uint64_t const* z/* %%rdx -> rbx */){
                            asm(
                              "ld 14, 0(4)              \n"                   
                              "ld 15, 8(4)              \n"                   
                              "ld 16, 16(4)              \n"                   
                              "ld 17, 24(4)              \n"                   
                              "ld 18, 0(5)              \n"                   
                              "ld 19, 8(5)              \n"                   
                              "ld 20, 16(5)              \n"                   
                              "ld 21, 24(5)              \n"                   
                              "xor 9, 9, 9               \n"                   
                              "xor 8, 8, 8               \n"                   
                              "xor 24, 24, 24               \n"                   
                              "xor 25, 25, 25               \n"                   
                              "xor 26, 26, 26               \n"                   
                              "xor 27, 27, 27               \n"                   
                              "xor 28, 28, 28               \n"                   
                              "xor 29, 29, 29               \n"                   
                              "sradi 6,17,63   \n" 
                              "cmpi 0,0,6,0            \n"
                              "beq 0, _Negativea_512_256_  \n"
                              "not 14, 14               \n"
                              "not 15, 15               \n"
                              "not 16, 16               \n"
                              "not 17, 17               \n"
                              "addic. 14, 14, 1         \n"
                              "addze 15, 15             \n"
                              "addze 16, 16             \n"
                              "addze 17, 17             \n"
                              "addi 8, 8, 1          \n"
                              "_Negativea_512_256_ :    \n"               
                              "sradi 7,21,63   \n" 
                              "cmpi 0,0,7,0            \n"
                              "beq 0, _Negativeb_512_256_  \n"
                              "not 18, 18               \n"
                              "not 19, 19               \n"
                              "not 20, 20               \n"
                              "not 21, 21               \n"
                              "addic. 18, 18, 1         \n"
                              "addze  19, 19            \n"
                              "addze  20, 20            \n"
                              "addze  21, 21            \n"
                              "addi  9, 9, 1         \n"
                              "_Negativeb_512_256_ :    \n"               
                              /*----------------------- start ------------------------*/ 
                              "mulld 22 ,14,18          \n" 
                              "mulhdu 23 ,14,18          \n" 
                              "mulld 6  ,15,18          \n" 
                              "mulhdu 7  ,15,18          \n" 
                              "addc  23 ,23,6           \n"
                              "addze 24 ,7              \n"
                              "mulld 6  ,16,18          \n" 
                              "mulhdu 7  ,16,18          \n" 
                              "addc  24 ,24,6           \n"
                              "addze 25 ,7              \n"
                              "mulld 6  ,17,18          \n" 
                              "mulhdu 7  ,17,18          \n" 
                              "addc  25 ,25,6           \n"
                              "addze 26 ,7              \n"
                              /*-----------------------  end --------------------------*/ 
                              /*-----------------------  start ------------------------*/ 
                              "mulld 6 ,14,19          \n" 
                              "mulhdu 7 ,14,19          \n" 
                              "addc  23 ,23,6          \n"
                              "adde  24 ,24,7          \n"
                              "addze 25 ,25            \n"
                              "addze 26 ,26            \n"
                              "addze 27 ,27            \n" // maybe 1 in 27
                              "mulld 6 ,15,19          \n" 
                              "mulhdu 7 ,15,19          \n" 
                              "addc  24 ,24,6           \n"
                              "adde 25 ,25,7          \n"
                              "addze 26 ,26            \n"
                              "addze 27 ,27            \n"
                              "mulld 6 ,16,19          \n" 
                              "mulhdu 7 ,16,19          \n" 
                              "addc  25 ,25,6           \n"
                              "adde 26 ,26,7          \n"
                              "addze 27 ,27            \n"
                              "mulld 6 ,17,19          \n" 
                              "mulhdu 7 ,17,19          \n" 
                              "addc  26 ,26,6           \n"
                              "adde 27 ,27,7          \n"
                              "addze 28 ,28            \n"// maybe 1 in 28
                              /*-----------------------  end   ------------------------*/ 
                              /*-----------------------  start ------------------------*/ 
                              "mulld 6 ,14,20          \n" 
                              "mulhdu 7 ,14,20          \n" 
                              "addc  24 ,24,6          \n"
                              "adde  25 ,25,7          \n"
                              "addze 26 ,26            \n"
                              "addze 27 ,27            \n"
                              "addze 28 ,28            \n"
                              "mulld 6 ,15,20          \n" 
                              "mulhdu 7 ,15,20          \n" 
                              "addc  25 ,25,6           \n"
                              "adde 26 ,26,7          \n"
                              "addze 27 ,27            \n"
                              "addze 28 ,28            \n"
                              "mulld 6 ,16,20          \n" 
                              "mulhdu 7 ,16,20          \n" 
                              "addc  26 ,26,6           \n"
                              "adde 27 ,27,7          \n"
                              "addze 28 ,28            \n"
                              "mulld 6 ,17,20          \n" 
                              "mulhdu 7 ,17,20          \n" 
                              "addc  27 ,27,6           \n"
                              "adde 28 ,28,7          \n"
                              "addze 29 ,29            \n"
                              /*-----------------------  end   ------------------------*/ 
                              /*-----------------------  start ------------------------*/ 
                              "mulld 6 ,14,21          \n" 
                              "mulhdu 7 ,14,21          \n" 
                              "addc  25 ,25,6          \n"
                              "adde  26 ,26,7          \n"
                              "addze 27 ,27            \n"
                              "addze 28 ,28            \n"
                              "addze 29 ,29            \n"
                              "mulld 6 ,15,21          \n" 
                              "mulhdu 7 ,15,21          \n" 
                              "addc  26 ,26,6           \n"
                              "adde 27 ,27,7          \n"
                              "addze 28 ,28            \n"
                              "addze 29 ,29            \n"
                              "mulld 6 ,16,21          \n" 
                              "mulhdu 7 ,16,21          \n" 
                              "addc  27 ,27,6           \n"
                              "adde 28 ,28,7          \n"
                              "addze 29 ,29            \n"
                              "mulld 6 ,17,21          \n" 
                              "mulhdu 7 ,17,21          \n" 
                              "addc  28 ,28,6           \n"
                              "adde 29 ,29,7          \n"
                              /*----------------------- end   ------------------------*/ 
                              "xor 8, 8, 9               \n"                   
                              "cmpi 0,0,8,0            \n"
                              "beq 0, _IsNegativeResult_512_256_  \n"     
                              "not 22  , 22             \n"
                              "not 23  , 23             \n"
                              "not 24  , 24             \n"
                              "not 25  , 25             \n"
                              "not 26  , 26             \n"
                              "not 27  , 27             \n"
                              "not 28  , 28             \n"
                              "not 29  , 29             \n"
                              "addic. 22, 22, 1         \n"
                              "addze  23, 23            \n"
                              "addze  24, 24            \n"
                              "addze  25, 25            \n"
                              "addze  26, 26            \n"
                              "addze  27, 27            \n"
                              "addze  28, 28            \n"
                              "addze  29, 29            \n"
                              "_IsNegativeResult_512_256_ :  \n"       
                              "std 22, 0(3)             \n"
                              "std 23, 8(3)             \n"
                              "std 24, 16(3)             \n"
                              "std 25, 24(3)             \n"
                              "std 26, 32(3)             \n"
                              "std 27, 40(3)             \n"
                              "std 28, 48(3)             \n"
                              "std 29, 56(3)             \n"
                              : : : "r6","r7","r8","r9","r14","r15","r16","r17","r18","r19","r20","r21","r22","r23","r24","r25","r26","r27","r28","r29","memory"   
                              );
                     }
    } // end namespace detail
} // end namespace vli

#endif
