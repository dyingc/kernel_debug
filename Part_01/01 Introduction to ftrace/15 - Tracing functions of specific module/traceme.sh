#!/bin/bash

DEBUGFS=`grep debugfs /proc/mounts | awk '{print $2;}'`
echo 0 > $DEBUGFS/tracing/tracing_on
echo nop > $DEBUGFS/tracing/current_tracer
echo > $DEBUGFS/tracing/trace
echo  > $DEBUGFS/tracing/set_ftrace_pid
echo function > $DEBUGFS/tracing/current_tracer
echo ':mod:e1000' > /sys/kernel/debug/tracing/set_ftrace_filter
echo 1 > $DEBUGFS/tracing/tracing_on
read enter
echo 0 > $DEBUGFS/tracing/tracing_on

