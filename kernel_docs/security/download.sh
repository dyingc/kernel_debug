#!/bin/bash

BASE="https://www.kernel.org/doc/Documentation/security/" # https://www.kernel.org/doc/html/v5.0/security/index.html

curl "${BASE}" --output root.html 2>/dev/null

cat root.html | egrep "^<a href=" | cut -d '"' -f 2 | egrep -v '/$' | awk '{print "'"${BASE}"'"$1}' | while read item
do
  echo -n "Downloading ${item} ... ..."
  wget ${item} 2>/dev/null
  echo -e "\tDone.\n"
done
