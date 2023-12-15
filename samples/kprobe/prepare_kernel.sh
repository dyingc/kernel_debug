#!/bin/bash

kernel_header_cnt=`dpkg -l | egrep "linux-header.*$(uname -r)" | wc -l | awk '{print $1}'`
if [ ${kernel_header_cnt} -eq 0 ]; then
  sudo apt-get -y install linux-headers-$(uname -r)
fi
