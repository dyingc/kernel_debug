#!/bin/sh

user=$(whoami)
if [ "${user}" != root ]; then
    echo "Usage: sudo $0"
    exit 1
fi

DEBUGFS=`grep debugfs /proc/mounts | awk '{print $2;}'`
echo 0 > $DEBUGFS/tracing/tracing_on
echo > $DEBUGFS/tracing/set_ftrace_filter
echo > $DEBUGFS/tracing/set_ftrace_notrace
echo > $DEBUGFS/tracing/set_graph_function
echo > $DEBUGFS/tracing/set_graph_notrace
echo  > $DEBUGFS/tracing/set_ftrace_pid
echo > $DEBUGFS/tracing/trace

make clean > /dev/null 2>&1 ; rmmod hello > /dev/null 2>&1
make > /dev/null

echo nop > $DEBUGFS/tracing/current_tracer

echo 1 > $DEBUGFS/tracing/tracing_on
insmod hello.ko > /dev/null
sleep 1m
rmmod hello > /dev/null
echo 0 > $DEBUGFS/tracing/tracing_on

make clean > /dev/null

cat $DEBUGFS/tracing/trace
