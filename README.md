# parallel sort
# ORIGINAL
 my original sort

# PARADIS
parallel sort for in-place msd radix sort.

In-place sort means that the operation for array is only swap so that no new memory allocation for array is required. This idea is more helpful for data saving!

Msd radix sort is more parallel executable than wellknown radix sort(lsdradix sort). This algorithm begins with the most significant digit.

PARADIS is one of the solution that make this msd radix sort more parallel executable. Compared to sequential one,new two phases, permutate and repair, are needed to acommplish high parallel efficiency.

# MAKE
Makefile in PARADIS is helpful for compile, debug. It gives some target, "clean", "debug", "help" etc.
If you want to know what you should, please command **"make help"**!

# REFERENCE
ROLLAND HE. "PARADIS: A PARALLEL IN-PLACE RADIX SORT ALGORITHM", https://stanford.edu/~rezab/classes/cme323/S16/projects_reports/he.pdf

Minsik Cho, Daniel Brand, Rajesh Bordawekar, Ulrich Finkler, Vincent Kulandaisamy, Ruchir Puri. "PARAIDS: An Efficient Parallel Algorithm for In-place Radix Sort",0a August 2015, https://dl.acm.org/doi/abs/10.14778/2824032.2824050