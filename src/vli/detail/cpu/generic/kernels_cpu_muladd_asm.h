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

#ifndef KERNELS_CPU_GENERIC_MULADD_ASM_H
#define KERNELS_CPU_GENERIC_MULADD_ASM_H

namespace vli {
    namespace detail{
    
                   template <std::size_t NumWords>
                   void muladd( uint64_t * x, uint64_t const* y, uint64_t const* z);
 
                   #define FUNCTION_mul_add(z, n, unused) \
                   template<>                      \
                   void muladd<n+2>(uint64_t* x, uint64_t const* y, uint64_t const* w){ \
                        mul<n+2>(x,y,w);   \
                   } \
        
                   BOOST_PP_REPEAT(VLI_THREE, FUNCTION_mul_add, ~)
                   
                   #undef FUNCTION_mul_add
           
        } //namespace detail
} //namespace vli

#endif

