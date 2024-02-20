#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <asm/unistd.h>  // May be needed for architecture-specific codes
#include <string.h>
#include <linux/perf_event.h>    /* Definition of PERF_* constants */
#include <linux/hw_breakpoint.h> /* Definition of HW_* constants */
#include <sys/syscall.h>         /* Definition of SYS_* constants */
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                    group_fd, flags);
    return ret;
}


// ... (perf_event_open wrapper - see previous examples) ...

#define ARRAY_SIZE 1000000  // Adjust as needed

int main() {
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1; 
    }

    struct perf_event_attr pe;
    long long cache_misses; 
    int fd;

    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HW_CACHE;
    pe.size = sizeof(struct perf_event_attr);

    // Hypothetical Intel L3 miss configuration
    pe.config = PERF_COUNT_HW_CACHE_RESULT_ACCESS | 
                (PERF_COUNT_HW_CACHE_OP_READ << 8) | 
                (PERF_COUNT_HW_CACHE_LL << 16); 

//pe.config = PERF_COUNT_HW_CACHE_L1D |
 //               PERF_COUNT_HW_CACHE_OP_READ << 8 |
  //              PERF_COUNT_HW_CACHE_RESULT_MISS << 16;
	pe.config = PERF_COUNT_HW_CACHE_LL | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
	pe.config = 0x20c4; 

    pe.disabled = 1;
    pe.exclude_kernel = 1;

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening perf event\n");
        exit(EXIT_FAILURE);
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

    // *** Array Access Logic ***
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;  // Sample operation
    }

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &cache_misses, sizeof(long long));

    printf("L3 Cache Misses: %lld\n", cache_misses);
    close(fd);
    free(array); 
    return 0;
}

