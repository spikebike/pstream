#define _GNU_SOURCE
#define _MULTI_THREADED
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <float.h>
#include <string.h>
#include <assert.h>
#include <sched.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef USEHUGE
#include <sys/mman.h>
#endif
#ifdef USENUMA
#include <numa.h>
#endif

int cacheLineSize = 64;		  /* bytes per cacheline */
double increaseArray = 0.925;
int pageSize = 4096;
int perCacheLine;
int cacheLinesPerPage;

double begin,end;

/* crude wall clock time keeping, returns a double of seconds */
double
second ()
{
	struct timeval tp;
	struct timezone tzp;
	gettimeofday (&tp, &tzp);
	return ((double) tp.tv_sec + (double) tp.tv_usec * 1.e-6);
}

void
swap (int64_t * a, int64_t x, int64_t y)
{
	int64_t t;
	t = a[x];
	a[x] = a[y];
	a[y] = t;
}

/* choose between l and h, inclusive of both. */
uint64_t
choose (uint64_t l, uint64_t h)
{
	uint64_t range, smallr, ret;

	range = h - l;
	assert (l <= h);
	smallr = range / perCacheLine;	
	ret = (l + (uint64_t) (drand48 () * smallr) * perCacheLine);
	assert (ret <= h);
	if (l < h)
		return ret;
	return h;
}

int
followAr (int64_t * a, int64_t size, int repeat, int64_t hops)
{
	int64_t p = 0;
	int64_t *b;
	int i;
	int64_t base;
	for (i = 0; i < repeat; i++)
	{
		base=0;
		while (base<size) {
			b=&a[base];  // start pointer chasing at begin of page
			p=b[0];
//			printf("%p\n",(void *)&p[b]);
			while (p)
			{
				p = b[p];
//				printf("%p\n",(void *)&p[b]);
			}
			base=base+hops*cacheLineSize;  // fix 4k / 8 bytes = 512
		}
	}
	return (a[0]);
}

initAr(int64_t *a,int64_t size)
{
   while (base<size)
	{
		max=MIN(hops,size-base);
		b=&a[base]; 
		for (i = 0; i < cacheLinePerPage; i = i + perCacheLine) //fix
		{
			b[i] = i + perCacheLine;	/* assign each int the index of the next int */
		}
		b[i-cacheLineSize]=0;
		base=base+pageSize/sizeof(int64_t);
	}
}	

shuffleAr(int64_t *a, int64_t size)
{
	srand48 ((long int) getpid ());
	base=0;
   while (base<size)
   {
		b=&a[base];
      // leave the first hop alone, it loads the page and cacheline and we don't want to exit
		for (i = 0; i < cacheLinesPerPage; i = i + perCacheLine) //fix
		{
			c = choose (i, cacheLinesPerPage - perCacheLine);
			x = b[i];
			y = b[c];
			swap (b,i,c);
			swap (b,x,y);
		}
		base=base+pageSize/sizeof(int64_t);
   }
	base=0;
}

int
main (int argc, char *argv[])
{
	int64_t *a,*b;
	int64_t *aa = NULL;
	int64_t x, y;
	int64_t i, c, max =0;
	int64_t size, len = 0;
   int64_t base,hops;
	double start,end;

	size = maxmem / sizeof (int64_t);  // number of int64s.

	if (posix_memalign((void **)&a, getpagesize(), sizeof(int64_t) * maxmem) != 0) {
		printf ("Memory allocation failed\n");
		exit(-1);
	}
	initAr(a,size);
	shuffler(a,size);
	start = second ();
	followAr (a, size, scale, hops);
	end = second();
	printf ("took %f seconds\n",end-start);
}

