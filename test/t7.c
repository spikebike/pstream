#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/perf_event.h>
#include <asm/unistd.h> 
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>

// ... (perf_event_open wrapper - see previous examples) ...

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                    group_fd, flags);
    return ret;
}


#define ARRAY_SIZE 1000000  

// Function to set up and read a performance counter
long long measure_cache_event(struct perf_event_attr *pe) {
    int fd = perf_event_open(pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening perf event\n");
        exit(EXIT_FAILURE);
    }

    long long count;
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

    // *** Array/Code to Monitor Here *** 

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &count, sizeof(long long));
    close(fd);
    return count;
}

int main() {
    // ... Array setup as in previous examples ... 

    struct perf_event_attr pe_l1, pe_l2, pe_l3; // One for each level

    // --- Hypothetical L1 Miss Configuration ---
    memset(&pe_l1, 0, sizeof(struct perf_event_attr));
    pe_l1.type = PERF_TYPE_HW_CACHE;
    pe_l1.size = sizeof(struct perf_event_attr);
    pe_l1.config = // ... Consult architecture manual for L1 miss-like events...

    // --- Similar for L2 & L3,  different 'config' codes ---

    long l1_misses = measure_cache_event(&pe_l1);
    long l2_misses = measure_cache_event(&pe_l2);
    long l3_misses = measure_cache_event(&pe_l3);

    printf("L1 Misses: %lld\n", l1_misses);
    printf("L2 Misses: %lld\n", l2_misses);
    printf("L3 Misses: %lld\n", l3_misses);

    return 0;
}

