#!/bin/bash

DEBUGFS=`grep debugfs /proc/mounts | awk '{print $2;}'`
echo nop > $DEBUGFS/tracing/current_tracer
echo > $DEBUGFS/tracing/trace
echo $1
echo $$ > $DEBUGFS/tracing/set_ftrace_pid
echo function > $DEBUGFS/tracing/current_tracer
echo 1 > $DEBUGFS/tracing/tracing_on
exec $*
echo 0 > $DEBUGFS/tracing/tracing_on

