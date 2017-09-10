#!/bin/bash

testcases="
surprise!
-1
0
1
2
3
4
5
6
7
32767
32768
32769
65535
65536
65537
"

rm -f input.{1,2,3,4}

for number in $testcases; do
	./isPrime-correct $number > output.ref
	for i in 1 2 3 4; do
		timeout --foreground 35 ./isPrime-broken$i "$number" > output.$i
		[ $? == 124 ] && echo "Timeout !!"
		diff output.ref output.$i
		if [ $? != 0 ]; then
			echo "Found a bug in isPrime-broken$i !!"
			echo "Input was:"
			echo "$number" | tee input.$i
		fi
	done
done

for i in 1 2 3 4; do
	if [ ! -e input.$i ]; then
		echo "Still need to find a bug in isPrime-broken$i"
	fi
done

rm -f output.{1,2,3,4,ref}
