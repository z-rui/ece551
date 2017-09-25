#!/bin/bash

IFS=$'\n'
s="
1

2
3"

# You won't see the blank line in the output!
for line in $s; do
	echo "^$line$"
done
