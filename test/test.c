#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/param.h>
#include <unistd.h>

int perCacheLine;

void
swap (int64_t * a, int64_t x, int64_t y)
{
   int64_t t;
   t = a[x];
//	if (x==0) { printf ("x is 0\n"); }
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

void followAr(int64_t * a,int64_t n)
{
	int64_t min,max,p,cnt;

	max=0;
	min=65536;
	p=a[n];
   cnt=0;
//	printf ("f @ %ld ", n);
	while (p > 0)
   {
         p = a[p];
			cnt++;
			if (p>max) {
				max=p;
			}
			if (p<min) {
				min=p;
			}
   }
	printf ("min=%ld max=%02ld cnt=%ld\n",min,max,cnt);
}

void
printAr (int64_t * a, int64_t N,int64_t hops)
{
	int64_t i;
	int64_t oldi;
	int64_t *b;

	oldi=0;
   for (i = 0; i < N; i++)
   {
		if (i%hops==0) {
			printf("i=%03ld  ",i);
		}
      printf("%3ld=%03ld ",i%hops,a[i]);
		if (i%hops==(hops-1)) {
			b=&a[i-(hops-1)];
			followAr(b,0);
			oldi=i;
		}
   }
   printf("\n\n");
}

int
main (int argc, char *argv[])
{
	int64_t size, len = 0;
	int64_t c,x,y,max,hops,base;
	int64_t i;
	int64_t *a, *b;
	int64_t p = 0;
   int l;

	srand48(getpid());

   perCacheLine=1;

	size=64;  // int64s
   hops=8;    // keep hops within N INT64s
	len = (size * sizeof (uint64_t));
	a = (int64_t *) malloc (len);
	base=0;
	while (base<size)
   {
		max=MIN(hops,size-base);
		b=&a[base];
		for (i = 0; i < max; i = i + perCacheLine)
   	{
     		b[i] =  i + perCacheLine;   /* assign each int the index of the next int */
   	}
		printf ("base=%ld i=%ld\n",base,i);	
      a[base+i-perCacheLine]=0;
		base=base+hops;
   }
   printAr(a,size,hops);
	printf("\n");
	printf("\n");
   base=0;
	while (base<size)
	{
		max=MIN(hops,size-base);
		b=&a[base];
		printf ("**************** base=%ld max=%ld a[0]=%ld \n",base,max,a[0]);
		printf ("**************** a=%p b=%p diff=%ld\n",a,b,b-a);
		for (i = 0; i < max; i = i + perCacheLine)
   	{
			c = choose (i, max);
			x = b[i];
			y = b[c];
	 	 	printAr(a,size,hops);
			printf ("base=%ld aswapping %ld with %ld i=%ld max=%ld c=%ld\n",base,i+base,c+base,i,max,c);
			swap (b, i, c);
  		 	printAr(a,size,hops);
			printf ("base=%ld bswapping %ld with %ld i=%ld max=%ld c=%ld\n",base,x,y,i,max,c);
			swap (b, x, y);
 		}
		base=base+hops;
   }
   printf ("exit while\n");
	printf ("base=%ld max=%ld a[0]=%ld\n",base,max,a[0]);
   printAr(a,size,hops);
	followAr(a,0);

}
