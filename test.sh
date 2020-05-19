#! /bin/bash

SHELL_DIR=$(dirname $(readlink -f "$0"))
TESTER_DIR=$SHELL_DIR/tester
TMP_DIR=$TESTER_DIR/tmp
LOG_DIR=$TESTER_DIR/log
TEST_DIRS=$(ls -r $TESTER_DIR/units)
FAIL_TESTS=0
SUCCESS_TESTS=0
TEST_RET=


output_print () {
	if [ -z "$OUTPUT_FILE" ]
	then
		echo -e "$1"
	else
		echo -e "$1" >> $OUTPUT_FILE
	fi
}

print_fail () {
	output_print "--------------------------------------------"
	output_print "\e[1;31mFAIL\e[0m $1"
	if [ -z "$2" ]
	then
		output_print "STDOUT \e[1;32mSUCCESS\e[0m"
	else
		output_print "STDOUT \e[1;31mFAIL\e[0m\n$2"
	fi
	if [ -z "$3" ]
	then
		output_print "STDERR \e[1;32mSUCCESS\e[0m"
	else
		output_print "STDERR \e[1;31mFAIL\e[0m\n$3"
	fi
	if [ "$4" = "$5" ]
	then
		output_print "EXIT_CODE \e[1;32mSUCCESS\e[0m"
	else
		output_print "EXIT_CODE \e[1;31mFAIL\e[0m\nbash=$4\n42sh=$5"
	fi
	output_print "$(cat $LOG_DIR/redir.diff)"
	output_print "--------------------------------------------"
}

move_shell_redir () {
	if [ -f $1/redir ]
	then
		local file_arr
		IFS='\n\r' command eval "file_arr=($(cat $1/redir))"
		for i in ${file_arr[@]}
		do
			mv $TMP_DIR/$i $LOG_DIR/$i.shell
		done
	fi
	echo "${file_arr[@]}"
}

move_bash_redir () {
	if [ -f $1/redir ]
	then
		local file_arr
		IFS='\n\r' command eval "file_arr=($(cat $1/redir))"
		for i in ${file_arr[@]}
		do
			mv $TMP_DIR/$i $LOG_DIR/$i
		done
	fi
	echo "${file_arr[@]}"
}

redir_diff () {
	local diff=""
	local output=""
	REDIR_RET=0
	for i in "$@"
	do
		output="$output"
		if [ -f "$LOG_DIR/$i" ]
		then
			if [ -f "$LOG_DIR/$i.shell" ]
			then
				diff=$(diff $LOG_DIR/$i $LOG_DIR/$i.shell)
				if [ -n "$diff" ]
				then
					output="$output""\e[1;31m$i\e[0m\n"
					output="$output$diff\n"
					REDIR_RET=1
				else
					output="$output""\e[1;32m$i\e[0m\n"
				fi
			else
				output="$output""\e[1;31m$i\e[0m\nNot outputed by $SHELL_FILE\n"
				REDIR_RET=1
			fi
		else
			if [ -f "$TMP_DIR/$i.shell" ]
			then
				output="$output""\e[1;31m$i\e[0m\nNot outputed by Bash\n"
				REDIR_RET=1
			else
				output="$output""\e[1;32m$i\e[0m Both absent\n"
			fi
		fi
	done
	echo "$output"
}



make_test () {
	REDIR_FILE=""
	cd $TMP_DIR

	$SHELL_DIR/$SHELL_FILE $1/input >$LOG_DIR/stdout.sh 2>$LOG_DIR/stderr.sh
	local sh_exit=$?
	local redir_files=($(move_shell_redir $1))
	/bin/bash --posix $1/input >$LOG_DIR/stdout.bash 2>$LOG_DIR/stderr.bash
	local bash_exit=$?
	move_bash_redir $1
	redir_diff ${redir_files[@]} >$LOG_DIR/redir.diff
	local stdout_diff=$(diff $LOG_DIR/stdout.sh $LOG_DIR/stdout.bash)
	local stderr_diff=$(diff $LOG_DIR/stderr.sh $LOG_DIR/stderr.bash)
	if [ -n "$stdout_diff" -o -n "$stderr_diff" -o "$bash_exit" != "$sh_exit" -o "$REDIR_RET" != "0" ]
	then
		print_fail $(basename $1) "$stdout_diff" "$stderr_diff" "$bash_exit" "$sh_exit"
		TEST_RET=1
	else
		output_print "\e[1;32mSUCCESS\e[0m $(basename $1)"
		TEST_RET=0
	fi
	rm -f $LOG_DIR/*
}

run_tests () {
	for i in "$@"
	do
		local test="$TESTER_DIR/units/$i"
		make_test $test
		if [ "$TEST_RET" = "1" ]
		then
			FAIL_TESTS=$((FAIL_TESTS + 1))
		else
			SUCCESS_TESTS=$((SUCCESS_TESTS + 1))
		fi
		#rm -f $TMP_DIR/*
	done
	echo "$FAIL_TESTS out of $(($FAIL_TESTS + $SUCCESS_TESTS)) failed"
}

print_usage () {
	echo 'Usage: test.sh [-h] [-o output_file] shell_file' 1>&2
}

list_vars () {
	while getopts ":o:h" opt;
	do
		case ${opt} in
			o )
				OUTPUT_FILE=$OPTARG
				;;
			h )
				print_usage
				;;
			\? )
				echo "Invalid option: -$OPTARG" 1>&2
				print_usage
				;;
			: )
				echo "Invalid option: -$OPTARG requires an argument" 1>&2
				print_usage
				;;
		esac
	done

	shift $((OPTIND -1))
	if [ -f "$1" ];
	then
		SHELL_FILE=$1
	else
		echo "Missing shell file" 1>&2
		print_usage
		exit 1
	fi
}

list_vars $@
run_tests $TEST_DIRS
