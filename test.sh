#! /bin/bash

SHELL_DIR=$(dirname $(readlink -f "$0"))
TESTER_DIR=$SHELL_DIR/tester
TMP_DIR=$TESTER_DIR/tmp
LOG_DIR=$TESTER_DIR/log
TEST_DIRS=$(ls $TESTER_DIR/units | sort)
FAIL_TESTS=0
SUCCESS_TESTS=0
TEST_RET=

### OUTPUT ###

output_print () {
	if [ -z "$OUTPUT_FILE" ]
	then
		echo -e "$1"
	else
		echo -e "$1" >> $SHELL_DIR/$OUTPUT_FILE
	fi
}

print_fail () {
	output_print $(printf "%.0s-" $(eval "echo {1.."$(($TERM_SIZE))"}"))
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
	output_print $(printf "%.0s-" $(eval "echo {1.."$(($TERM_SIZE))"}"))
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
	rm -rf $TMP_DIR/*
	REDIR_FILE=""
	cd $TMP_DIR
	env_setup $1/setup
	env LANG=us_US.UTF-8 GDM_LANG=us_US.utf8 LC_CTYPE=us_US.UTF-8 $SHELL_DIR/$SHELL_FILE $1/input >$LOG_DIR/stdout.sh 2>$LOG_DIR/stderr.sh
	local sh_exit=$?
	local redir_files=($(move_shell_redir $1))
	rm -rf $TMP_DIR/*
	cd $TMP_DIR
	env_setup $1/setup
	env LANG=us_US.UTF-8 GDM_LANG=us_US.utf8 LC_CTYPE=us_US.UTF-8 /bin/bash --posix $1/input >$LOG_DIR/stdout.bash 2>$LOG_DIR/stderr.bash
	local bash_exit=$?
	move_bash_redir $1
	rm -rf $TMP_DIR/*
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
	rm -rf $LOG_DIR/*
}

run_test_group () {
	local group=$1
	local group_basename=$(basename $group)
	local dirs=($(ls $group))
	output_print $(printf '=%.0s' $(eval "echo {1.."$(($TERM_SIZE))"}"))
	output_print "${group_basename^^}"
	output_print $(printf '=%.0s' $(eval "echo {1.."$(($TERM_SIZE))"}"))
	GROUP_FAIL=0
	GROUP_SUCCESS=0
	for i in "${dirs[@]}"
	do
		make_test $group/$i
		if [ "$TEST_RET" = "1" ]
		then
			GROUP_FAIL=$((GROUP_FAIL + 1))
			FAIL_TESTS=$((FAIL_TESTS + 1))
		else
			GROUP_SUCCESS=$((GROUP_SUCCESS + 1))
			SUCCESS_TESTS=$((SUCCESS_TESTS + 1))
		fi
	done
	output_print "$GROUP_FAIL/$(($GROUP_FAIL + $GROUP_SUCCESS)) tests failed"
}

run_tests () {
	for i in "$@"
	do
		local test_group="$TESTER_DIR/units/$i"
		run_test_group $test_group
		rm -rf $TMP_DIR/*
	done
	output_print $(printf '=%.0s' $(eval "echo {1.."$(($TERM_SIZE))"}"))
	output_print "TOTAL"
	output_print $(printf '=%.0s' $(eval "echo {1.."$(($TERM_SIZE))"}"))
	output_print "$(echo "$FAIL_TESTS out of $(($FAIL_TESTS + $SUCCESS_TESTS)) failed")"
	if [ -n "$OUPUT_FILE" ]
	then
		echo "$FAIL_TESTS out of $(($FAIL_TESTS + $SUCCESS_TESTS)) failed"
	fi
}

print_usage () {
	echo 'Usage: test.sh [-h] [-o output_file] shell_file' 1>&2
}

get_term_size () {
	if [ -n "$OUTPUT_FILE" ]
	then
		TERM_SIZE="45"
	else
		TERM_SIZE="$(($(tput cols)))"
	fi
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
get_term_size

read -p "Which test do you wish to perform?
	0)	All
	1)	42pdf_corr
	2)	builtin_alias
	3)	builtin_cd
	4)	builtin_echo
	5)	builtin_env
	6)	builtin_exit
	7)	builtin_hash
	8)	builtin_type
	9)	builtin_test
	10)	current
	11)	error_handling
	12)	heredocs
	13)	logical_ops
	14)	pipes
	15)	quoting
	16)	redirections
	17)	signals
	18)	parameter_exp
	19)	history
	20)	more	
	21)	exit
    > " ret
case $ret in
	0 ) ;;
	1 ) TEST_DIRS="42pdf_corr";;
	2 ) TEST_DIRS="builtin_alias";;
	3 ) TEST_DIRS="builtin_cd";;
	4 ) TEST_DIRS="builtin_echo";;
	5 ) TEST_DIRS="builtin_env";;
	6 ) TEST_DIRS="builtin_exit";;
	7 ) TEST_DIRS="builtin_hash";;
	8 ) TEST_DIRS="builtin_type";;
	9 ) TEST_DIRS="builtin_test";;
	10 ) TEST_DIRS="current";;
	11 ) TEST_DIRS="error_handling";;
	12 ) TEST_DIRS="heredocs";;
	13 ) TEST_DIRS="logical_ops";;
	14 ) TEST_DIRS="pipes";;
	15 ) TEST_DIRS="quoting";;
	16 ) TEST_DIRS="redirections";;
	17 ) TEST_DIRS="signals";;
	18 ) TEST_DIRS="parameter_exp";;
	19 ) TEST_DIRS="history";;
	20 ) TEST_DIRS="more";;
	21 ) exit;;
esac

if [ -n "$SHELL_FILE" -a "$(basename $SHELL_FILE)" != "42sh_db" ]
then
	run_tests $TEST_DIRS
fi
