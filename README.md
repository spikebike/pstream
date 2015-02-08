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

To run a latency test: 
	./pstream -l -f output_file 

To graph bandwidth data:
	./view results/i5-4570-bw-UA

To graph latency data:
    ./lview results/i5-4570-lat-p1 

Example run script in run.sh.

Test:
![Image of Yaktocat](https://octodex.github.com/images/yaktocat.png)

test2:
![image of graph](https://raw.githubusercontent.com/spikebike/pstream/master/png/i5-4570-bw-Ua.png)


