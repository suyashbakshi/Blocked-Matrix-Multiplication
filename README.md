# Blocked-Matrix-Multiplication

A simple implementation of Blocked Matrix-Matrix multiplication for a 2-level memory hierarchy (L1 and L0). Extension to more levels can be implemented with minimal effort.

The code implements naive GEMM operation C = C + A * B for symmetric matrices.
However, code can be easily modified to use different sizes for each the dimension. OpenMP calls for parallelization can be also added easily.

Code Flow:
The program has 2 nested loops apart from the blocked MM operation.
Outer loop loops through different block sizes.
Inner loop loops through cases for different number of threads (requires addition of OpenMP calls).

Brief explanation about different loop orders is provided in the code. For a beginner, it will be an interesting exercise to play around with different loop orders and tile/block sizes to understand their implications on the performance of code.

Note that, the loop orders define WHEN the data is moved between the levels of memory hierarchy.
On the other hand, block sizes define HOW MUCH data should be moved between the levels of memory hierarchy.

There are several great resources on the internet that go into detail about why blocking is beneficial for achieving better performance. To summarize them in terms of a simple metric, it is maximizing the "Data Reuse" (many times also referred to as Arithmetic Intesity, though there is a subtle difference between both, but ignore that for now). For example:

If you have 3 NxN matrices A, B and C. And if you perform the GEMM operation naively. That is, using N^2 inner products, each involving N elements of A and N elements of B to compute 1 element of C. The memory references are '2N + 1' (~2N), and arithmetic operations are 2N (N multiplications and N additions). Thus data reuse of 2N/2N = 1.

However, if you load bxb blocks of each of the 3 matrices (where b < N), you have 3b^2 memory references and 2b^3 arithmetic operations. Thus data reuse of 2b^3/3b^2 = 2b/3, which is obviously better than 1 in the naive case.

Maximizing data reuse in computer architecture is important as it reduces the expensive data movements (both in terms of latency and energy consumption) from a lower level memory such as DRAM.

Here's a link to my work with SGEMM on Intel/Movidius Myriad-2 SoC showing importance of such optimizations in achieving high performance and energy efficiency: https://ieeexplore.ieee.org/document/9235073
