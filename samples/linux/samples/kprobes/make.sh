#!/bin/bash

make && ssh dev "rm -f ${PWD}/*"; scp -r * dev:${PWD}/
