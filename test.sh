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
	output_print "\e[1;41m$1\e[0m"
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
	if [ "$REDIR_RET" != "0" ]
	then
		output_print "REDIR \e[1;31mFAIL\e[0m"
		output_print "$(cat $LOG_DIR/redir.diff)"
	else
		output_print "REDIR \e[1;32mSUCCESS\e[0m"
		output_print "$(cat $LOG_DIR/redir.diff)"
	fi
	if [ "$6" == "156" ]
	then
		output_print "LEAKS \e[1;31mFAIL\e[0m\nDetails in $LOG_DIR/.$(basename $1)"
	else
		output_print "LEAKS \e[1;32mSUCCESS\e[0m"
	fi
	output_print "--------------------------------------------"
}

move_shell_redir () {
	if [ -f $1/redir ]
	then
		local file_arr
		IFS='\n\r' command eval "file_arr=($(cat $1/redir))"
		for i in ${file_arr[@]}
		do
			if [ -f $TMP_DIR/$i ]
			then
				mv $TMP_DIR/$i $LOG_DIR/$i.shell
			fi
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
			if [ -f $TMP_DIR/$i ]
			then
				mv $TMP_DIR/$i $LOG_DIR/$i
			fi
		done
	fi
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

env_setup () {
	if [ -f "$1" ]
	then
		/bin/bash $1
	fi
}

get_std_diff () {
	if [ -f "$1/$2" ]
	then
		diff $LOG_DIR/$2.sh $1/$2
	else
		diff $LOG_DIR/$2.sh $LOG_DIR/$2.bash
	fi
}

make_test () {
	REDIR_FILE=""
	cd $TMP_DIR
	env_setup $1/setup
	$SHELL_DIR/$SHELL_FILE $1/input >$LOG_DIR/stdout.sh 2>$LOG_DIR/stderr.sh
	local sh_exit=$?
	local redir_files=($(move_shell_redir $1))
	rm -f $TMP_DIR/*
	cd $TMP_DIR
	env_setup $1/setup
	/bin/bash --posix $1/input >$LOG_DIR/stdout.bash 2>$LOG_DIR/stderr.bash
	local bash_exit=$?
	move_bash_redir $1
	rm -f $TMP_DIR/*
	redir_diff ${redir_files[@]} >$LOG_DIR/redir.diff
	local stdout_diff=$(get_std_diff $1 "stdout")
	local stderr_diff=$(get_std_diff $1 "stderr")
	valgrind --leak-check=yes --leak-check=full --track-origins=yes --error-exitcode=256 --log-file="$LOG_DIR/.$(basename $1)" $SHELL_DIR/$SHELL_FILE $1/input 1>&- 2>&-
	local leaks_check=$?
	if [ -n "$stdout_diff" -o -n "$stderr_diff" -o "$bash_exit" != "$sh_exit" -o "$REDIR_RET" != "0" -o "$leaks_check" == "256" ]
	then
		print_fail $(basename $1) "$stdout_diff" "$stderr_diff" "$bash_exit" "$sh_exit" "$leaks_check"
		TEST_RET=1
	else
		output_print "\e[1;42m$(basename $1)\e[0m"
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
		rm -f $TMP_DIR/*
	done
	echo "$FAIL_TESTS out of $(($FAIL_TESTS + $SUCCESS_TESTS)) failed"
}

print_usage () {
	echo 'Usage: test.sh [-h] [-o output_file] shell_file' 1>&2
}

list_vars () {
	local h_opt=0
	local usage=0
	local exit_code=0
	while getopts ":o:h" opt;
	do
		case ${opt} in
			o )
				OUTPUT_FILE=$OPTARG
				;;
			h )
				h_opt=1
				usage=1
				;;
			\? )
				echo "Invalid option: -$OPTARG" 1>&2
				usage=1
				exit_code=1
				;;
			: )
				echo "Invalid option: -$OPTARG requires an argument" 1>&2
				usage=1
				exit_code=1
				;;
		esac
	done

	shift $((OPTIND -1))
	if [ -f "$1" ];
	then
		SHELL_FILE=$1
	else
		echo "Missing shell file" 1>&2
		usage=1
		exit_code=1
	fi
	if [ "$usage" = "1" ]
	then
		print_usage
	fi
	if [ "$h_opt" = "1" ]
	then
		cat tester/doc
	fi
	if [ "$exit_code" = "1" ]
	then
		exit 1
	fi
}

list_vars $@
if [ -n "$SHELL_FILE" ]
then
	run_tests $TEST_DIRS
fi