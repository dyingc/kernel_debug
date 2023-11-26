#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

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
	setup_trace_marker();
	write_trace_marker("Before printf");
	printf("Hello Printf\n");
	write_trace_marker("After printf");
	close(marker_fd);
	return 0;
}

