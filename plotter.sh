#! /bin/bash



OUTPUT_EXTENSION=".png"



# 0 or more than 2 parameters
if [ $# -ne 1 ] && [ $# -ne 2 ]; then
	echo "PARAMETERS ERROR - 1 or 2 parameters required:"
	echo -e "\t1: the path of the file to plot"
	echo -e "\t2: the optional flag -o to write the output on a .png file"
# first parameter does not exist
elif ! [ -e "$1" ]; then
	if [ $# -eq 1 ]; then
		echo "PARAMETER ERROR - \"$1\" does not exist"
	else
		echo "FIRST PARAMETER ERROR - \"$1\" does not exist"
	fi
# first parameter is not a regular file
elif ! [ -f "$1" ]; then
	if [ $# -eq 1 ]; then
		echo "PARAMETER ERROR - \"$1\" is not a regular file"
	else
		echo "FIRST PARAMETER ERROR - \"$1\" is not a regular file"
	fi
# second parameter specified and is not -o
elif [ $# -eq 2 ] && [ "$2" != "-o" ]; then
	echo "SECOND PARAMETER ERROR - only the optional flag \"-o\" is accepted"
# 1 parameter
elif [ $# -eq 1 ]; then
	# -p: persistent; -e: command list
	gnuplot -p -e "plot \"$1\" using 1:3 with lines"
# 2 parameters
else
	# -e: command list
	gnuplot -e "set term png;set key inside left top vertical Right noreverse enhanced autotitle box lt \"black\";set output \"${1}${OUTPUT_EXTENSION}\";plot \"$1\" using 1:3 with lines"
fi
