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

#ifndef VLI_KARATSUBA_ASM_H
#define VLI_KARATSUBA_ASM_H

#include <stdio.h>
namespace vli {
    namespace detail {

    template <std::size_t NumBits> 
    __device__ void KA_add(boost::uint32_t* x, boost::uint32_t const* y){
        asm volatile( "add.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(x[0]):"r"(y[0])); 
        #pragma unroll
        for(int i=1; i < (NumBits/32); ++i)
            asm volatile( "addc.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(x[i]):"r"(y[i])); 
    }

    template <std::size_t NumBits>
    __device__ void KA_add_extend(boost::uint32_t* x, boost::uint32_t const* y){
        asm volatile( "add.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(x[0]):"r"(y[0])); 
        #pragma unroll
        for(int i=1; i < (NumBits/32); ++i)
            asm volatile( "addc.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(x[i]):"r"(y[i])); 

        asm volatile( "addc.u32  %0 , %0 , 0 ; \n\t" : "+r"(x[NumBits/32]):); 
    };

    template <std::size_t NumBits>
    __device__ void KA_sign_zero_extend(boost::uint32_t* x,  boost::uint32_t mask){
        for(int i(0); i <= (NumBits/32); ++i) 
            x[i] ^= mask; 
        mask >>= 31; 
        asm volatile( "add.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(x[0]):"r"(mask)); 
        #pragma unroll
        for(int i=1; i <= (NumBits/32); ++i)
            asm volatile ( "addc.cc.u32  %0 , %0 , 0 ; \n\t" : "+r"(x[i]):);
    }

    template <std::size_t NumBits>
    __device__ boost::uint32_t KA_sub_b(boost::uint32_t* x, boost::uint32_t const* y){
        boost::uint32_t sign(0); // attention %1 %0 ou %0 %1 
        asm volatile( "sub.cc.u32  %0 , %1 , %0 ; \n\t" : "+r"(x[0]):"r"(y[0])); 
        #pragma unroll
        for(int i=1; i < (NumBits/32); ++i)
            asm volatile( "subc.cc.u32  %0 , %1 , %0 ; \n\t" : "+r"(x[i]):"r"(y[i])); 

        asm volatile( "subc.u32  %0 , 0 , 0 ; \n\t" : "=r"(sign):); // constraint = should be enough 
        return sign; 
    }; 


    template <std::size_t NumBits>
    __device__ void KA_sign(boost::uint32_t* x, boost::uint32_t mask){
        for(int i(0); i < (NumBits/32); ++i) 
            x[i] ^= mask; 
        mask >>= 31; 
        asm volatile( "add.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(x[0]):"r"(mask)); 
        #pragma unroll
        for(int i=1; i < (NumBits/32); ++i)
            asm volatile( "addc.cc.u32  %0 , %0 , 0 ; \n\t" : "+r"(x[i]):);
    };

//  A less recursive variant of Karatsuba-Ofman algorithm for Multiplying Operands of Size a Power of Two
//  Swedar S. Erdem Cetin Koc, coded section 4 Efficient Implementation of KOA
    template <std::size_t NumBits>
    __device__ void  KA(boost::uint32_t* res0, boost::uint32_t const*  a, boost::uint32_t const*  b){
            boost::uint32_t am[NumBits/64];
            boost::uint32_t bm[NumBits/64];
            boost::uint32_t medium[NumBits/32+1];

            #pragma unroll
            for(int i=0; i <= NumBits/32; ++i)
                medium[i] = 0;

            // step 3 and 4 not coded I direct use the data a and b 
            #pragma unroll
            for(int i=0; i < NumBits/64; ++i){
                am[i] = a[i]; // step 2
                bm[i] = b[i+NumBits/64]; // step 5
            }
             
            boost::uint32_t mask_s1 = KA_sub_b<NumBits/2>(am,(a+NumBits/64)); // step 6, am = al - ah, ( ah = a + NumBits/128 arith ptr), mask is the sign 0 positif, -1 (0xffff .... ) negatif
            boost::uint32_t mask_s2 = KA_sub_b<NumBits/2>(bm,b); // step 7, bm = bh - bl ( bl = b), mask the sign again
           
            KA_sign<NumBits/2>(am,mask_s1); // step 6 next, it is just the absolute value
            KA_sign<NumBits/2>(bm,mask_s2); // step 7 next, absolute value again

            KA<NumBits/2>(res0,a,b); // step 8
            KA<NumBits/2>((res0+(2*NumBits)/64),(a+NumBits/64),(b+NumBits/64)); // step 9
            KA<NumBits/2>(medium,am,bm); // step 10

            KA_sign_zero_extend<NumBits>(medium, (mask_s1^mask_s2)); // 0 +- medium, in both case I do 0 + Two Complementary method on medium, for the postive version it changes nothings, step 11a (+- medium)
            KA_add_extend<NumBits>(medium,res0); //  step 11a next
            KA_add_extend<NumBits>(medium,(res0+NumBits/32)); //  step 11a next
            KA_add<NumBits+32>((res0+(NumBits/32)/2),(medium));  // 11b

    }
    // I benchmark Karatusba is only faster for 256 * 256 = 512 bits    
    template <> // if 64 -> mul_extend<128>, 128 -> mul_extend<256>
    __device__ void KA<128>(boost::uint32_t* res0, boost::uint32_t const*  a, boost::uint32_t const*  b){//
       vli::detail::mul_extend<256>(res0,a,b);
      /*  32 bits version
           asm(
               "mul.lo.u32  %0, %2, %3; \n\t"
               "mul.hi.u32  %1, %2, %3; \n\t"
                :"=r"(res0[0]),"=r"(res0[1]):"r"(a[0]), "r"(b[0])
           );
     */
    }
        
    template <std::size_t NumBits>
    __device__ void karatsuba(boost::uint32_t* res, boost::uint32_t const* a, boost::uint32_t const* b){
     KA<NumBits>(res,a,b);
    }
    
    }
}
#endif
