rmmod hello 2>/dev/null
insmod hello.ko && cat /sys/kernel/tracing/available_filter_functions | egrep '(device|dyingc)_.*\[hello\]' | awk '{print $1}' > /sys/kernel/tracing/set_ftrace_filter

