#!/bin/bash

./download.sh
mkdir orig
cp *.txt *.rst orig/
./consolidate.sh
./combine.sh
