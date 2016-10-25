#!/usr/bin/env bash
PATH=../src:$PATH

run () {
	echo "====================================================="
	echo "Running input: $1"
	shlex <$1
	echo "Run finished"
	echo
}

for i in 01 02 03 04 05 
do
	run in/in$i.txt
done
#run in/in01.txt
#run in/in02.txt
#run in/in03.txt
#run in/in04.txt
