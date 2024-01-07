#!/bin/bash

make && ssh dev "rm -f ~/udemy/kernel/UdemyCourseKernelDebug/samples/linux/samples/kprobes/*"; scp -r * dev:~/udemy/kernel/UdemyCourseKernelDebug/samples/linux/samples/kprobes/
