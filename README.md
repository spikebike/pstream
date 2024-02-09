pstream - A bandwidth, latency, and parallelism benchmark to test cache and memory.

Pstream is designed to explore the bandwidth and latency of the memory hierarchy with different size arrays and different number of threads. 

Includes gnuplot scripts for visualizing results.

Todo:
* Implement shuffel to visit each cacheline of N (settable with -p) pages exactly once before proceeding to the next page.
* Ensure HUGETLB and regular memory allocation are page aligned.
* Fix defaults to enable linux flags for NUMA and USEAFFINITY
* Write example script to show bandwidth/latency impacted by TLB friendliness.
  



 
