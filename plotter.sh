#! /bin/bash



X_AXIS_NAME="Generations"

Y_AXIS_NAME="Fitness values"

OUTPUT_EXTENSION=".png"



# neither 1 nor 3 parameters
if [ $# -ne 1 ] && [ $# -ne 3 ]; then
	echo "PARAMETERS ERROR - 1 or 3 parameters required:"
	echo -e "\t1: the path of the file to plot"
	echo -e "\t2: the optional flag -o to write the output on a .png file"
	echo -e "\t3: with the optional flag -o, specify the title of the plot"
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
elif [ $# -eq 3 ] && [ "$2" != "-o" ]; then
	echo "SECOND PARAMETER ERROR - only the optional flag \"-o\" is accepted"
# third parameter specified and is empty string
elif [ $# -eq 3 ] && [ -z "$3" ]; then
	echo "THIRD PARAMETER ERROR - specify a non-empty string for the title of the plot"
# 1 parameter
elif [ $# -eq 1 ]; then
	# -p: persistent; -e: command list
	gnuplot -p -e "unset key;set title \"$1\";set xlabel \"${X_AXIS_NAME}\";set ylabel \"${Y_AXIS_NAME}\";plot \"$1\" using 1:3 with lines"
# 3 parameters
else
	# -e: command list
	gnuplot -e "set term png;set output \"${1}${OUTPUT_EXTENSION}\";unset key;set title \"$3\";set xlabel \"${X_AXIS_NAME}\";set ylabel \"${Y_AXIS_NAME}\";plot \"$1\" using 1:3 with lines"
fi
