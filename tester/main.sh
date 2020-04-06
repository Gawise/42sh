#! /bin/bash

echo $1 $2

test_arr+=($PWD/units/test1/test1.input $PWD/units/test2/test2.input)

./cmd_exec -n /dev/pts/$1 "cd $PWD"
./cmd_exec -n /dev/pts/$2 "cd $PWD"
./cmd_exec -n /dev/pts/$1 "../21sh"
./cmd_exec -n /dev/pts/$2 "bash"
sleep 1s

for i in ${test_arr[@]}
do
	./cmd_exec -n /dev/pts/$1 "$(cat $i)"
	./cmd_exec -n /dev/pts/$2 "$(cat $i)"
	sleep 1s
done

./cmd_exec -n /dev/pts/$2 "exit"
./cmd_exec -n /dev/pts/$1 "exit"
