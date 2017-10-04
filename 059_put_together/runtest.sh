#!/bin/sh

checkoutput()
{
	diff "list$1.txt.ans" "list$1.txt.counts"
}

./count_values kvs1.txt list1a.txt list1b.txt
for i in a b; do checkoutput 1$i; done
./count_values kvs2.txt list2a.txt list2b.txt list2c.txt
for i in a b c; do checkoutput 2$i; done
