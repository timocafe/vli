# Very Large Integer
VLI is a library written in C++ for high but fixed precision (128 to 512-bit) arithmetic 
and symbolic polynomials computations. The speed of VLI is achieved by using standard algorithms, 
but based on a stack allocation approach with highly optimized assembly language core kernels for 
X86-64 and Power64 architecture. Moreover, VLI performs tunable hybrid CPU/GPU inner product function 
using OpenMP and NVIDIA® CUDA™ with inline PTX assembly.

Contrary to the well know GMP, VLI does not adjust the bit precision during the execution time. 
VLI can give a great improvement of the performance, as the dynamic size poses a significant overhead 
and much more optimized implementations are possible with fixed size.

INSTALLING VLI

With thousand of lines of assembly langage, the library is limited to X86 and Power64 system. 
The library has been successfully tested on various system AMD64 (Bulldozer, Istanbul), X86-64 
(Sandy-Bridge, Westmere) and power system (Power6, Power7, BGQ). The requirements are :

x86-64 or Power64 architecture
C++ compiler supporting fully GCC inline assembly langage e.g. GCC, Clang, Intel
cmake for generating makefile
boost include and the libraries boot::test and boost::random (Min 1.49)
CUDA™ NVIDIA® graphic card supporting CUDA™ 4.2 for hybrid inner product only (optional)
Doxygen to generate the doc (optional)
GMP for the regression tests (optional)
makefiles are generated with cmake, if your environment is correctly set up:
    cd vli
    mkdir build
    cd build
    cmake ..
If the cmake processing runs well, you may open the cmake options (ccmake .) and plug the GPU, 
setup install path, regression tests and so on. You can now compile the library. We recommand to compile with -O2.

    make
    
When the library is compiled, before installation, we encouraged the execution of the regression
tests (you need GMP) and the generation of the documentation:

    make test
    make doc

And you can install (under /usr/local by default) with

    make install


NOMENCLATURE AND TYPES

The library provides two classes for large integer and polynomial modeling: class integer and class polynomial.
Both classes have the same vli namespace.

For the integers, a vli::integer means a multiple precision signed integer. The size of the integer is a 
template parameter and it must be a multiple of 64-bit between [128-512] bit.

    vli::integer<192> a;
    vli::integer<460> b; 

The polynomials are represented by a template class vli::polynomial

    polynomial< CoeffType, Structure, Var0, Var1, Var2, Var3 > 
The CoeffType can be a vli::integer, a standard int, a gmp type (mpz_class) and so on, or any class which 
respect standard operator rules (+=, *, ...). The structure<N> specifies the type of polynomials having 
either a dense (i,j,k,l ≤ N) or a triangular (i+j+k+l ≤ N) structure (N is the order). The structures are 
declared by vli::max_order_each (dense polynomial) and vli::max_order_combined (triangular polynomial). 
Finally, the Var indicates the variables of the polynomial, it must be declared like vli::var<'x'> for a x variable.

To summarize, a dense polynomials of one variable, order 10 with 128-bit coefficient is declared by:

    typedef vli::integer<128> integer;
    vli::polynomial< integer, vli::max_order_each<10>, vli::var<'x'>  > p; 

whereas the equivalent triangular polynomial will be:

    typedef vli::integer<128> integer;
    vli::polynomial< integer, vli::max_order_combined<10>, vli::var<'x'>  > p; 
   
   HEADERS AND LIBRARY
   
   The include are vli/integer.hpp and vli/polynomial.hpp

    #include "vli/integer.hpp"
    #include "vli/polynomial.hpp" 
    
All programs using VLI must be link against the libvli library, as usual on linux system, it should be done with -lvli. 
If the library is installed in the standard location

    g++  my_program.cpp -lvli
 
 If VLI has been installed to a non-standard location then it may be necessary to use -I and -L compiler
options to point to the right directories.

The first tutorial illustrates the construction of VLI numbers. A basic example is:

    #include <iostream>
    #include "vli/integer.hpp"

    int main(int argc, char * argv[]){
        vli::integer<128> a;
        std::cout << a << std::endl;
        std::cout << std::hex << a <<  std::endl;
    }
The output will be:

    0 // for the decimal form
    0 0 // for the hexadecimal form, by default the hexadecimal prints
        // all entries of the large integer.
This example is not very significative, considering now the following initialization of the VLI number a:

    vli::integer<192> a(1);
The VLI library will fit the number into a multiple of 64-bit, thus the container will be 192-bit. The print under hexadecimal mode gives (little endian by default):

    0 0 1         
We can create a negative vli::integer:

    vli::integer<128> a(-1);
