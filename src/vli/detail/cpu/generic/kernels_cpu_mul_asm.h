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
#ifndef KERNELS_CPU_GENERIC_MUL_ASM_H
#define KERNELS_CPU_GENERIC_MUL_ASM_H

namespace vli{
    namespace detail{
                       template <std::size_t NumWords>
                       void mul( uint64_t * x, uint64_t const* y);

                       #define FUNCTION_mul_nbits_nbits(z, n, unused) \
                          template<>                                                                                         \
                          void mul<n+2>( uint64_t* x,  uint64_t const* y){                                     \
                            boost::uint32_t* cy = const_cast<boost::uint32_t*>(reinterpret_cast<boost::uint32_t const*>(y)); \
                            boost::uint32_t* cx = reinterpret_cast<boost::uint32_t*>(x);                                     \
                            boost::uint32_t tmp[2*(n+2)];                                                                    \
                            memset((void*)tmp,0,(n+2)*sizeof(uint64_t));                                              \
                            uint64_t new_val, carry(0);                                                               \
                            for(int i(0); i < 2*(n+2);++i){                                                                  \
                                carry = 0;                                                                                   \
                                for(int j(0); j < 2*(n+2)-i;++j){                                                            \
                                   new_val = static_cast<uint64_t>(cx[i])*static_cast<uint64_t>(cy[j]) + static_cast<uint64_t>(tmp[i+j]) + carry; \
                                   tmp[i+j] = static_cast<boost::uint32_t>(new_val);                                         \
                                   carry = new_val >> 32;                                                                    \
                                } \
                            } \
                            memcpy((void*)x,(void*)tmp,(n+2)*sizeof(uint64_t)); \
                       } \
                       
                       BOOST_PP_REPEAT(VLI_MAX_ITERATION, FUNCTION_mul_nbits_nbits, ~)
                       
                       #undef FUNCTION_mul_nbits_nbits
        
                       template <std::size_t NumWords>
                       void mul( uint64_t * x, uint64_t const b);
        
                       #define FUNCTION_mul_nbits_64bits(z, n, unused)                           \
                           template<>                                                            \
                           void mul<(n+2)>( uint64_t* x,  uint64_t const b){       \
                              uint64_t tmp[n+2];                                          \
                              tmp[0] = b;                                                        \
                             uint64_t mask = -(b >> 63);                                  \
                             for( int i = 1; i < n+2; ++i)                                       \
                                  tmp[i] = mask;                                                 \
                              mul<n+2>(x,tmp);                                                   \
                           }
                   
                       BOOST_PP_REPEAT(VLI_MAX_ITERATION, FUNCTION_mul_nbits_64bits, ~)
                       
                       #undef FUNCTION_mul_nbits_64bits
    
                      template <std::size_t NumWords>
                      void mul(uint64_t * x, uint64_t const* y, uint64_t const* w);
        
                      #define FUNCTION_mul_2nbits_nbits_nbits(z, n, unused) \
                          template<>                      \
                          void mul<(n+2)>(uint64_t * x, uint64_t const* y, uint64_t const* w){ \
                              boost::uint32_t* cx = reinterpret_cast<boost::uint32_t*>(x);                          \
                              boost::uint32_t cy[4*(n+2)];                                                          \
                              boost::uint32_t cw[4*(n+2)];                                                          \
                              boost::uint32_t masky = -(y[(n+2)-1] >> 63);                                          \
                              boost::uint32_t maskw = -(w[(n+2)-1] >> 63) ;                                         \
                              for( int i = 2*(n+2) ; i < 4*(n+2); ++i){                                             \
                                  cy[i] = masky;                                                                    \
                                  cw[i] = maskw;                                                                    \
                              }                                                                                     \
                              memcpy((void*)cy,(void*)y,(n+2)*sizeof(uint64_t));                             \
                              memcpy((void*)cw,(void*)w,(n+2)*sizeof(uint64_t));                             \
                              uint64_t new_val, carry(0);                                                    \
                              for(int i(0); i < 4*(n+2);++i){                                                       \
                                  carry = 0;                                                                        \
                                  for(int j(0); j < 4*(n+2)-i;++j){                                                 \
                                     new_val = static_cast<uint64_t>(cw[i])*static_cast<uint64_t>(cy[j]) + static_cast<uint64_t>(cx[i+j]) + carry; \
                                     cx[i+j] = static_cast<boost::uint32_t>(new_val);                               \
                                     carry = new_val >> 32;                                                         \
                                  }                                                                                 \
                              }                                                                                     \
                              }
        
                      BOOST_PP_REPEAT(VLI_THREE, FUNCTION_mul_2nbits_nbits_nbits, ~)
                      
                      #undef FUNCTION_mul_2nbits_nbits_nbits
        
    } // end namespace detail
} // end namespace vli

#endif
