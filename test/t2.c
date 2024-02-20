#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <math.h>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

#define SIZE 33554432

int main() {
    struct perf_event_attr pe;
    long long count;
    int fd;
	 int64_t *a;
	 int i;

	 a=malloc(SIZE*sizeof(int64_t));
    // Hypothetical configuration - ADJUST for your architecture!
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_HW_CACHE_MISSES; // May need different codes
    pe.disabled = 1; 
    pe.exclude_kernel = 1; // Often exclude kernel from user program profiling

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening perf event\n");
        exit(EXIT_FAILURE);
    }
	 for (i=0;i<SIZE; i++) {
       a[i]=i;
	 }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);  // Reset the count
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0); // Start counting

    // ***** Your Code to Monitor Goes Here *****
	 for (i=SIZE-1;i>0; i=i-1) {
       a[i]=a[i]*2;
	 }
	

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0); // Stop counting
    read(fd, &count, sizeof(long long));

    printf("Cache misses: %lld for array of %d, expected %ld\n", count,SIZE,(SIZE*sizeof(int64_t))/64);
	printf("array MB = %Lf %Lf\n",(SIZE*sizeof(int64_t)) / powl(2,20),powl(2,20));
    close(fd);
    return 0;
}