or alternatively,

    vli::integer<128> a;
    a = -1;
Negative numbers are constructed with the two complementary method. Thus, the hexadecimal print will be 

    0xffffffffffffffff 0xffffffffffffffff

The last possibility to initiate a number is the operators brackets [], as they allow a direct access to the container.

    vli::integer<256> a;
    a[0] = 1;
    a[2] = 3;
it will give

    0x0 0x3 0x0 0x1
Constant size arithmetic operations
The second tutorial illustrates the arithmetic operations between large integer numbers. Considering, the following program:

    vli::integer<128> a(2),b(3); // I create two VLI numbers of 128-bit
    a+=b; // We make the sum The output gives 5. But if we consider the inputs:
    integer<128> a,b(3);
    a[0] = 0xffffffffffffffff; // fill up the 64-bit first digits to one; The print shows the propagation of the carry bit.
    18 446 744 073 709 551 618 // for the decimal form
    0x1 0x2 // for the hexadecimal form This last operation can be rewritten under the followings form
    vli::integer<128> a,b(3);
    a[0] = -1;
    c = a+b; // less performable, it necessitates an extra copy due to operator=
    a += 3; // alternative if the right value fits into 64-bit. In both case a and c will have the same value. The same think can be applied for subtraction

    vli::integer<128> a(1),b(3);
    a -= b; We will obtain
    -2 // decimal form
    0xffffffffffffffff 0xfffffffffffffffe // hexadecimal form

For the optimuma performances, the user should privilege for the addition operations with the following order.

integer<NumBits> += long
integer<NumBits> += integer<NumBits>
integer<NumBits> = integer<NumBits> + integer<NumBits>
In the three cases the ASM solvers are not the same and they can necessitate an extra work. The same rules are true for the subtraction, because the solvers differ only from one pair of mnemonic instructions (add/adc becomes sub/sbb).

The next operations are the multiplication (*) and the division (/). Multiply two integer<NumBits> may give an integer twice larger integer<2*NumBits>. Contrary to GMP, the VLI library does not adjust the size of the vli::integer. Consequently, we must distinguish the two possibilities. First, the operators *= and * conserve the initial type.

    vli::integer<128> a(0xfffffffffffffff),b(0xfffffffffffffff); // fill up 63-bit
    a *= b; // I will get a 126-bit number, it fits into 128 bit number gives,
    0xffffffffffffff 0xe000000000000001
    
If now, we define a and b with the following initialization (1 0xffffffffffffffff). The multiplication gives (fffffffffffffffc 1) which is wrong. The correct result is (5 fffffffffffffffc 1). The library does not provide any safeties for the multiplication contraty to GMP. In terms of performance, the multiplication respects the same ladder than the additions/subtractions. The possibilities of the extended multiplications will be presented in the next tutorial.

To finish this tutorial, we give some words on the division. The division is closed to the % operator as it use the same functions. The solvers are generic whatever the left/right value of the operators, thus the execution time is the same e.g. for the operations integer<NumBits> /= , %= integer<NumBits> and integer<NumBits>/=, %=long.

Non-constant size arithmetic operations
The third tutorial illustrates the arithmetic operations which produce larger output than input. By definition a multiplication can generate a twice larger operand. Considering the two 128 bit large integers a and b, initialized under the following form (0x0fffffffffffffff, 0x0fffffffffffffff), the produce of the operands will give an integer with a size of 256-bit. The extend multiplication is performed by the vli::multiply_extend free function;

Considering the following example :
    vli::integer<128> a,b;
    vli::integer<256> c; // a and b inputs, c ouputs
    a[0] = a[1] = b[0] = b[1] = 0xfffffffffffffff;
    vli::multiply_extend(c,a,b);
    std::cout << std::hex << c << std::endl; produces
    0xffffffffffffff 0xe200000000000000 0xc100000000000001 0xe000000000000001 Similarly, large integers can be negative.
    vli::integer<128> a,b;
    vli::integer<256> c; // a and b inputs, c ouputs
    a[0] = b[0] = b[1] = 0xfffffffffffffff; // 15 times f
    a[1] = 0xffffffffffffffff // 16 times f
    vli::multiply_extend(c,a,b);
    std::cout << std::hex << c << std::endl; gives
   0xffffffffffffffff 0xf100000000000000 0xd100000000000001 0xe000000000000001
We summarize the possibilities of the arithmetic operations on the vli::integer into the next table.

bit ops	+	-	*	/	multiply_extend	multiply_add
integer <= 256-bit	yes	yes	yes	yes	yes	yes	yes
256-bit > integer	yes	yes	yes	yes	yes	no	no
Polynomials and polynomial arithmetics
The library allows the utilization of polynomial up to 4 variables. The polynomials are defined under the following forms :

