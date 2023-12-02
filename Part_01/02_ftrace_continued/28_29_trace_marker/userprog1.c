#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

static int marker_fd = -1;

static void setup_trace_marker(void)
{
	marker_fd = open("/sys/kernel/tracing/trace_marker", O_WRONLY);
}

static void write_trace_marker(const char *fmt, ...)
{
	char buf[2048];
	va_list ap;
	int n;

	if (marker_fd < 0)
		return;

	va_start(ap, fmt);
	n = vsnprintf(buf, 2048, fmt, ap);
	va_end(ap);
	write(marker_fd, buf, n);

}

int main()
{
	int tracing_on_fd;
	int ret;

	tracing_on_fd = open("/sys/kernel/debug/tracing/tracing_on", O_WRONLY);
	if (tracing_on_fd < 0) {
		perror("error:");
		exit(-1);
	}
	setup_trace_marker();
	ret = write(tracing_on_fd, "1", 1); // Enable tracing
	if (ret < 0) {
		perror("error:");
		exit(-1);
	}
	write_trace_marker("Before printf");
	printf("Hello Printf\n");
	write_trace_marker("After printf");
	ret = write(tracing_on_fd, "0", 1); // Disable tracing
	if (ret < 0) {
		perror("error:");
		exit(-1);
	}
	close(marker_fd);
	close(tracing_on_fd);
	return 0;
}

