#!/bin/bash

RES=0
check_header() {
local line=0
while IFS= read -r var
do
	if [ "$line" -ge 11 ]
	then
		break
	fi
	if [[ "$var" != /\** ]]
	then
		RES=0
		return
	fi
	line=$((line + 1))
done < $1
RES=1
}

if [[ $1 == del ]]; then
	for arg in "$@"; do
		if [[ -e $arg ]]; then
			check_header $arg
			if [ "$RES" -eq 1 ]
			then
				vim -c :delete12 +:wq $arg
			fi
		fi
	done
elif [[ $1 == add ]]; then
	for arg in "$@"; do
		if [[ -e $arg ]]; then
			vim -c :Stdheader +:wq $arg
		fi
	done
else
	echo Usage: $0 [add] [del] \*.c
fi
