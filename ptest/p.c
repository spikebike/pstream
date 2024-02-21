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

int pageSize = 4096;
int cacheLineSize = 128;		  /* bytes per cacheline */
int perCacheLine;
int cacheLinesPerPage;

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

int64_t
initAr(int64_t *a,int64_t size)
{
	int64_t base,i;
	int64_t *b;
	int64_t cnt;

	base=0;
	cnt=0;
	printf ("cacheLinesPerPage=%d\n",cacheLinesPerPage);
   while (base<size)
	{
		b=&a[base]; 
		for (i = 0; i < pageSize/sizeof(int64_t); i = i + perCacheLine)
		{
			b[i] = i + perCacheLine;	/* assign each int the index of the next int */
			cnt++;
		}
		b[i-cacheLineSize/sizeof(int64_t)]=0;
		base=base+pageSize/sizeof(int64_t);
	}
	return(cnt);
}	

int
shuffleAr(int64_t *a, int64_t size)
{
	int64_t base,c,x,y;
	int64_t *b;

	srand48 ((long int) getpid ());
	base=0;
   while (base<size)
   {
		b=&a[base];
		for (int64_t i = 0; i < pageSize/sizeof(int64_t); i = i + perCacheLine)
		{
			c = choose (i, pageSize/sizeof(int64_t) - perCacheLine);
			x = b[i];
			y = b[c];
			swap (b,i,c);
			swap (b,x,y);
		}
		base=base+pageSize/sizeof(int64_t);
   }
	return(0);	
}

int64_t
followAr (int64_t * a, int64_t size, int repeat)
{
	int64_t p;
	int64_t *b;
	int64_t base;
	int64_t cnt;

	cnt=0;
	for (int64_t i = 0; i < repeat; i++)
	{
		base=0;
		while (base<size) {
			b=&a[base];  // start pointer chasing at begin of page
			p=b[0];
			cnt++;
//  uncomment to debug addresses
//			printf("%p\n",(void *)&p[b]);
			while (p)
			{
				p = b[p];
//				printf("%p\n",(void *)&p[b]);
				cnt++;
			}
			base=base+pageSize/sizeof(int64_t);
		}
	}
	return (cnt);
}

int
verifyAr(int64_t * a, int64_t size)
{
	printf ("perCacheline=%d\n",perCacheLine);
	for (int64_t i=0;i<4096; i=i+perCacheLine) { 
      if (i%(pageSize/sizeof(int64_t)) ==0) { 
         printf("\nbase=%04ld ",i/perCacheLine);
      }
      printf("%ld ",a[i]); 
   }
	printf("\n");
	return(0);
}

int
main (int argc, char *argv[])
{
	int64_t *a;
	int64_t size,ret;
	int64_t maxmem=1073741824; // 1GB 
	double start,end;

	cacheLinesPerPage = pageSize/cacheLineSize;
	perCacheLine = cacheLineSize/sizeof(int64_t);

	size = maxmem / sizeof (int64_t);  // number of int64s.

	if (posix_memalign((void **)&a, getpagesize(), sizeof(int64_t) * maxmem) != 0) {
		printf ("Memory allocation failed\n");
		exit(-1);
	} else { 
		printf ("Allocated %ld bytes or %ld INT64s succeeded.\n",maxmem,size);
	}
	
	ret=initAr(a,size);
	printf("Initialized %ld cachelines\n",ret);
	ret=verifyAr(a,size);
	ret=shuffleAr(a,size);
	ret=verifyAr(a,size);
	if (!ret) { printf ("Shuffle succeded\n"); } else { printf ("shuffle failed\n"); }
	start = second ();
	ret=followAr (a, size, 1);
	end = second();
	printf("visited %ld cachelines\n",ret);
	printf ("took %f seconds, %f ns per cacheline\n",end-start,((end-start)/ret)*1000000000 );
}

