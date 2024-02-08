#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/param.h>

int perCacheLine;

void
swap (int64_t * a, int64_t x, int64_t y)
{
   int64_t t;
   t = a[x];
	if (x==0) { printf ("x is 0\n"); }
   a[x] = a[y];
   a[y] = t;
}

uint64_t
choose (uint64_t l, uint64_t h)
{
   uint64_t range, smallr, ret;

   range = h - l;
   assert (l <= h);
   smallr = range / perCacheLine;   /* the number of cachelines in
                                       the range */
   /* pick a cache line within the range */
   ret = (l + (uint64_t) (drand48 () * smallr) * perCacheLine);
/*  printf ("l=%lld h=%lld ret=%lld\n",l,h,ret);  */
   assert (ret <= h); 
   if (l < h)
      return ret;
   return h;
}

int
main (int argc, char *argv[])
{
	int64_t size, len = 0;
	int64_t c,x,y,cnt,max,hops,base;
	int i;
	int64_t *a;
	int64_t p = 0;

   perCacheLine=2;

	size=256;  // int64s
   hops=16;    // keep hops within N INT64s
	len = (size * sizeof (uint64_t));
	a = (int64_t *) malloc (len);

	for (i = 0; i < size-1; i = i + perCacheLine)
   {
      a[i] = i + perCacheLine;   /* assign each int the index of the next int */
   }

	for (i = 0; i < size-perCacheLine+1; i++)
   {
		printf("%03ld ",a[i]);
	}
	printf("\n");
	printf("\n");
	while (base<size)
	{
		max=MIN(hops,(size)-base);
		printf ("base=%ld max=%ld a[0]=%ld\n",base,max,a[0]);
		for (i = 0; i < max; i = i + perCacheLine)
   	{
			c = choose (i, max);
			x = a[i+base];
			if (x==0) { 
			 	printf("c=%ld x=%ld i=%d base=%ld\n",c,x,i,base);
			}
			y = a[c+base];
			swap (a, i+base, c+base);
			swap (a, x, y);
 		}
		base=base+hops;
   }
   printf ("exit while\n");
	printf ("base=%ld max=%ld a[0]=%ld\n",base,max,a[0]);
	for (i = 0; i < size-perCacheLine+1; i++)
   {
		printf("%03ld ",a[i]);
	}
	printf("\n");

	max=0;
	p=a[0];
	while (p > 0)
   {
         p = a[p];
			cnt++;
			if (p>max) {
				max=p;
			}
   }
	printf ("max=%ld cnt=%ld\n",max,cnt);

}
