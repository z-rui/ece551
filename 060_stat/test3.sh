#!/bin/bash

DUMMY=test3_dummyfile

run_test () {
	diff -s	<(./mystat $DUMMY | sed -n '3p') \
		<(stat $DUMMY | sed -n '3p')
	if [ $? -ne 0 ]; then
		echo "FAILED!"
		exit
	fi
}

run_testvalgrind() {
	echo "Should pass Valgrind:"
	valgrind ./mystat $DUMMY
}

touch $DUMMY
run_test
run_testvalgrind
rm $DUMMY

mkdir $DUMMY
run_test
run_testvalgrind
rmdir $DUMMY

echo "Test for Valgrind"
