#!/bin/bash
run_test() {
    prog="$1"
    testfile="$2"
    IFS=$'\n'
    for line in `cat $testfile`
    do
        echo "$line"
	IFS=" " correct=`/usr/local/ece551/rot_matrix/rotateMatrix $line 2>&1`
	IFS=" " broken=`$prog $line 2>&1`
	if [ "$broken" != "$correct" ]
	then
	    echo "<$correct"
	    echo "---"
	    echo ">$broken"
	    #return 0
	fi
	echo "pass"
    done
    return 1
}

run_test ./rotateMatrix tests.txt
