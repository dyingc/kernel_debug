#!/bin/bash

BASE="https://www.kernel.org/doc/Documentation/trace/"

curl ${BASE} --output root.html 2>/dev/null

cat root.html | egrep "^<a href=" | cut -d '"' -f 2 | egrep -v '/$' | awk '{print "https://www.kernel.org/doc/Documentation/trace/"$1}' | while read item
do
  echo -n "Downloading ${item} ... ..."
  wget ${item} 2>/dev/null
  echo -e "\tDone.\n"
done
