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



// ... (perf_event_open wrapper from previous example) ...
int
main(int argc, char **argv)
{
    struct perf_event_attr pe;
    long long count;
    int fd;
	 uint64_t n;
	 char *chars, c;

	 if (argc > 1) {
        n = strtoll(argv[1], NULL, 0);
    } else {
        n = 10000;
    }
	chars = malloc(n * sizeof(char));

    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HW_CACHE; 
    pe.size = sizeof(struct perf_event_attr);

    // Hypothetical L3 miss codes (Intel)
// gem
    pe.config = PERF_COUNT_HW_CACHE_RESULT_ACCESS | 
                (PERF_COUNT_HW_CACHE_OP_READ << 8) | 
                (PERF_COUNT_HW_CACHE_LL << 16); 
// works from https://stackoverflow.com/questions/10082517/simplest-tool-to-measure-c-program-cache-hit-miss-and-cpu-time-in-linux
pe.config = PERF_COUNT_HW_CACHE_L1D |
                PERF_COUNT_HW_CACHE_OP_READ << 8 |
                PERF_COUNT_HW_CACHE_RESULT_MISS << 16;
//try
pe.config = PERF_COUNT_HW_CACHE_L1D |
                PERF_COUNT_HW_CACHE_OP_READ << 8 |
                PERF_COUNT_HW_CACHE_RESULT_MISS << 16;

    pe.disabled = 1;
    pe.exclude_kernel = 1;

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    // ... rest of the logic - error handling, ioctl calls, reading count ... 

	for (size_t i = 0; i < n; i++) {
        chars[i] = 1;
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

    /* Read from memory. */
    int j;
    for (j=0;j<1; j++) {
       for (size_t i = 0; i < n; i++) {
         c = chars[i];
       }
  	 }
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &count, sizeof(long long));

    printf("%lld to read %ld expected %ld\n", count,n,n/64);
    printf("array size = %8.0LF MB  1 MB=%8.0LF\n", n/powl(2,20),powl(2,20));


}


