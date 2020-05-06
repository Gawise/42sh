#! /bin/bash

test_arr=$(ls $PWD/units)
./cmd_exec -n /dev/pts/$1 "cd $PWD"
./cmd_exec -n /dev/pts/$2 "cd $PWD"
sleep 1s
for i in ${test_arr[@]}
do
	./cmd_exec -n /dev/pts/$1 "../21sh >$PWD/units/$i/21sh_stdout 2>$PWD/units/$i/21sh_stderr"
	./cmd_exec -n /dev/pts/$2 "bash >$PWD/units/$i/bash_stdout 2>$PWD/units/$i/bash_stderr"
	sleep 1s
	./exec $1 $2 $PWD/units/$i
	pid1=$(ps aux | grep ../21sh | grep pts/$1 | grep -v grep | awk '{print $2}')
	pid2=$(ps aux | grep bash | grep pts/$2 | grep -v grep | awk '{print $2}')
	kill $pid1
	kill $pid2
	DIFFERR=$(diff $PWD/units/$i/21sh_stderr $PWD/units/$i/bash_stderr)
	DIFFOUT=$(diff $PWD/units/$i/21sh_stdout $PWD/units/$i/bash_stdout)
	if [[ "$DIFFOUT" != "" || "$DIFFERR" != "" ]];
	then
		echo "$i FAIL"
	else
		echo "$i PASS"
	fi
done

