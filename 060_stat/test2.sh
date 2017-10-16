#!/bin/bash

DUMMY=test2_dummyfile

run_test () {
	echo "Test for permission code $1"
	chmod $1 $DUMMY
	diff -s	<(./mystat $DUMMY | sed -n '3p') \
		<(stat $DUMMY | sed -n '3s/^\([^)]*\).*$/\1/p')
	if [ $? -ne 0 ]; then
		echo "FAILED!"
		exit
	fi
}

SEQ=`seq 0 7`

run_testset() {
	for u in $SEQ; do
		for g in $SEQ; do
			for o in $SEQ; do
				run_test $u$g$o
				#read -n1
			done
		done
	done
}

run_testvalgrind() {
	echo "Should pass Valgrind:"
	valgrind ./mystat $DUMMY
	read -n1
}

touch $DUMMY
run_testset
run_testvalgrind
rm $DUMMY

mkdir $DUMMY
run_testset
run_testvalgrind
rmdir $DUMMY

echo "Test for Valgrind"
