#!/bin/bash

for i in /usr/local/ece551/bin_search/bsearch*.o
do
    test=`basename $i | sed 's/bsearch//' | sed 's/.o//'`
    if [ "$test"  == "" ]
    then
	echo "**Testing correct implementation **"
    else
	echo "**Testing broken implementation ${test} **"
    fi
    echo "-------------------------------------"
    echo ""
    g++ -o test-bsearch test-bsearch.cpp $i -lm
    if [ "$?" != "0" ]
    then
	echo "Could not compile test-bsearch.cpp with $i" > /dev/stderr
	exit 1
    fi
    ./test-bsearch
    if [ "$?" != 0 ]
    then
	if [ "$test" == "" ]
	then
	    echo "Your test program falsely failed the correct implementation!" > /dev/stderr
	    exit 1
	fi
    else
	if [ "$test" != "" ]
	then
	    echo "Your test program did not identify $i as broken!" > /dev/stderr
	    exit 1
	fi
    fi
    echo ""
done
