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

#ifndef __GPU_MANAGER__
#define __GPU_MANAGER__

#include <cuda.h>

#include "boost/lexical_cast.hpp"
#include <cuda_runtime_api.h>
#include <stdexcept>

namespace gpu {    
     static void cu_check_error(cudaError_t  const& err, char const* file,  std::size_t line) {
         if(err != cudaSuccess){
             char const* cuda_msg = cudaGetErrorString(err);
             throw(std::runtime_error("CUDA error (Error:"+ boost::lexical_cast<std::string>(err) +") in " + boost::lexical_cast<std::string>(file) + " line : " +  boost::lexical_cast<std::string>(line) + "\n cuda message: " + std::string(cuda_msg) ));
         }
     }

     static void cu_check_error_kernel(const char *message, const char* file, std::size_t line) {
         cudaError_t error = cudaGetLastError();
         if(error!=cudaSuccess) {
             throw(std::runtime_error("CUDA error (Error:"+ boost::lexical_cast<std::string>(message) +") in " + boost::lexical_cast<std::string>(file) + " line : " +  boost::lexical_cast<std::string>(line)  ));
         }                         
      }




}

#endif
