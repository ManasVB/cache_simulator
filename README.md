# Multi-level Cache and Memory Hierarchy Design Simulator

- Simulated a multi-level cache module with various cache sizes and associativity, implementing LRU replacement and WBWA write policy. Modeled the cache for different memory hierarchy levels, from CPU to main memory, and integrated efficient Stream Buffer Prefetching.
- Compared the performance, area, and energy of different memory hierarchy configurations using the simulator, with a subset of the SPEC 2006 benchmark suite and SPEC 2017 benchmark suite.

1. Build:
	- make all

2. Run:

   To run without throttling output:
   ./sim 32 8192 4 262144 8 3 10 ../example_trace.txt

   To run with throttling (via "less"):
   ./sim 32 8192 4 262144 8 3 10 ../example_trace.txt | less

```
	> ===== Simulator configuration =====
	> BLOCKSIZE:  32
	> L1_SIZE:    8192
	> L1_ASSOC:   4
	> L2_SIZE:    262144
	> L2_ASSOC:   8
	> PREF_N:     3
	> PREF_M:     10
	> trace_file: ../example_trace.txt
	>
```