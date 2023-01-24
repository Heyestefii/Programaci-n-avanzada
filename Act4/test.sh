#!/bin/bash

cal=10

gcc $1
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))

#---
./a.out noexiste1 file2.txt file3.txt
if [[ $? -ne 253 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out file1.txt noexiste2 file3.txt
if [[ $? -ne 252 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out file1.txt file2.txt noexiste3 
if [[ $? -ne 251 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))
#---
#---
./a.out empty1.txt file2.txt file3.txt
if [[ $? -ne 250 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out file1.txt empty2.txt file3.txt
if [[ $? -ne 249 ]] ; then 
	echo "grade: $cal"
	exit 1
fi

./a.out file1.txt file2.txt empty3.txt 
if [[ $? -ne 248 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
cal=$((cal+10))
#---
./a.out file1.txt file2.txt file3.txt &
pid=$!

sleep 2
echo "signal 1"
kill -s SIGUSR1 $pid
ps aux | grep $pid >/dev/null && cal=$((cal+15)) || echo "Not running"

echo "signal 2"
kill -s SIGUSR2 $pid
ps aux | grep $pid >/dev/null && cal=$((cal+15)) || echo "Not running"

echo "signal 3"
kill -s SIGPWR $pid
ps aux | grep $pid >/dev/null && cal=$((cal+15)) || echo "Not running"

echo "end"
kill -s SIGINT $pid
sleep 5
echo "proc $pid"
ps -f
ps aux | grep $pid >/dev/null && cal=$((cal+15)) || echo "Not running"

echo "final grade: $cal"

