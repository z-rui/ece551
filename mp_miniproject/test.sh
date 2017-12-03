echo $SSH_CLIENT
set file_name1 README
wc $file_name1
set file_name2 big
set file_name2 $file_name2\file
echo $file_name2
cat $file_name2 | wc
set expression 1995 + 10 - 3 * 10 / 6
echo $expression | bc
