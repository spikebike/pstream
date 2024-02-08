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

void followAr(int64_t * a)
{
	int64_t max,p,cnt;

	max=0;
	p=a[0];
   cnt=0;
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

void
printAr (int64_t * a, int64_t N,int64_t hops)
{
	int64_t i;
   for (i = 0; i < N; i++)
   {
		if (i%hops==0) {
			printf("\ni=%03ld  ",i);
		}
      printf("%3ld=%03ld ",i,a[i]);
   }
	followAr(a);
   printf("\n\n");
}

int
main (int argc, char *argv[])
{
	int64_t size, len = 0;
	int64_t c,x,y,max,hops,base;
	int64_t i;
	int64_t *a;
	int64_t p = 0;
   int l;

	srand48(getpid());

   perCacheLine=1;

	size=64;  // int64s
   hops=8;    // keep hops within N INT64s
	len = (size * sizeof (uint64_t));
	a = (int64_t *) malloc (len);
   for (p=0; p < size; p=p+hops)
   {
		for (i = 0; i < size-perCacheLine; i = i + perCacheLine)
   	{
     		a[i] = i + perCacheLine;   /* assign each int the index of the next int */
   	}
      a[size-perCacheLine]=0;
   }
   printAr(a,size,hops);
	printf("\n");
	printf("\n");
   base=0;
	while (base<size)
	{
		max=MIN(hops,(size-1)-base);
//		printf ("**************** base=%ld max=%ld a[0]=%ld bm\n",base,max,a[0],base+max);
		for (i = 0; i < max; i = i + perCacheLine)
   	{
			c = choose (i, max);
         if (c == i) {
				printf ("no swap base=%ld i=%ld c=%ld max=%ld\n",base,i,c,max);
			} else {
				x = a[i+base];
				y = a[c+base];
				int64_t  bm=base+max-1;
				while ((x>bm) || (y>bm) || (a[i]>bm) || (a[c] > bm)) {
					printf ("w x=%ld y=%ld a[%ld]=%ld a[%ld]=%ld i=%ld max=%ld bm=%ld ",x,y,i,a[i],c,a[c],i,max,bm);
               for (l=i; l<max;l++) { printf ("a[%d]=%ld ",l,a[l]); }
               printf ("\n");
					c = choose (i, max);
					x = a[i+base];
					y = a[c+base];
				}
				printf ("no w x=%ld y=%ld a[%ld]=%ld a[%ld]=%ld i=%ld max=%ld ",x,y,i,a[i],c,a[c],i,max);
			
				if ((x<base) || (y<base)){ 
				 	printf("base too low base=%ld c=%ld x=%ld y=%ld i=%ld max=%ld\n",base,c,x,y,i,max);
				}
				if ((x>(base+hops)) || (y>(base+hops))){ 
				 	printf("base too high base=%ld c=%ld x=%ld y=%ld i=%ld max=%ld\n",base,c,x,y,i,max);
				}
  		 		printAr(a,size,hops);
				printf ("base=%ld aswapping %ld with %ld i=%ld max=%ld c=%ld bm=%ld\n",base,i+base,c+base,i,max,c,bm);
				swap (a, i+base, c+base);
  		 		printAr(a,size,hops);
				printf ("base=%ld bswapping %ld with %ld i=%ld max=%ld c=%ld bm=%ld\n",base,x,y,i,max,c,bm);
				swap (a, x, y);
			}
 		}
		base=base+hops;
   }
   printf ("exit while\n");
	printf ("base=%ld max=%ld a[0]=%ld\n",base,max,a[0]);
   printAr(a,size,hops);
	followAr(a);

/*	max=0;
	p=a[0];
	while (p > 0)
   {
         p = a[p];
			cnt++;
			if (p>max) {
				max=p;
			}
   }
	printf ("max=%ld cnt=%ld\n",max,cnt); */
}
