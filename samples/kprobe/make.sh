#!/bin/bash

make && make bind_test && ssh dev "rm -f ~/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/*"; scp -r * dev:~/udemy/kernel/UdemyCourseKernelDebug/samples/kprobe/
