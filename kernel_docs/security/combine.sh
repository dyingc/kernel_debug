#!/bin/bash

rm -f combined.txt

ls *.txt | while read item; do echo -e "\n\n\n########################################################\n#### ${item}\n########################################################\n" >> combined.txt; cat "${item}" >> combined.txt; done
