#!/bin/bash

./download.sh
mkdir -p orig 2>/dev/null
cp *.txt *.rst orig/
./consolidate.sh
./combine.sh
