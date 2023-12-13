#!/bin/bash

ls *.rst | while read rst
do
  txt="${rst%.rst}.txt"
  if [ -e "${txt}" ]; then
    echo "${txt} exists for ${rst}, removing ${txt}"
    rm -f "${txt}"
  fi
done

# Change .rst to .txt
ls *.rst | while read item; do dest="${item%.rst}"; dest="${dest}.txt"; mv "${item}" "${dest}"; done