The polynomials can be "dense" (i,j,k,l ≤ N) or "triangular" (i+j+k+l ≤ N), with various type of coefficient 
(vli::integer, basic type, GMP::mpz_class ...) moreover the libary allows also monomials which are automatically 
integrated with the polynomial include.

    #include "vli/polynomial.hpp"
The vli::polynomial must be instantiated over the coefficient type, polynomial type (dense or triangular) and
the symbolic name of the variables. To illustrate this functionnality, we consider a dense polynomial on one
variable where coefficients are assimilated to a vli::integer<128>, we have :

    typedef vli::integer<128> integer_type_cpu_128; // New type of 128-bit integer
    typedef vli::polynomial< integer_type_cpu_128, vli::max_order_each<10>,
        vli::var<'x'>  >polynomial_type_each_x_128;
        // dense polynomial, order 10, one variable "x"
            
    polynomial_type_each_x_128 p;
        // Create the polynomial with null coefficients
The initialization of the coefficients of the polynomial respect the rule of the coefficient class (here a vli::integer).
In the example, it can be attained with the operator () and =,

    p(0) = 1;
If the polynomial has more then one variable, the brackets operator will fit until four variables. Thus,
the polynomial will be initialized by:

    p(0,0) = 1;
Monomials are constructed like polynomial but without the dense/triangular structure, and they are initialized as usual

    typedef vli::integer<128> integer_type_cpu_128; // New type of 128-bit integer
    typedef vli::monomial< integer_type_cpu_128, vli::var<'x'>  >monomial_x_128;
            // monomial, one variable "x"
    monomial_x_128 m(2);
The polynomial class allows arithmetic operations with constants, monomials and polynomials. Basic operations 
respect elementary mathematic rules. Per example, polynomials interaction with a monomial and a constant can be done by

    polynomial_type_each_x_128 p;  //Polynomial class is std compatible
    std::generate(p1.begin(),p1.end(),Generator)
    monomial_x_128 m(3);
    p += m;
    p *= m;
    p *= 3;
The polynomial allows only arithmetic operations (addition and substraction) between polynomials of the same type, 
necessary. If we make the sum of polynomial of one variables, we write

    polynomial_type_each_x_128 p1,p2;
    std::generate(p1.begin(),p1.end(),Generator)
    std::generate(p2.begin(),p2.end(),Generator)
    p1 += p2; //make the sum
The polynomial multiplications need a twice larger (Order and Size of coefficient) result polynomial. The polynomial 
class has a trait class which gives the exact result type of the polynomial multiplication.

    typedef typename vli::polynomial_multiply_result_type::type polynomial_res;
Thus, the multiplication between two polynomials is performed by

    polynomial_type_each_x_128 p1,p2;
    polynomial_res pr
    std::generate(p1.begin(),p1.end(),Generator) //Polynomials are std compatible
    std::generate(p2.begin(),p2.end(),Generator) //Polynomials are std compatible
    pr = p1 * p2;
GPU accelerator for inner products
The last tutorial presents the possibility of the library to perform efficient inner product by multhreading,
GPU accelerator or hybrid-mode. The library allows a module for vector of polynomial by the following include.

    #include "vli/vector.hpp"
As previously, with the usual rule of template, we can define a vector of polynomial,

    typedef vli::integer<128> integer_type_cpu_128; 
    typedef vli::polynomial< integer_type_cpu_128, vli::max_order_each<10>,
        vli::var<'x'>  >polynomial_type_each_x_128;
    typedef vli::vector polynomial_type_each_x_128;
As the vli::vector derived from a std::vector, it has similar properties. The inner product is simply call by the 
vli::inner_product function which returns a polynomial twice larger (Order and Size of coefficient). We use the class trait.

    typedef typename vli::polynomial_multiply_result_type::type polynomial_res;
The inner product operation is called by the free function vli::inner_product,

    polynomial_type_each_x_128 p1,p2;
    polynomial_res p_res;
                    
    std::generate(p1.begin(), p1.end(), foo); // foo init the number of p1
    std::generate(p2.begin(), p2.end(), foo); // foo init the number of p2
                    
    p_res = vli::inner_product(p1,p2);
By default, the inner product will be multithreaded, free to the developer to fix the desired number of thread with
the usual OpenMP command (export OMP_NUM_THREADS=xx). If the GPU is enabled (with the help of cmake), the hybrid 
inner product will be executed between the CPU/GPU following the setting of the users.
