# Blocked-Matrix-Multiplication

A simple implementation of Blocked Matrix-Matrix multiplication for a 2-level memory hierarchy (L1 and L0). Extension to more levels can be implemented with minimal effort.

The code implements naive GEMM operation C = C + A * B for symmetric matrices (double precision).
However, code can be easily modified to use different sizes for each the dimension. OpenMP calls for parallelization can be also added easily.

Important note: the code uses statically defined matrices. For larger matrices you should consider modifying the code to use malloc() for matrix memory allocation.

Code Flow:
The program has 2 nested loops apart from the blocked MM operation.
Outer loop loops through different block sizes.
Inner loop loops through cases for different number of threads (requires addition of OpenMP calls).

Brief explanation about different loop orders is provided in the code. For a beginner, it will be an interesting exercise to play around with different loop orders and tile/block sizes to understand their implications on the performance of code.

Note that, the loop orders define WHEN the data is moved between the levels of memory hierarchy.
On the other hand, block sizes define HOW MUCH data should be moved between the levels of memory hierarchy.

There are several great resources on the internet that go into detail about why blocking is beneficial for achieving better performance. To summarize them in terms of a simple metric, it is maximizing the "Data Reuse", not to be confused with "Potential Arithmetic Intensity".
GEMM operation has a potential arithmetic intensity of 2N/3 (2N^3 arithmetic operations performed over 3N^2 data elements), meaning that its a compute intensive algorithm. However, practically this is almost impossible to achieve unless all 3 matrices fit completely in the caches. Data reuse is a measure of quality of an implementation of the algorithm (GEMM, in this case) that shows how much of that potential arithmetic intensity it was able to exploit. Using blocked MM, data reuse can be improved significantly over a naive implementation of the GEMM operation.

For example:

If you have 3 NxN matrices A, B and C. And if you perform the GEMM operation naively. That is, using N^2 inner products, each involving N elements of A and N elements of B to compute 1 element of C. The memory references are '2N + 1' (~2N), and arithmetic operations are 2N (N multiplications and N additions). Thus data reuse of 2N/2N = 1.

However, if you load bxb blocks of each of the 3 matrices (where b < N), you have 3b^2 memory references and 2b^3 arithmetic operations. Thus data reuse of 2b^3/3b^2 = 2b/3, which is obviously better than 1 in the naive case.

Maximizing data reuse in computer architecture is important as it reduces the expensive data movements (both in terms of latency and energy consumption) from a lower level memory such as DRAM.

There are several considerations that go into optimizing an algorithm. Here's a link to my work with SGEMM on Intel/Movidius Myriad-2 SoC showing importance of such optimizations in achieving high performance and energy efficiency: https://ieeexplore.ieee.org/document/9235073
