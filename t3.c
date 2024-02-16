#define _GNU_SOURCE
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                    group_fd, flags);
    return ret;
}

int
main(int argc, char **argv)
{
    struct perf_event_attr pe;
    long long count;
    int fd;
    char *chars, c;

    uint64_t n;
    if (argc > 1) {
        n = strtoll(argv[1], NULL, 0);
    } else {
        n = 10000;
    }

    chars = malloc(n * sizeof(char));

    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HW_CACHE;
    pe.size = sizeof(struct perf_event_attr);
// works
      pe.config = PERF_COUNT_HW_CACHE_L1D |
                PERF_COUNT_HW_CACHE_OP_READ << 8 |
                PERF_COUNT_HW_CACHE_RESULT_MISS << 16;
// gemini
//	 pe.config = PERF_COUNT_HW_CACHE_RESULT_ACCESS | 
 //               (PERF_COUNT_HW_CACHE_OP_READ << 8) | 
  //              (PERF_COUNT_HW_CACHE_LL << 16); 
//PERF_MEM_LVL_MISS | PERF_MEM_LVL_L3;
//	pe.config = PERF_MEM_LVL_L3  ;
//	pe.config = pe.config << 5;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    // Don't count hypervisor events.
    pe.exclude_hv = 1;

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe.config);
        exit(EXIT_FAILURE);
    }

    /* Write the memory to ensure misses later. */
    for (size_t i = 0; i < n; i++) {
        chars[i] = 1;
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

    /* Read from memory. */
	 int j;
	 for (j=0;j<32; j++) {
	    for (size_t i = 0; i < n; i++) {
     	   c = chars[i];
   	 }
	}
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &count, sizeof(long long));

    printf("%lld to read %ld expected %ld\n", count,n,n/64);
    printf("array size = %8.0LF MB  1 MB=%8.0LF\n", n/powl(2,20),powl(2,20));

    close(fd);
    free(chars);
}
