#!/bin/bash

user=$(whoami)
if [ "${user}" != root ]; then
    echo "Usage: sudo $0"
    exit 1
fi

DEBUGFS=`grep debugfs /proc/mounts | awk '{print $2;}'`
echo 0 > $DEBUGFS/tracing/tracing_on
echo nop > $DEBUGFS/tracing/current_tracer
echo > $DEBUGFS/tracing/trace
echo  > $DEBUGFS/tracing/set_ftrace_pid
echo function > $DEBUGFS/tracing/current_tracer
echo ':mod:e1000' > /sys/kernel/debug/tracing/set_ftrace_filter
echo ':mod:hello' > /sys/kernel/debug/tracing/set_ftrace_filter
make > /dev/null
echo 1 > $DEBUGFS/tracing/tracing_on
#read enter
./userapp
echo 0 > $DEBUGFS/tracing/tracing_on
make clean > /dev/null

cat $DEBUGFS/tracing/trace
