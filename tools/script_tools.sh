#!/bin/sh
DRAW()
{
	draw_temp=$1
	while [ "$draw_temp" -gt 0 ]; do
		if [ "$draw_temp" -ge 10 ]; then
			echo -n "$2$2$2$2$2$2$2$2$2$2"
			draw_temp=`expr $draw_temp - 10`
		elif [ "$draw_temp" -ge 5 ]; then
			echo -n "$2$2$2$2$2"
			draw_temp=`expr $draw_temp - 5`
		elif [ "$draw_temp" -ge 2 ]; then
			echo -n "$2$2"
			draw_temp=`expr $draw_temp - 2`
		else
			echo -n "$2"
			draw_temp=`expr $draw_temp - 1`
		fi
	done
}

DRAW_LINE()
{
	content_length=`expr length "$2" + 2`
	line_left=`expr $1 - $content_length`
	line_left=`expr $line_left / 2`
	line_right=`expr $1 - $content_length - $line_left`
	if [ "$3" != "" ]; then
		echo -e "\033[$3;1m"
	fi

	echo ""; DRAW $line_left "*"; echo -n " "; echo -n "$2"; echo -n " "; DRAW $line_right "*"; echo ""

	if [ "$3" != "" ]; then
		echo -e "\033[0m"
	fi
}

DRAW_BOX()
{
	content_length=`expr length "$2"`
	line_left=`expr $1 - $content_length - 2`
	line_left=`expr $line_left / 2`
	line_right=`expr $1 - $content_length - $line_left - 2`

	if [ "$3" != "" ]; then
		echo -e "\033[$3;1m"
	fi

	echo ""
	DRAW $1 "*"
	echo ""
	echo -n "*"
	DRAW $line_left " "
	echo -n "$2"
	DRAW $line_right " "
	echo -n "*"
	echo ""
	DRAW $1 "*"
	echo ""

	if [ "$3" != "" ]; then
		echo -e "\033[0m"
	fi
}

SEARCH_AND_REPLACE()
{
	if [ -f "$1" ]; then
		filename=$1
		target_old=$2
		target_new=$3
	
		target_line=`cat $filename | grep -n "$2" | cut -f 1 -d :`

		if [ "$target_line" ]; then
			total_line=`cat $filename | wc -l`
	
			cat ${filename} | head -n `expr $target_line - 1` > ${filename}_temp
			echo "$target_new" >> ${filename}_temp
			cat ${filename} | tail -n `expr $total_line - $target_line` >> ${filename}_temp
	
			cat ${filename}_temp > ${filename}
			rm -rf ${filename}_temp
		fi
	fi
}
