
1. How trustworthy are the numbers?

	I've done audit's using cache and TLD CPU counters.  I've also compared to lmbench's
   lat_mem_rd

   Example 1: lat_mem_rd stride 512 = pstream -b 16
```
   $ lat_mem_rd -P 1 1024 512 # 1GB stride 512 bytes
   512.00000 71.283
   768.00000 70.767
   1024.00000 70.404

   $ ./pstream -t 1 -T 1 -f log -p 16 -l -m 512000 -M 1024 -i 75
   1 Thread(s) size=1.000GB repeat=1.000   diff=0.589 lat = 70.245258 avgLat = 70.245258 
   1 Thread(s) size=768.0MB repeat=1.000   diff=0.439 lat = 69.759733 avgLat = 69.759733
   1 Thread(s) size=576.0MB repeat=1.000   diff=0.343 lat = 72.628271 avgLat = 72.628271
```
	Example 2: lat_mem_rd stride 256 = pstream -b 8
```
	$ ./lat_mem_rd -P 1 1024 256
	512.00000 57.530
	768.00000 57.232
	1024.00000 57.638

	$ ./pstream -t 1 -T 1 -f log -p 16 -l -m 512000 -M 1024 -i 75
	1 Thread(s) size=1.000GB repeat=1.000   diff=0.471 lat = 56.130069 avgLat = 56.130069
	1 Thread(s) size=768.0MB repeat=1.000   diff=0.358 lat = 56.874768 avgLat = 56.874768
	1 Thread(s) size=576.0MB repeat=1.000   diff=0.267 lat = 56.485949 avgLat = 56.485949
```
	
2. Why is benchmarking random access so hard?  

	The two main factors in memory latency are the actual latency between CPU and memory, but another factor if the TLB.  TLB is much like a cache, but for memory pages.  So accessing 1GB purely randomly is slow (105ns or so):
	1 Thread(s) size=1.000GB repeat=1.000   diff=0.973 lat = 115.954506 avgLat = 115.954506
	1 Thread(s) size=768.0MB repeat=1.000   diff=0.671 lat = 106.690112 avgLat = 106.690112
	1 Thread(s) size=576.0MB repeat=1.000   diff=0.478 lat = 101.295095 avgLat = 101.295095

	Pstream supports a sliding window, this doesn't impact that every cacheline is accessed    exactly once, but does control the locality of random accesses.  So -p 8 will keep the shuffle within 8 pages instead of over the entire array.  The resulting latency numbers should be more directly related to memory latency and less sensitive to differences in the TLB.  









