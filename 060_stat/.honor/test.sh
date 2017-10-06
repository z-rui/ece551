#!/bin/bash
stat $@ > right.txt
./mystat $@ > yours.txt
diff right.txt yours.txt
rm right.txt yours.txt
