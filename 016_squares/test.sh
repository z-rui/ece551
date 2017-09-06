#!/bin/bash
cat > temp.c <<EOF
#include <stdio.h>
#include <stdlib.h>
EOF
cat squares.c >> temp.c
cat >> temp.c <<EOF
int main(void) {
 squares(6, 3, 4, 7);
 return 0;
}
EOF
gcc -Wall -Werror -pedantic -std=gnu99 temp.c -o squares 2>errors.txt
if [ "$?" = "0" ]
    then
    echo "Your code appears to have legal syntax!"
    echo "Here is what I get when I run squares(6,3,4,7)"
    echo "-----------------"
    ./squares
    echo "-----------------"
    echo "Here is what I would expect the answers to be:"
    echo "----------------"
    cat <<EOF
######    
#    #    
#    #    
#    #    
#  *******
###*##   *
   *     *
   *     *
   *     *
   *     *
   *******
EOF
    echo "---------------"
    else
    echo "Oh no, the syntax of your code is not correct!"
    mesg=`grep error errors.txt | head -1`
    ln=`echo "$mesg" | cut -f2 -d":"`
    let ln=${ln}-2
    echo "I discovered the problem on line $ln "
    echo "(though the problem may be earlier and I just got confused)"
    echo "Here is my best attempt to describe what is wrong:"
    echo "$mesg" | cut -f5 -d":"
    fi
rm -f temp.c errors.txt squares

