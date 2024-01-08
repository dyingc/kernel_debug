#!/bin/bash

make clean; make && make bind_test && ssh dev "rm -f $PWD/*"; scp -r * dev:$PWD/
