#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>

#define N 8388608

int main() {
    int64_t *data; // Our array

    int64_t sum;    

    data = malloc(N * sizeof(int64_t));

    // Set up perf_event_attr structure
    struct perf_event_attr pe;
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HW_CACHE;
    pe.config = PERF_COUNT_HW_CACHE_LL | 
//				( PERF_COUNT_HW_CACHE_OP_PREFETCH << 8) | 
				( PERF_COUNT_HW_CACHE_OP_READ << 8) | 
				(PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
    pe.size = sizeof(struct perf_event_attr);
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    // Create the event
    int fd = syscall(__NR_perf_event_open, &pe, 0, -1, -1, 0);
    if (fd == -1) {
        perror("perf_event_open");
        exit(EXIT_FAILURE);
    }

    // Enable the event

    // Access the array (simulate cache misses)
    for (int i = 0; i < N; i=i+1) {
        data[i] = i;
    }
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
	 sum=0;
//	 int j;
//	for (int i = N; i >  0; i--) {
	for (int i = 0; i < N; i=i+8) {
		sum=sum+data[i];
	}
    printf("sum=%ld\n",sum);

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);

    // Read the event count
    long long count;
    int ret;
    ret=read(fd, &count, sizeof(long long));
    printf("L3 cache misses: %lld ret=%d\n", count,ret);
    printf("N=%d MB= %Lf\n", N,(N*sizeof(int64_t))/powl(2,20));
    printf("expected %ld\n",(N*sizeof(int64_t))/128);
    printf("int=%ld\n",sizeof(int64_t));

    close(fd);
    return 0;
}
