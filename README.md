# pstream
A bandwidth, latency, and parallelism benchmark to test cache and memory. 

Designed to expose and quantify parallelism in the memory hierarchy 
by tracking the performance of varying number of threads over varying
size arrays of doubles.

Very loosely based on John D. McCalpin's Stream: 
	http://www.cs.virginia.edu/stream/

The bandwidth test does Triad and Sum with very similar inner loops to McCalpin's stream.

The latency test visits each cache line exactly once.  With -p0 each member of the array is shuffled to random position in the entire array.

With -pN each member of the array is shuffled to within N pages.  This is used to allow seperation of testing memory latency with TLB latency/thrashing.

To compile:
    make

For command line options:
    ./pstream -?

To run a bandwidth test: 
	./pstream -b -f output_file 
	
	Example 
	![Performance stats](https://github.com/spikebike/pstream/blob/master/png/i5-4570-bw-Ua.png)


To run a latency test: 
	./pstream -l -f output_file 

to graph bandwidth data:
	./view results/i5-4570-bw-UA

to graph latency data:
    ./lview results/i5-4570-lat-p1 

Example run script in run.sh.

