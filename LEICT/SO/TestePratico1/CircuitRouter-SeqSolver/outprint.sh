#!/bin/bash

nomeFile="$2.res"
nomeFileWr="$nomeFile.wr"
nomeFileOld="$nomeFile.old"
nomeFileOldWr="$nomeFileOld.wr"

case $3 in
	1)
		if [ -e $nomeFile ]
		then
			if [ -e $nomeFileOld ]
			then
				rm $nomeFileOld
				printf "$1" >> $nomeFileOldWr
			else
				printf "$1" >> $nomeFileOldWr
			fi
		else
			printf "$1" >> $nomeFileWr
		fi
	;;
	2)
		if [ -e $nomeFileWr ]
		then
			printf "$1" >> $nomeFileWr
		else
			if [ -e $nomeFileOldWr ]
			then
				printf "$1" >> $nomeFileOldWr
			fi
		fi
	;;
	3)
		if [ -e $nomeFileWr ]
		then
			printf "$1" >> $nomeFileWr
			mv $nomeFileWr $nomeFile
		else
			if [ -e $nomeFileOldWr ]
			then
				printf "$1" >> $nomeFileOldWr
				mv $nomeFileOldWr $nomeFileOld
			fi
		fi
	;;
	4)
		if [ -e $nomeFileWr ]
		then
			printf "$1" >> $nomeFileWr
		else
			if [ -e $nomeFileOldWr ]
			then
				printf "$1" >> $nomeFileOldWr
			fi
		fi
	;;
esac