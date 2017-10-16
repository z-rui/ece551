#!/bin/bash

echo "Should say missing operand:"
./mystat
echo

# having too many arguments is not a problem;
# they will be handled when I get to Step 3

echo "Should say file not exist:"
./mystat xyzzy
echo

echo "Should say permission denied:"
./mystat /root/.
echo

echo "Should match:"
diff -s <(./mystat ~) <(stat ~ | head -n3)
echo

echo "Should pass Valgrind:"
valgrind ./mystat ~
echo
