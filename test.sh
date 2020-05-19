#! /bin/bash

SHELL_DIR=$(dirname $(readlink -f "$0"))
TESTER_DIR=$SHELL_DIR/tester
TMP_DIR=$TESTER_DIR/tmp
TEST_DIRS=$(ls -r $TESTER_DIR/units)

output_print () {
	if [ -z "$OUTPUT_FILE" ]
	then
		echo -e "$1"
	else
		echo -e "$1" >> $OUTPUT_FILE
	fi
}

print_fail () {
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
}

make_test () {
	local sh_exit=$($SHELL_DIR/$SHELL_FILE $1/input >$TMP_DIR/stdout.sh 2>$TMP_DIR/stderr.sh)
	local bash_exit=$(/bin/bash $1/input >$TMP_DIR/stdout.bash 2>$TMP_DIR/stderr.bash)
	local stdout_diff=$(diff $TMP_DIR/stdout.sh $TMP_DIR/stdout.bash)
	local stderr_diff=$(diff $TMP_DIR/stderr.sh $TMP_DIR/stderr.bash)
	local ret=
	if [ -n "$stdout_diff" -o -n "$stderr_diff" -o "$bash_exit" = "$sh_exit" ]
	then
		print_fail $(basename $1) "$stdout_diff" "$stderr_diff" "$bash_exit" "$sh_exit"
		ret=1
	else
		output_print "\e[1;32mSUCCESS\e[0m $1"
		ret=0
	fi
	output_print "\n\n"
	return $ret
}

run_tests () {
	for i in "$@"
	do
		local test="$TESTER_DIR/units/$1"
		make_test $test
		shift
		rm -f $TMP_DIR/*
	done
}

print_usage () {
	echo 'Usage: test.sh [-h] [-f output_file] shell_file' 1>&2
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
